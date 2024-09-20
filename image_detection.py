from keras.models import load_model  # TensorFlow is required for Keras to work
import cv2  # Install opencv-python
import numpy as np
from gpiozero import Servo
from gpiozero.pins.pigpio import PiGPIOFactory
from time import sleep
import threading
import serial

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
sleep(2)  # Wait for the Arduino to initialize

def angle_to_servo_value(angle, min_angle=0, max_angle=180):
    """Convert angle to servo value."""
    return (angle - min_angle) / (max_angle - min_angle) * 2 - 1

factory = PiGPIOFactory()
servo = Servo(17, pin_factory=factory)

# Disable scientific notation for clarity
np.set_printoptions(suppress=True)

# Load the model
model = load_model("keras_model.h5", compile=False)

# Load the labels and strip newline characters
with open("labels.txt", "r") as file:
    class_names = [line.strip() for line in file]

# Initialize the camera (0 is usually the default camera)
camera = cv2.VideoCapture(0)

# Reduce frame resolution to improve processing speed
camera.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)

# Global variables
stop_sweep = False
stop_angle = 0
current_angle = 0  # Global variable to keep track of the current servo angle
data_sent = False  # Flag to ensure data is only sent once for each detection
detected = False   # Flag to indicate detection has happened
start_processing = False  # Flag to control whether frame processing is active

def sweep_servo():
    """Sweep the servo continuously."""
    global stop_sweep, stop_angle, current_angle
    while True:
        if not start_processing:
            sleep(0.1)  # Check every 0.1 seconds if processing should start
            continue

        if stop_sweep:
            # Hold at the stop angle
            servo.value = angle_to_servo_value(stop_angle)
            current_angle = stop_angle
            sleep(0.1)  # Small delay to reduce CPU usage
            continue
        
        # Sweep from 0 to 180 degrees
        for angle in range(0, 181, 3):
            if stop_sweep:
                break
            servo.value = angle_to_servo_value(angle)
            current_angle = angle
            sleep(0.03)  # Adjust speed as needed

        # Sweep from 180 to 0 degrees
        for angle in range(180, -1, -3):  # Move in steps of 3 degrees
            if stop_sweep:
                break
            servo.value = angle_to_servo_value(angle)
            current_angle = angle
            sleep(0.03)  # Adjust speed as needed

def set_servo_position(angle):
    """Set servo to a specific position and stop sweeping."""
    global stop_sweep, stop_angle, current_angle
    stop_angle = angle
    stop_sweep = True
    servo.value = angle_to_servo_value(angle)
    current_angle = angle
    print(f"Stopped at {angle} degrees")

def reset_flags():
    """Reset relevant flags in the Python script."""
    global stop_sweep, detected, data_sent
    stop_sweep = False  # Allow the servo to sweep again
    detected = False    # Reset detection status
    data_sent = False   # Reset data sent status
    print("Flags reset in Python")

def process_frames():
    """Capture and process frames from the camera."""
    global stop_sweep, data_sent, detected, start_processing
    while True:
        if not start_processing:
            sleep(0.1)  # Check every 0.1 seconds if processing should start
            continue

        ret, image = camera.read()
        if not ret:
            print("Failed to grab frame")
            break

        # Resize the raw image into (224-height, 224-width) pixels
        image_resized = cv2.resize(image, (224, 224), interpolation=cv2.INTER_AREA)

        # Show the image in a window
        cv2.imshow("Webcam Image", image_resized)

        # Make the image a numpy array and reshape it to the model's input shape
        image_array = np.asarray(image_resized, dtype=np.float32).reshape(1, 224, 224, 3)

        # Normalize the image array
        image_array = (image_array / 127.5) - 1

        # Predict using the model
        prediction = model.predict(image_array)
        index = np.argmax(prediction)
        class_name = class_names[index]
        confidence_score = prediction[0][index]

        # Print prediction and confidence score
        print(f"Class: {class_name}, Confidence Score: {confidence_score * 100:.2f}%")

        # Check for specific class name and set the servo position
        if class_name == "1 bottles" and not detected:
            # Stop the servo and mark detection
            set_servo_position(current_angle)
            detected = True  # Object detected

        if detected and not data_sent:
            # Send data only once
            if 0 < current_angle <= 45:
                ser.write(b'a')
                sleep(2)
                
            elif 45 < current_angle <= 90:
                ser.write(b'b')
                sleep(2)
            elif 90 < current_angle <= 125:
                ser.write(b'c')
                sleep(2)
            elif 125 < current_angle <= 180:
                ser.write(b'd')
                sleep(2)            
            data_sent = True  # Set the flag to stop further data sending
            print("Data sent to Arduino")

        # Listen to the keyboard for presses
        keyboard_input = cv2.waitKey(1)
        # 27 is the ASCII code for the ESC key
        if keyboard_input == 27:
            break

def read_serial():
    """Read data from the serial port."""
    global ser, start_processing
    while True:
        if ser.in_waiting > 0:
            serial_data = ser.readline().decode('utf-8').strip()
            print(f"Received from Arduino: {serial_data}")
            
            if serial_data == 's':
                # Start the processing
                start_processing = True
                print("Starting frame processing")

            elif serial_data == 'p':
                # Reset flags in Python
                reset_flags()

# Start threads
sweep_thread = threading.Thread(target=sweep_servo)
sweep_thread.start()

serial_thread = threading.Thread(target=read_serial)
serial_thread.start()

try:
    # Start frame processing
    process_frames()
finally:
    # Release resources and close windows
    stop_sweep = True  # Stop sweeping
    camera.release()
    cv2.destroyAllWindows()
    ser.close()

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Stepper.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified();
float calibrationOffset = 11.1;

#define LIMIT_SWITCH_PIN 2
const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 22, 24, 23, 25);

#define in1 8
#define in2 9
#define in3 10  
#define in4 11

const int trigPin = 6;
const int echoPin = 5;

#define Sensor A0
float R1 = 9000.0;  // Resistor 1 in ohms
float R2 = 7000.0;  // Resistor 2 in ohms
const float MAX_VOLTAGE = 12.0;  // Maximum voltage to measure

bool task1 = false;
bool task2 = false;
bool entire_task = false;
char msg = 's';
bool msg_received = false; 
bool arm_msg = false;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  lcd.begin();
  lcd.backlight();
  myStepper.setSpeed(60);
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  if (!bmp.begin()) {
    Serial.print("Couldn't find the BMP180 sensor");
    while (1);
  }
}

void loop() {
  int percentage = voltage();
  float pressure = waterPressure();

  if (!task1) {
    int water = waterlevel();
    if (water >= 400) {
      forward();
      delay(5000);
      stop();
      task1 = true;
    } else {
      forward();
      delay(500);
    }
  }

  if (task1 && !task2) {
    int switchState = digitalRead(LIMIT_SWITCH_PIN);
    if (pressure < 1.5) {
      while (switchState == HIGH) {
        myStepper.step(stepsPerRevolution);
        delay(10);
        switchState = digitalRead(LIMIT_SWITCH_PIN);
      }
      stopStepper();
      Serial.write(msg);
      task2 = true;
    }
  }

  if (task2) {
    if (Serial.available()) {
      char receivedChar = Serial.read();
      Serial.println(receivedChar);
      delay(1000);
      switch (receivedChar) {
        case 'a':
          left();
          delay(600);
          stop();
          msg_received = true;
          break;
        case 'b':
          left();
          delay(300);
          stop();
          msg_received = true;
          break;
        case 'c':
          right();
          delay(300);
          stop();
          msg_received = true;
          break;
        case 'd':
          right();
          delay(600);
          stop();
          msg_received = true;
          break;
      }
    }

    if (msg_received) {
      int distance = getDistance();
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      if (distance <= 10 && !arm_msg) {
          Serial.println("Object detected");
          Serial2.write('r');  // Inform the other device that an object was detected
          arm_msg = true;
          msg_received = false;
          stop();  // Stop any motor or action related to object detection
      }
    }

    if (Serial2.available()) {
      char c = Serial2.read();      
      if (c == 's') {  
          arm_msg = false;
          Serial.write('p');
          delay(1000);
      }
    }
  }
  delay(1000);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  return distance;
}

void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Moving forward");
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.println("Stopped");
}

void stopStepper() {
  myStepper.setSpeed(0);
  Serial.println("Stepper Stopped");
}

void left() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Turning left");
}

void right() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("Turning right");
}

int voltage() {
  int value = analogRead(Sensor);
  float vOUT = value * (MAX_VOLTAGE / 1023.0);
 float vIN = (vOUT * (R1 + R2)) / R2;
  float percentage = (vOUT / MAX_VOLTAGE) * 100;

  if (percentage > 100.0) {
    percentage = 100.0;
  }
  if (percentage < 30) {
    Serial.println("Low battery");
    myStepper.step(-stepsPerRevolution);
//    delay(10);
    Serial.write('e');
    task1=true;
    task2=true;
  }
 
  lcd.setCursor(0, 0);
  lcd.print("Battery: ");
  lcd.print(percentage, 2);
  lcd.println(" %");

  return percentage;
}

int waterlevel() {
  int level = analogRead(A1);
  Serial.println(level);
  return level;
}

float waterPressure() {
  sensors_event_t event;
  bmp.getEvent(&event);
  float pressure = 0;
  float temperature;

  if (!bmp.getEvent(&event)) {
    Serial.println("Error reading BMP180 sensor data");
    return 0;
  }

  if (event.pressure) {
    float calibratedPressure = event.pressure + calibrationOffset;
    pressure = calibratedPressure / 1013.25;
    lcd.setCursor(0, 1);  // Start at the beginning of the second line
    lcd.print("P:");
    lcd.print(pressure);
    lcd.print("pa ");  // Add spaces to ensure there's a gap between pressure and temperature

    bmp.getTemperature(&temperature);
    lcd.print("T:");
    lcd.print(temperature);
    lcd.print("C");
  } else {
    Serial.println("No pressure data");
  }
  return pressure;
}

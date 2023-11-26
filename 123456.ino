
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define enA 1
#define enB 6
int xpin = A0;
int ypin = A1;
int xval = 0;
int yval = 0;
int motorSpeedA = 0;
int motorSpeedB = 0;
RF24 radio(7, 8);  // CE, CSN

const byte address[6] = "00001";

void setup() {
  pinMode(xpin, INPUT);
  pinMode(ypin, INPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.begin(9600);
}

void loop() {
  xval = analogRead(xpin);
  yval = analogRead(ypin);
  Serial.print("X AXIS :");
  Serial.println(xval);
  Serial.print("Y AXIS :");
  Serial.println(yval);
  delay(100);

  if (xval > 300 && xval < 350 && yval > 310 && yval < 360) {
    const char msg[] = "0";
    radio.write(&msg, sizeof(msg));
    delay(100);
  }
  if (yval > 310 && yval < 360) {
    if (xval < 320) {
      const char msg[] = "1";
      //  motorSpeedA = map(yval, 300, 0, 0, 200);
      //   motorSpeedB = map(yval, 300, 0, 0, 200);      
      radio.write(&msg, sizeof(msg));
      delay(100);
    }
    if (xval > 360) {
      const char msg[] = "2";
      // motorSpeedA = map(xval, 380, 1023, 0, 200);
      // motorSpeedB = map(xval, 380, 1023, 0, 200);
      radio.write(&msg, sizeof(msg));
      delay(100);
    }
    

  } else {
    if (xval > 300 && xval < 350) {
      if (yval < 310) {
        const char msg[] = "3";
        // motorSpeedA = map(yval, 300, 0, 0, 200);
        // motorSpeedB = map(yval, 300, 0, 0, 200);
        radio.write(&msg, sizeof(msg));
        delay(100);
      }

      if (yval > 360) {
        const char msg[] = "4";
        // motorSpeedA = map(yval, 350,500 , 0, 200);
        // motorSpeedB = map(yval, 350, 500, 0, 200);
        radio.write(&msg, sizeof(msg));
        delay(100);
        
      }
      

    }
  }
//   if(motorSpeedA < 30){motorSpeedA = 0;}
// if(motorSpeedB < 30){motorSpeedB = 0;}
  
// analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
// analogWrite(enB, motorSpeedB);
}
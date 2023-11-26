#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);  // CE, CSN
// #define enA 1
#define in1 2
#define in2 3

#define in3 4
#define in4 5
// #define enB 6

// int motorSpeedA = 0;
// int motorSpeedB = 0;

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  // pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // pinMode(enB, OUTPUT);
}

void loop() {
  if (radio.available()) {
    char msg[32];
    radio.read(&msg, sizeof(msg));
    char receivedchar = (char)msg[0];
    if (receivedchar == '0') {
      Serial.println(msg);
      Serial.println("STOP");
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      // Set Motor B stop
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      // delay(500);
    }
    if (receivedchar == '1') {
      Serial.println(msg);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      // Set Motor B stop
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      Serial.println("FORWORD");
      delay(10);
    }
    if (receivedchar == '2') {
      Serial.println(msg);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      // Set Motor B stop
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      Serial.println("BACKWORD");
      delay(10);
    }
    if (receivedchar == '3') {
      Serial.println(msg);
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      // Set Motor B backward
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      Serial.println("RIGHT");
      delay(10);
    }
    if (receivedchar == '4') {
      Serial.println(msg);
      // Set Motor B backward
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      // Set Motor B forward
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      Serial.println("LEFT");
      delay(10);
    }
  }
  //  analogWrite(enA, motorSpeedA); 
  //   analogWrite(enB, motorSpeedB);
}
#include <HX711_ADC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
Servo myservo;
HX711_ADC LoadCell(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int trigPin1 = 5;
int echoPin1 = 4;
int trigpin = 9;
int echopin = 10;
int green = 13;
int yellow = 12;
int red = 11;
int in1 = 8;
int in2 = 6;

int duration1, distance1, distance2, duration2;
void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  myservo.attach(7);

  LoadCell.begin();      // start connection to HX711
  LoadCell.start(2000);  // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(395);
  lcd.init();
  lcd.clear();
  lcd.begin(16, 2);
  lcd.backlight();
  myservo.write(0);
}

void loop() {
  LoadCell.update();  // retrieves data from the load cell
  float i = 0;
  i = LoadCell.getData();  // get output value
  lcd.setCursor(0, 0);
  lcd.print("Weight[g] : ");
  lcd.setCursor(0, 1);
  lcd.print(i);
  // Serial.println("Weight[g]:");  // print out to LCD
  // Serial.println(i);             // print out the retrieved value to the second row
  delay(100);
  if (i > 200) {
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);
    distance1 = (duration1 / 2) / 29.1;

    if (distance1 < 10) {
      Serial.println(distance1);
      myservo.write(90);
      delay(2000);
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);

    } else {

      myservo.write(0);
      delay(1000);
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);

    }
  }
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration2 = pulseIn(echopin, HIGH);
  distance2 = (duration2 / 2) / 29.1;
  Serial.println(distance2);
  if (distance2 > 30) {
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);

  } else if (distance2 >5 && distance2 < 30) {
    digitalWrite(yellow, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);

  } else if (distance2 <= 5) {
    digitalWrite(red, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);

  } else {
    // digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
  }
}

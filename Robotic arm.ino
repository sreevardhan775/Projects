#include <Servo.h>

Servo base_servo;
Servo shoulder_servo;
Servo elbow_servo;
Servo wrist_servo;
Servo pitch_servo;
Servo door_servo;

const int irPin = 9;
int irval;
char msg;
int bpos = 0;
int spos = 120;
int epos = 90;
int wpos = 50;
int ppos = 0;

bool task1 = false;
bool task2 = false;
bool task3 = false;
bool task4 = false;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  base_servo.attach(22);
  shoulder_servo.attach(23);
  elbow_servo.attach(24);
  wrist_servo.attach(25);
  pitch_servo.attach(26);
  door_servo.attach(27);

  pinMode(irPin, INPUT);

  base_servo.write(bpos);
  delay(1000);
  shoulder_servo.write(spos);
  elbow_servo.write(epos);
  wrist_servo.write(wpos);
  pitch_servo.write(ppos);
  door_servo.write(0);
//  Serial2.write('s');

}

void loop() {
  task4 = false;
  irval = digitalRead(irPin);
  delay(200);

  if (Serial2.available()) {
    msg = Serial2.read();
    Serial.println(msg);

    if (msg == 'r' && !task1) {
      // Perform servo movements
      for (wpos; wpos <= 120; wpos += 1) {
        wrist_servo.write(wpos);
        delay(15);
      }
      for (spos; spos >= 20; spos -= 1) {
        shoulder_servo.write(spos);
        delay(15);
      }
      for (epos; epos >= 50; epos -= 1) {
        elbow_servo.write(epos);
        delay(15);
      }
      task1 = true;
    }
  }

  if (irval == 0 && task1 && !task2) {
    for (ppos; ppos <= 80; ppos += 1) {
      pitch_servo.write(ppos);
      delay(15);
    }
    task2 = true;
  }

  if (task2 && !task3) {
    // Perform further servo movements
    for (spos = 20; spos <= 120; spos += 1) {
      shoulder_servo.write(spos);
      delay(15);
    }
    for (bpos; bpos <= 90; bpos += 1) {
      base_servo.write(bpos);
      delay(15);
    }
    for (spos; spos >= 40; spos -= 1) {
      shoulder_servo.write(spos);
      delay(15);
    }
    door_servo.write(90);
    delay(500);
    for (ppos = 80; ppos >= 0; ppos -= 1) {
      pitch_servo.write(ppos);
      delay(15);
    }
    delay(500);
    door_servo.write(0);
    task3 = true;
  }

  if (task3 && !task4) {
    // Final servo movements
    for (spos = 40; spos <= 120; spos += 1) {
      shoulder_servo.write(spos);
      delay(15);
    }
    for (epos = 50; epos <= 90; epos += 1) {
      elbow_servo.write(epos);
      delay(15);
    }
    for (wpos = 120; wpos >= 50; wpos -= 1) {
      wrist_servo.write(wpos);
      delay(15);
    }
    for (bpos = 90; bpos >= 0; bpos -= 1) {
      base_servo.write(bpos);
      delay(15);
    }
    task1 = false;
    task2 = false;
    task3 = false;
    task4 = true;
    Serial2.write('s');  // Acknowledge completion
  }
}

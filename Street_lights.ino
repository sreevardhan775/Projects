int ldr = A0;
int ldrval;
int led1 = 3;
int led2 = 5;
int trigpin1 = 6;
int echopin1 = 7;
int trigpin2 = 8;
int echopin2 = 9;
int distance1, duration1, distance2, duration2;
void setup() {
  Serial.begin(9600);
  pinMode(trigpin1, OUTPUT);
  pinMode(echopin1, INPUT);
  pinMode(trigpin2, OUTPUT);
  pinMode(echopin2, INPUT);
  pinMode(ldr, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {

  ldrval = analogRead(ldr);
  Serial.println(ldrval);
  //ultra 1
  digitalWrite(trigpin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin1, LOW);
  duration1 = pulseIn(echopin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  //ultra 2
  digitalWrite(trigpin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin2, LOW);
  duration2 = pulseIn(echopin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  if (ldrval > 500) {
    analogWrite(led1, 24);
    analogWrite(led2, 24);
    if (distance1 <= 5) {
      digitalWrite(led1, HIGH);
      delay(1000);
    }
    else if (distance2 <= 5) {
      digitalWrite(led2, HIGH);
      delay(1000);
    }
  }

  else {
    if (ldrval < 500) {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
    }
  }
}

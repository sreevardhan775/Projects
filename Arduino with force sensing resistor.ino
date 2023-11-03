const int fsrpin1=A0;
const int fsrpin2=A1;
const int buzzer=9;
const int onThreshold=600;
const int offThreshold=300;
int fsrvalue1;
int fsrvalue2;

void setup() {
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600);

}

void loop() {
  fsrvalue1=analogRead(fsrpin1);
  fsrvalue2=analogRead(fsrpin2);
  if(fsrvalue1==0 || fsrvalue2==0){
    digitalWrite(buzzer,HIGH);
  }
  else if(fsrvalue1>0|| fsrvalue2>0){
    digitalWrite(buzzer,LOW);
  }
  Serial.println("\nFSR 1 : ");
  Serial.print(fsrvalue1);
  //Serial.print("/t");
  Serial.print("\tFSR 2 : ");
  Serial.print(fsrvalue2);
  delay(500);
}


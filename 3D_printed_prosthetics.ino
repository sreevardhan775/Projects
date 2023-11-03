#include <SoftwareSerial.h>
#include <Servo.h>
 
SoftwareSerial mySerial(0,1); // RX | TX
 
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
  
 
int servothumb = 4;
int servoindex = 5;
int servomiddle = 6;
int servoring = 7;
int servolittle = 9;

int servo1Angle = 0;   // servo position in degrees
int servo2Angle = 0;
int servo3Angle = 0;
int servo4Angle = 0;
int servo5Angle = 0; 
char command;
 
void setup() {
   Serial.begin(9600);
   mySerial.begin(9600);
   Serial.println("You're connected via Bluetooth");
   servo1.attach(servothumb);
   servo2.attach(servoindex);
   servo3.attach(servomiddle);
   servo4.attach(servoring);
   servo5.attach(servolittle);
   servo1.write(90);
     servo2.write(180);
      servo3.write(180);
       servo4 .write(0);
       servo5 .write(0);
 }
 
 

void loop() {
if (mySerial.available())
   {
    command=(mySerial.read());
    //show one
    if (command=='1')
    {
      Serial.println("showing one");
      servo1.write(0);
      servo2.write(180);
      servo3.write(0);
      servo4.write(180);
      servo5.write(180);

    }
//show two
     if (command=='2')
    {
       Serial.println("showing two");
      servo1.write(0);
      servo2.write(180);
      servo3.write(180);
      servo4.write(180);
      servo5.write(180);

    }
    //show 3
    if (command=='3')
    {
      Serial.println("showing three");
      servo1.write(0);
      servo2.write(180);
      servo3.write(180);
      servo4.write(0);
      servo5.write(180);

    }
    //show 4
    if (command=='4')
    {
      Serial.println("Showing four");
      servo1.write(0);
      servo2.write(180);
      servo3.write(180);
      servo4.write(0);
      servo5.write(0);

    }
    //say hi
        if (command=='5')
    {
        Serial.println("saying hi");
          servo5.write(180);
       delay(120);
        servo4.write(180);
          delay(115);
          servo3.write(0);
          delay(110);
          servo2.write(0);
          delay(105);
          servo5.write(0);
          delay(100);
          servo4.write(0);
          delay(95);
          servo3.write(180);
          delay(90);
          servo2.write(180);  
          }
          if (command=='6')
    {
      Serial.println("swag");
      servo1.write(0);
      servo2.write(180);
      servo3.write(0);
      servo4.write(180);
      servo5.write(0);

    }
//    show super
    if(command=='7'){
      Serial.println("Showing super");
      servo1.write(0);
      servo2.write(0);
      servo3.write(180);
      servo4.write(0);
      servo5.write(0);
    }
    if (command=='8')
    {
      Serial.println("fist");
      servo1.write(0);
      servo2.write(0);
      servo3.write(0);
      servo4.write(180);
      servo5.write(180);
  
} 
    
    if(command=='f'){
      Serial.println("F**K");
      servo1.write(0);
      servo2.write(0);
      servo3.write(180);
      servo4.write(180);
      servo5.write(180);
    }
    
      
//     set to normal pos
     if (command=='s')
    {
      Serial.println("normal");
      servo1.write(90);
      servo2.write(180);
      servo3.write(180);
      servo4.write(0);
      servo5.write(0);
  
} 
   }
   }

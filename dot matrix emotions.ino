#include <LedControl.h>
int DIN = 12;
int CS = 10;
int CLK = 11;
LedControl lc=LedControl(DIN,CLK,CS,1);
int buttonstate1=0;
int buttonstate2=0;
int buttonstate3=0;
int buttonstate4=0;
//int buttonstate5=0;

void setup() {
lc.shutdown(0,false);       
 lc.setIntensity(0,1);      //Adjust the brightness maximum is 15
 lc.clearDisplay(0);    
pinMode(4,INPUT);
pinMode(5,INPUT);
pinMode(6,INPUT);
pinMode(7,INPUT);
// pinMode(4,INPUT);

}

void loop() {
  buttonstate1=digitalRead(4);
    buttonstate2=digitalRead(5);
  buttonstate3=digitalRead(6);
  buttonstate4=digitalRead(7);
  // buttonstate5=digitalRead(5);

      byte sad[8]=   {0xFF,0x81,0xA5,0x81,0x99,0xA5,0x81,0xFF,};
      byte happy[8]={0xFF,0x81,0xA5,0x81,0xA5,0x99,0x81,0xFF,};
      byte normal[8]={0xFF,0x81,0xA5,0x81,0x81,0xBD,0x81,0xFF,};
      byte surprise[8]={0xFF,0x81,0xA5,0x81,0x99,0xA5,0x99,0xFF,};
     byte heart[8]=   {0x24,0x5A,0x99,0x81,0x81,0x42,0x24,0x18,};
     if(buttonstate1==HIGH){
         printByte(happy);
        delay(2000);
     }
     else if(buttonstate2==HIGH){
       printByte(sad);  
  delay(2000);
     }
     else if(buttonstate3==HIGH){
      printByte(surprise);
      delay(2000);
     } 
  
   else if(buttonstate4==HIGH){
   printByte(heart);  
  delay(2000);
   }
   else{
     printByte(normal);
    delay(100);
   }
}

void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}


#include <Wire.h>
#include<Servo.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
int temp;
int Buzzer=11;
void setup() {
                        // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
   lcd.clear();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Temp Monitor");
  Serial.begin(9600);
  pinMode(Buzzer,OUTPUT);
  Serial.println("Adafruit MLX90614 test"); 
  mlx.begin();  
}
void loop() {
  temp=mlx.readObjectTempC();
  Serial.print("Temperature = ");
   Serial.print(temp);
  //  lcd.setCursor(1,1);
  //  lcd.print("Temparature :"); 
  //  lcd.print(temp);
   Serial.print("*C\tObject = "); Serial.print(mlx.readAmbientTempC()); Serial.println("*C");  

    delay(500);
  if(temp>38 && temp<42){
    digitalWrite(Buzzer,HIGH);
    lcd.clear();
lcd.setCursor(2,0);
   lcd.print("PLEASE VISIT"); 
   lcd.setCursor(2,1);
   lcd.print("MEDICAL ROOM"); 
    delay(1000);
   
  }
  else{
    digitalWrite(Buzzer,LOW);
    lcd.clear();
     lcd.setCursor(2,0);
  lcd.print("Temp Monitor");
     lcd.setCursor(1,1);
   lcd.print("Temparature :"); 
   lcd.print(temp);
    delay(500);
  }
}
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27,20,4);
SoftwareSerial mySerial(3, 2);

float readVoltage = 0;
int voltageInput = A1;
String mobileNumber = "+639564114308";
boolean once = false;


void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.clear();         
  lcd.backlight(); 
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CSQ");
  updateSerial();
  mySerial.println("AT+CCID");
  updateSerial();
  mySerial.println("AT+CREG?");
  updateSerial();


  
  constantScreen();
  delay(1000);
}

void loop() {
  readVoltage = analogRead(voltageInput)/40.92;
  printToLCD(0,2, (String)readVoltage + "V");

  if(readVoltage < 5 && !once){
      sendSMS("Low battery, please charge.");
      once = true;
  }
  
  updateSerial();
}

void printToLCD(int x, int y, String text){
  lcd.setCursor(x,y);
  lcd.print(text);
}

void constantScreen(){
  printToLCD(0,0, "Battery Monitoring");
  printToLCD(0,1, "Voltage Reading: ");
  printToLCD(0,2, (String)readVoltage + "V");
  printToLCD(0,3, "reading...");
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}


void sendSMS(String message){
   mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\""+mobileNumber+"\"");
  updateSerial();
  mySerial.print(message);
  updateSerial();
  mySerial.write(26);
}

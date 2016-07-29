#include <SoftwareSerial.h>

#define rxPin 8
#define txPin 7

SoftwareSerial mySerial(rxPin, txPin); // RX, TX
char myChar ; 

void setup() {
   Serial.begin(9600);   
  Serial.println("Goodnight moon!");

  mySerial.begin(9600);
  mySerial.println("Hello, world?");

  pinMode(13, OUTPUT);
}

void loop(){
  while(mySerial.available()){
    myChar = mySerial.read();
    Serial.print(myChar);
  }

  while(Serial.available()){
   myChar = Serial.read();
   mySerial.print(myChar);
  }

}

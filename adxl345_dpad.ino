#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define KEY_UP_ARROW 0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW 0xD7
#define ENTER_KEY 0xB0

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

//on/off button for accelerometer d-pad
int dPadFlag = HIGH;
int reading;
int previous = LOW;
long debounce = 200;
long time = 0; 
int dPadEnable = 11; //for turning the d-pad on, so you can test without losing control of the keyboard
int ledPin = A5;

//main buttons
int xButton = A3; //A button on gamecube
int zButton = A4; //B button on gamecube
int aButton = 6; //A button on gamecube
int sButton = 9; //A button on gamecube
int startButton = 10; //A button on gamecube



void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
}

void setup(void) 
{
  pinMode(dPadEnable, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(xButton, INPUT);  
  digitalWrite(xButton, HIGH);
  pinMode(zButton, INPUT);  
  digitalWrite(zButton, HIGH);  
  pinMode(aButton, INPUT);  
  digitalWrite(aButton, HIGH);
  pinMode(sButton, INPUT);  
  digitalWrite(sButton, HIGH);
  pinMode(startButton, INPUT);  
  digitalWrite(startButton, HIGH);
  
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);
  Serial.println("Accelerometer D-pad Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  
  accel.setRange(ADXL345_RANGE_2_G);
  displaySensorDetails();
  Serial.println("");
}

void loop(void) 
{
  sensors_event_t event; 
  accel.getEvent(&event);

    if (digitalRead(xButton) == 0)  // if the button goes low
  {
    Keyboard.write('x'); // send a 'x' to the computer via Keyboard HID
    //delay(50);  // delay so there aren't a kajillion x's
  }
    if (digitalRead(zButton) == 0) 
  {
    Keyboard.write('z');
    //delay(50);  // delay so there aren't a kajillion z's
  }
    if (digitalRead(aButton) == 0) 
  {
    Keyboard.write('a');  
    //delay(50);  // delay so there aren't a kajillion z's
  }
    if (digitalRead(sButton) == 0) 
  {
    Keyboard.write('s');
    //delay(50);  // delay so there aren't a presses
  }
    if (digitalRead(startButton) == 0)
  {
    Keyboard.write(ENTER_KEY); 
    //delay(50);  // delay so there aren't a kajillion key presses
  }
 
  /* Display the results (acceleration is measured in m/s^2) */
  /*Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");*/

  reading = digitalRead(dPadEnable);
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (dPadFlag == HIGH)
      dPadFlag = LOW;
    else
      dPadFlag = HIGH;

    time = millis();    
  }
  previous = reading;
  processAccelerometer(event.acceleration.x,event.acceleration.y, event.acceleration.z); 
  //delay(10); //vary delay based on demands
}


void processAccelerometer(int16_t XReading, int16_t YReading, int16_t ZReading)
{
  if (dPadFlag == HIGH ){
    digitalWrite(ledPin, HIGH);
      if( XReading > 3.75 ){
        Keyboard.write(KEY_LEFT_ARROW);
      }
      if( XReading < -5 ){
        Keyboard.write(KEY_RIGHT_ARROW);
      }
      if( YReading < -15 ){
        Keyboard.write(KEY_UP_ARROW);
      }
      if( YReading > -6 ){
        Keyboard.write(KEY_DOWN_ARROW);
      }
  }

  else{
    digitalWrite(ledPin, LOW);
    return;
  }
}



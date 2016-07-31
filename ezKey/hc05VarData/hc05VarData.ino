#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>

#define rxPin 8
#define txPin 7
SoftwareSerial hc05Serial(rxPin, txPin); // RX, TX

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

//hc-05 bluetooth
int bluePin=13; // led on D13 will show blink on / off
int BluetoothData;
int upThreshold = -15;
int downThreshold = -6;
int leftThreshold = 4;
int rightThreshold = -5;

//on/off button for accelerometer d-pad
int testFlag = HIGH;
int testButtonReading;
int previous = LOW;
long debounce = 200;
long time = 0; 
int testEnableButton = 11; //for turning the d-pad on, so you can test without losing control of the keyboard
int ledPin = 2;

//button press simulation
int up = A2;
int down = 12;
int left = 10;
int right = 9;

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
}

void setup(void) 
{
  pinMode(testEnableButton, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  //hc-05
  pinMode(bluePin,OUTPUT);
  hc05Serial.begin(9600);
  hc05Serial.println("Bluetooth On please press 1 or 0 blink LED ..");
  
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
 
  /* Display the results (acceleration is measured in m/s^2) */
  /*Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");*/

  testButtonReading = digitalRead(testEnableButton);
  if (testButtonReading == HIGH && previous == LOW && millis() - time > debounce) {
    if (testFlag == HIGH)
      testFlag = LOW;
    else
      testFlag = HIGH;

    time = millis();    
  }
  previous = testButtonReading;
  processAccelerometer(event.acceleration.x,event.acceleration.y, event.acceleration.z); 
  //delay(10); //vary delay based on demands


  //hc05
     if (hc05Serial.available())
     {
        readHc05();
     }
  
}


void processAccelerometer(int16_t XReading, int16_t YReading, int16_t ZReading)
{
  if (testFlag == HIGH ){
    digitalWrite(ledPin, HIGH);
      if( XReading > leftThreshold ){
          digitalWrite(left, LOW); // RCtxBtn is the number of the digital pin
          pinMode(left, OUTPUT);  // Pull the signal low to activate button
          delay(100);  // Wait half a second
          pinMode(left, INPUT);  // Release the button.
          delay(100);  // Wait half a second
      }
      if( XReading < rightThreshold ){
          digitalWrite(right, LOW); // RCtxBtn is the number of the digital pin
          pinMode(right, OUTPUT);  // Pull the signal low to activate button
          delay(100);  // Wait half a second
          pinMode(right, INPUT);  // Release the button.
          delay(100);  // Wait half a second
      }
      if( YReading < upThreshold ){
          //Serial.println("go up");
          digitalWrite(up, LOW); // RCtxBtn is the number of the digital pin
          pinMode(up, OUTPUT);  // Pull the signal low to activate button
          delay(100);  // Wait half a second
          pinMode(up, INPUT);  // Release the button.
          delay(100);  // Wait half a second
      }
      if( YReading > downThreshold ){
          digitalWrite(down, LOW); // RCtxBtn is the number of the digital pin
          pinMode(down, OUTPUT);  // Pull the signal low to activate button
          delay(100);  // Wait half a second
          pinMode(down, INPUT);  // Release the button.
          delay(100);  // Wait half a second
      }
  }

  else{
    digitalWrite(ledPin, LOW);
    return;
  }
}

void readHc05(){
  BluetoothData=hc05Serial.read();
           if(BluetoothData=='1')
           {   
            // if number 1 pressed ....
            digitalWrite(bluePin,1);
            hc05Serial.println("LED  On D13 ON ! ");
           }
          if (BluetoothData=='0')
          {
            // if number 0 pressed ....
            digitalWrite(bluePin,0);
            hc05Serial.println("LED  On D13 Off ! ");
          }

        int x = hc05Serial.parseInt();
        String str = Serial.readStringUntil('\n');
        Serial.println(str);
        Serial.println(x);        
}



#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

//on/off button for accelerometer d-pad
/*int testFlag = HIGH;
int reading;
int previous = LOW;
long debounce = 200;
long time = 0; 
int testEnableButton = 11; //for turning the d-pad on, so you can test without losing control of the keyboard
int ledPin = A5;*/

//button press simulation
int RCtxBtn = A2;


void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(50);
}

void setup(void) 
{
 /* pinMode(testEnableButton, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);*/
  
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
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  /*reading = digitalRead(testEnableButton);
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (testFlag == HIGH)
      testFlag = LOW;
    else
      testFlag = HIGH;

    time = millis();    
  }
  previous = reading;*/
  processAccelerometer(event.acceleration.x,event.acceleration.y, event.acceleration.z); 
}


void processAccelerometer(int16_t XReading, int16_t YReading, int16_t ZReading)
{
  /*if (testFlag == HIGH ){
    digitalWrite(ledPin, HIGH);*/
      if( YReading > 4 ){
          Serial.println("go up");
          digitalWrite(RCtxBtn, LOW); // RCtxBtn is the number of the digital pin
          pinMode(RCtxBtn, OUTPUT);  // Pull the signal low to activate button
          delay(500);  // Wait half a second
          pinMode(RCtxBtn, INPUT);  // Release the button.
          delay(500);  // Wait half a second
         }
  //}

  else{
    //digitalWrite(ledPin, LOW);
    return;
  }
}



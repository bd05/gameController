#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define KEY_UP_ARROW 0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW 0xD7

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

int dPadEnable = 12; //for turning the d-pad on, so you can test without losing control of the keyboard
int ledPin = A5;
int zButton = 6;


void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
  Serial.print  ("100 ");        
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  Serial.print  ("2 "); 
  Serial.println(" g");  
}

void setup(void) 
{
  pinMode(dPadEnable, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(zButton, INPUT);  
  digitalWrite(zButton, HIGH);  
  
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

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_2_G);
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);

    if (digitalRead(zButton) == 0)  // if the button goes low
  {
    Keyboard.write('x');  // send a 'z' to the computer via Keyboard HID
    //delay(50);  // delay so there aren't a kajillion z's
  }
 
  /* Display the results (acceleration is measured in m/s^2) */
  /*Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");*/

  processAccelerometer(event.acceleration.x,event.acceleration.y, event.acceleration.z);  // Work with the read data
  
  //delay(10); //maybe take out the delay for real gameplay
}


void processAccelerometer(int16_t XReading, int16_t YReading, int16_t ZReading)
{
  if (digitalRead(dPadEnable) == 0){
    //Serial.print("d-pad mode on \n");
    digitalWrite(ledPin, HIGH);
      if( XReading > 4 ){
        Keyboard.write(KEY_LEFT_ARROW);
        //Serial.print("going left \n");
      }

      if( XReading < -5 ){
        Keyboard.write(KEY_RIGHT_ARROW);
        //Serial.print("going right \n");
      }

      if( YReading < -15 ){
        Keyboard.write(KEY_UP_ARROW);
        //Serial.print("going up \n");
      }

      if( YReading > -6 ){
        Keyboard.write(KEY_DOWN_ARROW);
        //Serial.print("going down \n");
      }
  }

  else{
    digitalWrite(ledPin, LOW);
  }

  
}



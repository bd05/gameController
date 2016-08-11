#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>

/* Assign a unique ID to the accelerometer sensor*/
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

//hc-05 bluetooth
int bluePin=13; // led on D13 will show blink on / off
int BluetoothData;
int upEasy = -13;
int upMed = -17;
int upHard = -19;
int downEasy = -8;
int downMed = -6;
int downHard = -4;
int leftEasy = 3.5;
int leftMed = 6;
int leftHard = 7;
int rightEasy = -4;
int rightMed = -10;
int rightHard = -14;

int upThreshold = upEasy;
int downThreshold = downEasy;
int leftThreshold = leftEasy;
int rightThreshold = rightEasy;

//node
char val;         // variable to receive data from the serial port
unsigned int timeout=0;
unsigned char state=0;


//button press simulation
int up = A2;
int down = 2;
int left = 3;
int right = 4;

// Timer2 service
ISR(TIMER2_OVF_vect) { 
  TCNT2 = 0;
  timeout++;
  if (timeout>61) {
    state=1;
    timeout=0;
  }
}
 
// initialize the timer 2 service
void init_timer2(void) {
  TCCR2A |= (1 << WGM21) | (1 << WGM20);   
  TCCR2B |= 0x07;                         // by clk/1024
  ASSR |= (0<<AS2);                       // Use internal clock - external clock not used in Arduino
  TIMSK2 |= 0x01;                         //Timer2 Overflow Interrupt Enable
  TCNT2 = 0;
  sei();   
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
}

void setup(void) 
{
  //hc-05
  pinMode(bluePin,OUTPUT);
  //hc05Serial.begin(9600);
  //hc05Serial.println("node Bluetooth");
  
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);
  Serial.println("arduino pro mini motion game controller"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    //Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  
  accel.setRange(ADXL345_RANGE_2_G);
  displaySensorDetails();

  // interrupt for reading from the hc-05 bluetooth connection 
  attachInterrupt(0, cleantime, FALLING);
  init_timer2();
}

void loop(void) 
{
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  /*Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");*/
  processAccelerometer(event.acceleration.x,event.acceleration.y, event.acceleration.z); 

  switch(state) {
    case 0:
      // no bt connection, do nothing
      break;
   
    case 1:
      // when there is a bt connection enter the control function
      control(); 
      break;
  }
  
}

void processAccelerometer(int16_t XReading, int16_t YReading, int16_t ZReading)
{
      if( XReading > leftThreshold ){
          digitalWrite(left, LOW); // RCtxBtn is the number of the digital pin
          pinMode(left, OUTPUT);  // Pull the signal low to activate button
          delay(100);  // Wait half a second
          pinMode(left, INPUT);  // Release the button.
          delay(100);  // Wait half a second
      }
      if( XReading < rightThreshold ){
          digitalWrite(right, LOW); 
          pinMode(right, OUTPUT);  
          delay(100);  
          pinMode(right, INPUT);  
          delay(100);  
      }
      if( YReading < upThreshold ){
          digitalWrite(up, LOW); // 
          pinMode(up, OUTPUT);  
          delay(100);  // 
          pinMode(up, INPUT);  
          delay(100);  
      }
      if( YReading > downThreshold ){
          digitalWrite(down, LOW); 
          pinMode(down, OUTPUT);  
          delay(100);  
          pinMode(down, INPUT);  
          delay(100);  
      }
}

// function for controlling the led
void control(void) {

    if (Serial.available()) {
      val = Serial.read();                  // read it and store it in 'val'
      Serial.println(val);
  }

  if (val == '0') {               //up easy
    Serial.println("up easy");             
    upThreshold = upEasy;
    //blink LED to show success
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);  
  }
  if (val == '1') {                     //up medium  
    Serial.println("up med");                                            
    upThreshold = upMed;
    //blink LED to show success
    digitalWrite(13, HIGH);   
    delay(1000);              
    digitalWrite(13, LOW);    
    delay(1000);      
  }
  if (val == '2') {                     //up hard    
    Serial.println("up med");                                              
    upThreshold = upHard;
    //blink LED to show success
    digitalWrite(13, HIGH);   
    delay(1000);              
    digitalWrite(13, LOW);    
    delay(1000);  
  }
  if (val == '3') {                     //down easy   
    Serial.println("down easy");                                   
    downThreshold = downEasy;
    //blink LED to show success
    digitalWrite(13, HIGH);  
    delay(1000);             
    digitalWrite(13, LOW);   
    delay(1000);  
  }
  if (val == '4') {                       // down medium 
    Serial.println("down med");                      
    downThreshold = downMed;
    //blink LED to show success
    digitalWrite(13, HIGH);  
    delay(1000);              
    digitalWrite(13, LOW);    
    delay(1000);  
  }
  if (val == '5') {                       // down hard    
    Serial.println("down hard");                        
    downThreshold = downHard;
    //blink LED to show success
    digitalWrite(13, HIGH);   
    delay(1000);             
    digitalWrite(13, LOW);    
    delay(1000);  
  }
  if (val == '6') {                       // left easy   
    Serial.println("left easy");           
    leftThreshold = leftEasy;
    //blink LED to show success
    digitalWrite(13, HIGH);  
    delay(1000);              
    digitalWrite(13, LOW);  
    delay(1000);  
  }
  if (val == '7') {                       // left medium     
    Serial.println("left med");                   
    leftThreshold = leftMed;
    //blink LED to show success
    digitalWrite(13, HIGH);   
    delay(1000);            
    digitalWrite(13, LOW);  
    delay(1000);  
  }
  if (val == '8') {                       // left hard   
    Serial.println("left hard");                  
    leftThreshold = leftHard;
    //blink LED to show success
    digitalWrite(13, HIGH);   
    delay(1000);            
    digitalWrite(13, LOW);  
    delay(1000);  
    
  }
    if (val == '9') {                       // right easy
    Serial.println("right easy");                         
    rightThreshold = rightEasy;
    //blink LED to show success
    digitalWrite(13, HIGH);   
    delay(1000);            
    digitalWrite(13, LOW);  
    delay(1000);  
  }
  if (val == '10') {                       // right medium 
    Serial.println("right med");                           
    rightThreshold = rightMed;
    //blink LED to show success
    digitalWrite(13, HIGH);   
    delay(1000);            
    digitalWrite(13, LOW);  
    delay(1000);  
  }
  if (val == '11') {                       // right hard   
    Serial.println("right hard");                   
    rightThreshold = rightHard;
    //blink LED to show success
    digitalWrite(13, HIGH);   
    delay(1000);            
    digitalWrite(13, LOW);  
    delay(1000);  
  }
  
  val = ' ';
    
  delay(100);                     // wait 100ms for next reading
}

void cleantime() {
  timeout=0;
  state=0;
}

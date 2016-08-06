#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>

/*#define rxPin 1
#define txPin 0
SoftwareSerial hc05Serial(rxPin, txPin); // RX, TX*/

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
          digitalWrite(right, LOW); // RCtxBtn is the number of the digital pin
          pinMode(right, OUTPUT);  // Pull the signal low to activate button
          delay(100);  // Wait half a second
          pinMode(right, INPUT);  // Release the button.
          delay(100);  // Wait half a second
      }
      if( YReading < upThreshold ){
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

// function for controlling the led
void control(void) {
  //if (hc05Serial.available()) {               // if data is available to read
    if (Serial.available()) {
    /*val = hc05Serial.read();                  // read it and store it in 'val'
    hc05Serial.println(val);*/
    val = Serial.read();                  // read it and store it in 'val'
    Serial.println(val);
  }

  if (val == '0') {               //up easy
    Serial.println("up easy");    
    digitalWrite(bluePin, LOW);           
    upThreshold = upEasy;
  }
  if (val == '1') {                     //up medium  
    Serial.println("up med");                                  
    digitalWrite(bluePin, HIGH);           
    upThreshold = upMed;
  }
  if (val == '2') {                     //up hard    
    Serial.println("up med");                                   
    digitalWrite(bluePin, LOW);           
    upThreshold = upHard;
  }
  if (val == '3') {                     //down easy   
    Serial.println("down easy");                          
    digitalWrite(bluePin, HIGH);          
    downThreshold = downEasy;
  }
  if (val == '4') {                       // down medium 
    Serial.println("down med");             
    digitalWrite(bluePin, LOW);           
    downThreshold = downMed;
  }
  if (val == '5') {                       // down hard    
    Serial.println("down hard");               
    digitalWrite(bluePin, HIGH);           
    downThreshold = downHard;
  }
  if (val == '6') {                       // left easy   
    Serial.println("left easy");    
    digitalWrite(bluePin, LOW);           
    leftThreshold = leftEasy;
  }
  if (val == '7') {                       // left medium     
    Serial.println("left med");              
    digitalWrite(bluePin, HIGH);         
    leftThreshold = leftMed;
  }
  if (val == '8') {                       // left hard   
    Serial.println("left hard");          
    digitalWrite(bluePin, LOW);          
    leftThreshold = leftHard;
  }
    if (val == '9') {                       // right easy
    Serial.println("right easy");               
    digitalWrite(bluePin, LOW);           
    rightThreshold = rightEasy;
  }
  if (val == '10') {                       // right medium 
    Serial.println("right med");                   
    digitalWrite(bluePin, HIGH);         
    rightThreshold = rightMed;
  }
  if (val == '11') {                       // right hard   
    Serial.println("right hard");           
    digitalWrite(bluePin, LOW);          
    rightThreshold = rightHard;
  }
  
  val = ' ';
    
  delay(100);                     // wait 100ms for next reading
}

void cleantime() {
  timeout=0;
  state=0;
}

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>

#define rxPin 8
#define txPin 7
SoftwareSerial hc05Serial(rxPin, txPin); // RX, TX

/* Assign a unique ID to the accelerometer sensor*/
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

//hc-05 bluetooth
int bluePin=13; // led on D13 will show blink on / off
int BluetoothData;
int upThreshold = -15;
int downThreshold = -8; //-6;
int leftThreshold = 4.5;
int rightThreshold = -5.5;
//node
char val;         // variable to receive data from the serial port
unsigned int timeout=0;
unsigned char state=0;


//button press simulation
int up = A2;
int down = 12;
int left = 10;
int right = 9;

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
  hc05Serial.begin(9600);
  hc05Serial.println("node Bluetooth");
  
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

// function for controlling the led
void control(void) {
  if (hc05Serial.available()) {               // if data is available to read
    //hc05Serial.println("got to control");
    //Serial.println("got to control normal serial monitor");
    val = hc05Serial.read();                  // read it and store it in 'val'
    hc05Serial.println(val);
  }

  if (val == '0') {               //up easy
    hc05Serial.println('-15');   // display the new value
    digitalWrite(bluePin, LOW);           
    upThreshold = -15;
  }
  if (val == '1') {                     //up medium                    
    hc05Serial.println('-17');                  
    digitalWrite(bluePin, HIGH);           
    upThreshold = -17;
  }
  if (val == '2') {                     //up hard                    
    hc05Serial.println('-19');                  
    digitalWrite(bluePin, LOW);           
    upThreshold = -19;
  }
  if (val == '3') {                     //down easy               
    hc05Serial.println('-19');                 
    digitalWrite(bluePin, HIGH);          
    downThreshold = -8;
  }
  if (val == '4') {                       // down medium
    hc05Serial.println('-19');                 
    digitalWrite(bluePin, LOW);           
    downThreshold = -6;
  }
  if (val == '5') {                       // down hard
    hc05Serial.println('-4');                  
    digitalWrite(bluePin, HIGH);           
    downThreshold = -4;
  }
  if (val == '6') {                       // left easy
    hc05Serial.println('5');              
    digitalWrite(bluePin, LOW);           
    leftThreshold = 4.5;
  }
  if (val == '7') {                       // left medium
    hc05Serial.println('7');                  
    digitalWrite(bluePin, HIGH);         
    leftThreshold = 6;
  }
  if (val == '8') {                       // left hard
    hc05Serial.println('9');              
    digitalWrite(bluePin, LOW);          
    leftThreshold = 7;
  }
    if (val == '9') {                       // right easy
    hc05Serial.println('-5.5');              
    digitalWrite(bluePin, LOW);           
    rightThreshold = -5.5;
  }
  if (val == '10') {                       // right medium
    hc05Serial.println('-7');                  
    digitalWrite(bluePin, HIGH);         
    rightThreshold = -10;
  }
  if (val == '11') {                       // right hard
    hc05Serial.println('-9');              
    digitalWrite(bluePin, LOW);          
    rightThreshold = -14;
  }
  
  val = ' ';
    
  delay(100);                     // wait 100ms for next reading
}

void cleantime() {
  timeout=0;
  state=0;
}

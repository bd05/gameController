// This code uses the SoftwareSerial library.
// It can be obtained here: http://arduino.cc/en/Reference/SoftwareSerial

#include <SoftwareSerial.h>

#define rxPin 8
#define txPin 7

SoftwareSerial mySerial(rxPin, txPin); // RX, TX


unsigned int timeout=0;
unsigned char state=0;
 
char val;         // variable to receive data from the serial port
int ledpin = 13;  // LED connected to pin 13

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

// sets up the program
void setup() {
  // open the serial port
  mySerial.begin(9600);
  mySerial.println("node bluetooth test");

   Serial.begin(9600);
   Serial.println("normal serial monitor");

  // bind the ledpin as output
  pinMode(ledpin, OUTPUT);

  // bind pin 2 as input
  pinMode(2,INPUT);
 
  // interrupt for reading from the bluetooth connection 
  attachInterrupt(0, cleantime, FALLING);
  init_timer2();
}

// function for controlling the led
void control(void) {
  if (mySerial.available()) {               // if data is available to read
    mySerial.println("got to control");
    Serial.println("got to control normal serial monitor");
    val = mySerial.read();                  // read it and store it in 'val'
    mySerial.println(val);
  }

  if (val == '1') {                       // if '1' was received
    mySerial.println('1');                  // display the new value
    digitalWrite(ledpin, HIGH);           // turn ON the LED
  } else if (val == '0') { 
    mySerial.println('0');                  // display the new value
    digitalWrite(ledpin, LOW);            // otherwise turn it OFF
  } else if (val == 's') {                // if 's' is received display the current status of the led
    if (digitalRead(ledpin) == HIGH) {
      mySerial.println('1');
    } else {
      mySerial.println('0');
    } 
  }
  
  val = ' ';
    
  delay(100);                             // wait 100ms for next reading
}

// control loop for the program
void loop() {
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
 
void cleantime() {
  timeout=0;
  state=0;
}

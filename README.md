# gameController
motion controlled, wireless game controller
You must install the Adafruit sensor library and the Adafruit ADXL345 library to run code that uses the ADXL accelerometer.
Put both in the Documents/Arduino/libraries folder, make sure they do not have '-' 's in the name, and make sure they are not nested directories.

## Description of files:

# adxl345_dpad.ino:
This code is for the Adafruit Bluefruit 32u4 Feather. Decided to abandon this when I found out that BLE is not received well on MAC OSX and Windows 8.
For directory of keyboard hex codes:
https://www.arduino.cc/en/Reference/KeyboardModifiers

This code could probably also be used for Arduino Micro or Leonardo, or another microcontroller with HID.

## nodeSerial folder:
The web client for choosing difficulty level for each direction. Works with hc05VarData.ino or nodeTest.ino loaded onto the Arduino Uno, gameControllerHc05Mini.ino on the Arduino Pro Mini.

## ezKey folder:

hc05Test.ino:
for testing Bluetooth communication between the Arduino and HC-05 Bluetooth. Open the Arduino serial monitor and the Bluetooth serial monitor
on Tera Term to see bidirectional communication.

#hc05Data.ino:
Used for testing Bluetooth input and communication. Turn LED on and off by typing in 1 or 0 into Bluetooth serial port.

# nodeTest.ino:
Test whether or not the HC-05 Bluetooth module is receiving data from the web client. Web client would most likely send data through a web socket to (socket.io in all of my examples) Node.js server,
then Node.js server should send this to the Bluetooth virtual serial port. Arduino reads from the serial port using val = <name_of_your_BT_port>.read();,
then turns LED on or off depending on if val = 1 or val = 0.

# buttonSimADXL345_2.ino:
Arduino mini connected to an Adafruit Bluefruit EZ-Key. The EZ-Key is supposed to execute key presses on your computer's keyboard if buttons attached to its pins are pressed.
The Arduino mini simulates button presses by pulling pins from low to high. Diodes are used to prevent frying the pin. Uses an ADXL345 accelerometer as a tilt sensor.
The D-pad of the keyboard is controlled by using a combination of the EZ-Key, button press simulations from the Arduino mini pins, and the accelerometer.
Tilt the accelerometer forward > up key
Tilt accelerometer backward > down key
Tilt accelerometer downwards to the left > left key
Tilt accelerometer downwards to the right > right key

Test button to turn on and off this D-pad control. (LED will light when enabled.)

# hc05VarData.ino:
Cryptically named because I was using this as a test, then it ended up as the main code. Is a functioning D-pad by using the same methods
as buttonSimADXL345_2.ino, except you can adjust difficulty level with the web client in the nodeSerial folder.

# gameControllerHc05Mini.ino
hc05VarData.ino except on the Arduino pro mini. Works except the bluetooth serial monitor/windows console goes crazy and keeps receiving and printing garbage data.
Full functionality of hc05VarData.ino works though. Works wirelessly. Currently using Serial instead of declaring another serial monitor with SoftwareSerial and non-default Tx and Rx pins (not 0 and 1).
Seems to println more nicely.

Tested on Arduino only so far. Will test on the Arduino mini shortly. In principle, should also work.

Used the node-bluetooth-serial-port library from eelcocramer:
https://github.com/eelcocramer/node-bluetooth-serial-port

Note to self: when using the above library, the console for Node (aka your command line window) BECOMES the Bluetooth serial monitor as well.
The Node console and Bluetooth Serial port will SHARE the command line window as a console/serial monitor. You CANNOT run Tera Term
as the Bluetooth Serial monitor at the same time, or all of the serial data being printed will go to Tera Term and NOT to the serial
port shared with the Node.js Server.

Using this library I:
1. Have user put input into web client
2. Send this user input as data through a web socket (Socket.io)from the client to the server
3. Data is received by Node.js server
4. Data is sent from server to Bluetooth virtual serial port
5. Arduino reads data from Bluetooth virtual serial port using val = <name_of_your_BT_port>.read();
6. Use C code to make Arduino do something with that data (in this case, it reassigns difficulty levels for each tilt direction for the game controller D-pad)

# testLCDButton.ino
Interactive OLED LCD menu. Used it to test that the LCD menu was working.

# gameControllerMiniLCD.ino
What the soldered perfboard circuit is currently using (as of August 15, 2016). D-pad works with ADXL345 accelerometer and EZ-key, Return button works with
microswitch and EZ-key. You can set the difficulty level for Up, Down, and Left/Right using an OLED LCD screen. The screen is controlled by four buttons:
up, down, select, back.

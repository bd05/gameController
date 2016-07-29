// var obd = require('obd');
// obd.connect();

var BluetoothSerialPort = require("bluetooth-serial-port").BluetoothSerialPort;
var myBtPort = new BluetoothSerialPort();
var dataBuffer = "";

myBtPort.on('found', function (address, name) {
    console.log('Found: ' + address + ' with name ' + name);

    if(name != "ubuntu-0" && name != "OBDII") { return; }

    myBtPort.findSerialPortChannel(address, function(channel) {
        console.log('Found RFCOMM channel for serial port on ' + name + ': ' + channel);
        console.log('Attempting to connect...');

        myBtPort.connect(address, channel, function() {
            console.log('Connected. Receiving data...');
            myBtPort.on('data', function(buffer) {

            });
        });
    });
});

myBtPort.on('finished', function() {
    console.log('scan did finish');
});

myBtPort.inquire();
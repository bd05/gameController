// var obd = require('obd');
// obd.connect();

var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);

server.listen(8080);
 console.log('Magic happens on port 8080.');

app.use(express.static('/'));

app.get('/', function (req, res) {
  res.sendFile(__dirname + '/index.html');
});

var testData;

io.on('connection', function (socket) {
  socket.emit('news', { hello: 'world' });
  socket.on('my other event', function (data) {
    testData = data;
    console.log("data to send: " + data);
  });
});

//bluetooth
var BluetoothSerialPort = require("bluetooth-serial-port").BluetoothSerialPort;
var myBtPort = new BluetoothSerialPort();
var dataBuffer = "";

myBtPort.on('found', function (address, name) {
    console.log('Found: ' + address + ' with name ' + name);

    //if(name != "ubuntu-0" && name != "OBDII") { return; }

    myBtPort.findSerialPortChannel(address, function(channel) {
        console.log('Found RFCOMM channel for serial port on ' + name + ': ' + channel);
        console.log('Attempting to connect...');

        myBtPort.connect(address, channel, function() {
            console.log('Connected. Receiving data...');
            /*myBtPort.on('data', function(buffer) {

            });*/
            //from LED example
            process.stdin.resume();
            process.stdin.setEncoding('utf8');
            console.log('Press "1" or "0" and "ENTER" to turn on or off the light.')

            process.stdin.on('data', function (data) {
                myBtPort.write(data);
            });

            myBtPort.on('data', function(data) {
                console.log('Received: ' + data);
            });


        });
    });
});

myBtPort.on('finished', function() {
    console.log('scan did finish');
});

myBtPort.inquire();
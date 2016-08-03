
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

var diffSetting;

//bluetooth
var BluetoothSerialPort = require("bluetooth-serial-port").BluetoothSerialPort;
var myBtPort = new BluetoothSerialPort();
var dataBuffer = "";

myBtPort.on('found', function (address, name) {
    console.log('Found: ' + address + ' with name ' + name);

    myBtPort.findSerialPortChannel(address, function(channel) {
        console.log('Found RFCOMM channel for serial port on ' + name + ': ' + channel);
        console.log('Attempting to connect...');

        myBtPort.connect(address, channel, function() {
            console.log('Connected. Receiving data...');

            /*myBtPort.write(new Buffer('1', 'utf-8'), function(err, count) {
                if (err) console.log('Cannot conect');
            });*/

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


io.on('connection', function (socket) {
    socket.emit('news', { hello: 'world' });
        socket.on('selected difficulty', function (data) {
            console.log("difficulty selected: " + data);
            diffSetting = data;
            
            myBtPort.write(new Buffer(diffSetting, 'utf-8'), function(err, count) {
                if (err) console.log('Cannot conect');
            });
        });
});
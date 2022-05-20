/*
 * Authors: Samuel Moore, Taj Jones
 * 
 * Code for our Digital Electronics Semester 2 final.
 * 
 * This code takes inputs from a joystick and push button, and uses those inputs 
 * to determine the value of character variables.
 * 
 * These variables are then sent to the Robot inside of a two character string.
 * 
 * The first character directs the robot which direction to drive in.
 * 
 * The second character tells to robot to turn the LED on/off
 * 
 * 
 */

//include libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>



// pins

// radio pins
const int CE_PIN = 9;
const int CSN_PIN = 10;

// input pins
const int buttonPin = 4;
const int xPin = A0;
const int yPin = A1;

//===========
// options

int deadzone = 3;


//===========

// address to recieve data from (This must match the address of the controller.)
const byte slaveAddress[5] = {'R','x','A','A','A'};

// communications data
// character 1 = move command
// character 2 = button command
char dataToSend[3] = "00";

// command variables

// controls driving
char txNum1 = '0';

// controls LED
char txNum2 = '0';


// input joystick position variables
int xPos;
int yPos;

// radio object
RF24 radio(CE_PIN, CSN_PIN);

void setup() {

    // start serial
    Serial.begin(9600);

    // start radio & set options
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count

    // set as transmitter
    radio.openWritingPipe(slaveAddress);

    // setup push button input
    pinMode(buttonPin, INPUT);
    
}

//====================

void loop() {

    // send data to robot
    send();

}

//====================

void send() {

    // get drive command
    GetDriveData();

    // get LED command
    GetButtonData();

    // put commands in data string
    dataToSend[0] = txNum1;
    dataToSend[1] = txNum2;

    // send data string to robot
    radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

}

//================

void GetDriveData() {

    // get joystick inputs
    int xPos = map(analogRead(xPin),0, 1023, 0, 255);
    int yPos = map(analogRead(yPin),0, 1023, 0, 255);

    // center positions for the joystick
    int xCenter = 162;
    int yCenter = 128;

    // set drive command forward
    if (yPos <= yCenter - deadzone) {
      txNum1 = '1';
    }
    
    // set drive command backwards
    else if (yPos >= yCenter + deadzone) {
      txNum1 = '2';
    }
    
    // set drive command left
    else if (xPos <= xCenter - 50) {
      txNum1 = '3';
    }
    
    // set drive command right
    else if (xPos >= xCenter + deadzone) {
      txNum1 = '4';
    }

    // set drive command stop
    else {
      txNum1 = '0';
    }
    
}



void GetButtonData() {

  // read input from push button, and assign value to LED command
  if (digitalRead(buttonPin) == HIGH) {

    txNum2 = '1';
    
  } else {
    
    txNum2 = '0';
  }
  
}

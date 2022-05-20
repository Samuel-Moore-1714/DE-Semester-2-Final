/*
 * Authors: Samuel Moore, Taj Jones
 * 
 * Code for our Digital Electronics Semester 2 final.
 * 
 * This code takes and input string from the controller and assignes 
 * characters within the string to commands.
 * 
 * The code then uses these commands to execute functions on the robot, 
 * such as driving, or activating the LED light
 * 
 * 
 */


// include libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>


// options

// slows the forward/backwards speed to allow the motors to run with less power
int speedBuffer = 150;

// slows the left/right speed to allow the motots to run with less power
int turnBuffer = 350;

// pins

// radio pins
const int CE_PIN = 9;
const int CSN_PIN = 10;

// motor controller pins
const int LEFT_REAR_MOTOR_PIN = 6;
const int LEFT_FRONT_MOTOR_PIN = 3;
const int RIGHT_REAR_MOTOR_PIN = 5;
const int RIGHT_FRONT_MOTOR_PIN = 4;

const int LED_PIN = 39;

// address to recieve data from (This must match the address of the controller.)
const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

// communications data
// character 1 = move command
// character 2 = button command
// this must match dataToSend in the TX
char dataReceived[3];

// command for driving
char moveData = '0';

// command for LED
char buttonData = '1';

// radio object
RF24 radio(CE_PIN, CSN_PIN);

// servo objects
Servo leftRearMotor;
Servo leftFrontMotor;
Servo rightRearMotor;
Servo rightFrontMotor;

//===========

void setup() {
  
    // start serial
    Serial.begin(9600);

    // start radio & set options
    radio.begin();
    radio.setDataRate( RF24_250KBPS );

    // set as reciever
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();

    // setup motor controller pins
    leftRearMotor.attach(LEFT_REAR_MOTOR_PIN);
    leftFrontMotor.attach(LEFT_FRONT_MOTOR_PIN);
    rightRearMotor.attach(RIGHT_REAR_MOTOR_PIN);
    rightFrontMotor.attach(RIGHT_FRONT_MOTOR_PIN);

    // setup LED pin
    pinMode(LED_PIN, OUTPUT);
    
}

//=============

void loop() {

    // Get information from controller
    GetData();

}

//==============

void GetData() {
  
    if ( radio.available() ) {

        // read from controller
        radio.read( &dataReceived, sizeof(dataReceived) );

        // set character 1 as drive command
        moveData = dataReceived[0];

        // set character 2 as LED command
        buttonData = dataReceived[1];

        // drive robot using drive command
        Drive(moveData);

        // run LED using LED command
        Light(buttonData);


    }
}


void Drive(char command) {


  // drive forward
  if (command == '1') {

    leftRearMotor.writeMicroseconds(2000 - speedBuffer);
    leftFrontMotor.writeMicroseconds(2000 - speedBuffer);

    rightRearMotor.writeMicroseconds(1000 + speedBuffer);
    rightFrontMotor.writeMicroseconds(1000 + speedBuffer);


  // drive backwards
  } else if (command == '2') {

    leftRearMotor.writeMicroseconds(1000 + speedBuffer);
    leftFrontMotor.writeMicroseconds(1000 + speedBuffer);

    rightRearMotor.writeMicroseconds(2000 - speedBuffer);
    rightFrontMotor.writeMicroseconds(2000 - speedBuffer);


  // turn left
  } else if (command == '3') {

    // left
    
    leftRearMotor.writeMicroseconds(1000 + turnBuffer);
    leftFrontMotor.writeMicroseconds(1000 + turnBuffer);

    rightRearMotor.writeMicroseconds(1000 + turnBuffer);
    rightFrontMotor.writeMicroseconds(1000 + turnBuffer);


  // turn right
  } else if (command == '4') {

    // right
    
    leftRearMotor.writeMicroseconds(2000 - turnBuffer);
    leftFrontMotor.writeMicroseconds(2000 - speedBuffer);

    rightRearMotor.writeMicroseconds(2000 - turnBuffer);
    rightFrontMotor.writeMicroseconds(2000 - turnBuffer);


  // stop
  } else if (command == '0') {

    //stop
    
    leftRearMotor.writeMicroseconds(1500);
    leftFrontMotor.writeMicroseconds(1500);
    rightRearMotor.writeMicroseconds(1500);
    rightFrontMotor.writeMicroseconds(1500);
    
  }
}


void Light(char command) {
  
  // turn LED on
  if (command == '1') {

    digitalWrite(LED_PIN, HIGH);

  // turn LED off
  } else if (command == '0') {

    digitalWrite(LED_PIN, LOW);
  }

  
}

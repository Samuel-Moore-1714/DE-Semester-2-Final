// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// options

int speedBuffer = 100;
int turnBuffer = 300;

// pins

const int CE_PIN = 9;
const int CSN_PIN = 10;

const int LEFT_REAR_MOTOR_PIN = 6;
const int LEFT_FRONT_MOTOR_PIN = 3;
const int RIGHT_REAR_MOTOR_PIN = 5;
const int RIGHT_FRONT_MOTOR_PIN = 4;

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

char dataReceived[3]; // this must match dataToSend in the TX

char moveData = '0';

RF24 radio(CE_PIN, CSN_PIN);

Servo leftRearMotor;
Servo leftFrontMotor;
Servo rightRearMotor;
Servo rightFrontMotor;

//===========

void setup() {

    Serial.begin(9600);

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();

    leftRearMotor.attach(LEFT_REAR_MOTOR_PIN);
    leftFrontMotor.attach(LEFT_FRONT_MOTOR_PIN);
    rightRearMotor.attach(RIGHT_REAR_MOTOR_PIN);
    rightFrontMotor.attach(RIGHT_FRONT_MOTOR_PIN);
    
}

//=============

void loop() {
  
    GetData();

}

//==============

void GetData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );

        moveData = dataReceived[0];

        Drive(moveData);
        
        Serial.println(moveData);

    }
}

void Drive(char command) {
  
  if (command == '1') {

    // forward
    leftRearMotor.writeMicroseconds(2000 - speedBuffer);
    leftFrontMotor.writeMicroseconds(2000 - speedBuffer);

    rightRearMotor.writeMicroseconds(1000 + speedBuffer);
    rightFrontMotor.writeMicroseconds(1000 + speedBuffer);
    
  } else if (command == '2') {

    // backwards
    leftRearMotor.writeMicroseconds(1000 + speedBuffer);
    leftFrontMotor.writeMicroseconds(1000 + speedBuffer);

    rightRearMotor.writeMicroseconds(2000 - speedBuffer);
    rightFrontMotor.writeMicroseconds(2000 - speedBuffer);
    
  } else if (command == '3') {

    // left
    
    leftRearMotor.writeMicroseconds(1000 + turnBuffer);
    leftFrontMotor.writeMicroseconds(1000 + turnBuffer);

    rightRearMotor.writeMicroseconds(1000 + turnBuffer);
    rightFrontMotor.writeMicroseconds(1000 + turnBuffer);
    
  } else if (command == '4') {

    // right
    
    leftRearMotor.writeMicroseconds(2000 - turnBuffer);
    leftFrontMotor.writeMicroseconds(2000 - speedBuffer);

    rightRearMotor.writeMicroseconds(2000 - turnBuffer);
    rightFrontMotor.writeMicroseconds(2000 - turnBuffer);
    
  } else if (command == '0') {

    //stop
    
    leftRearMotor.writeMicroseconds(1500);
    leftFrontMotor.writeMicroseconds(1500);
    rightRearMotor.writeMicroseconds(1500);
    rightFrontMotor.writeMicroseconds(1500);
    
  }
}

// SimpleRx - the slave or the receiver

// libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// constants
const int CE_PIN = 9;
const int CSN_PIN = 10;

const int LEFT_REAR_EN = 4;
const int LEFT_REAR_IN1 = 26;
const int LEFT_REAR_IN2 = 28;

const int LEFT_FRONT_EN = 5;
const int LEFT_FRONT_IN1 = 30;
const int LEFT_FRONT_IN2 = 32;

const int RIGHT_REAR_EN = 6;
const int RIGHT_REAR_IN1 = 40;
const int RIGHT_REAR_IN2 = 42;

const int RIGHT_FRONT_EN = 7;
const int RIGHT_FRONT_IN1 = 44;
const int RIGHT_FRONT_IN2 = 46;


int driveDirection;
// 1 = forward
// 2 = reverse
// 0 = stop



//==============

int driveSpeed = 250;


const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[2]; // this must match dataToSend in the TX

int moveDataInt = 0;

//===========

void setup() {


    Serial.begin(9600);

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();

    pinMode(LEFT_REAR_IN1, OUTPUT);
    pinMode(LEFT_REAR_IN2, OUTPUT);

    pinMode(LEFT_FRONT_IN1, OUTPUT);
    pinMode(LEFT_FRONT_IN2, OUTPUT);

    pinMode(RIGHT_REAR_IN1, OUTPUT);
    pinMode(RIGHT_REAR_IN2, OUTPUT);

    pinMode(RIGHT_FRONT_IN1, OUTPUT);
    pinMode(RIGHT_FRONT_IN2, OUTPUT);
    
}

//=============

void loop() {

  run();
    
}



void run() {
  
  getData();
  //Drive(moveDataInt);
  

}

//==============

void getData() {
  
    if (radio.available() ) {
      
      radio.read(&dataReceived, sizeof(dataReceived));


      // speed
      char moveData[] =  {dataReceived[0]};
      
      moveDataInt = atoi(moveData);

      Drive(moveDataInt);
      
      Serial.println(moveDataInt);
    
      
    }
    
}


void Drive(int driveCommand) {

  if (driveCommand == 1) {
    
    // forward
    driveForward(driveSpeed);
    
  } else if (driveCommand == 2) {

    // backwards
    
  } else if (driveCommand == 3) {

    // left
    
  } else if (driveCommand == 4) {

    //right
    
  } else if (driveCommand == 0) {

    //stop
    
  }
}

void driveForward(int speed) {

  digitalWrite(LEFT_REAR_IN1, HIGH);
  digitalWrite(LEFT_REAR_IN2, LOW);

  analogWrite(LEFT_REAR_EN, speed);
  
}

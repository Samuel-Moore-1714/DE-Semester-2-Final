// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   9
#define CSN_PIN 10

int deadzone = 3;

const int xPin = A0;
const int yPin = A1;

const byte slaveAddress[5] = {'R','x','A','A','A'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

char dataToSend[2] = "00";

char txNum1 = '0';
char txNum2 = '0';


int xPos;
int yPos;


void setup() {

    Serial.begin(9600);

    radio.begin();
    
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
}

//====================

void loop() {
    
    send();

}

//====================

void send() {

    getData();
    
    radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.println(dataToSend);

}

//================

void getData() {


    int xPos = map(analogRead(xPin),0, 1023, 0, 255);
    int yPos = map(analogRead(yPin),0, 1023, 0, 255);

    int xCenter = 162;
    int yCenter = 128;

    //forward
    if (yPos <= yCenter - deadzone) {
      txNum1 = '1';
    }
    
    //backwards
    else if (yPos >= yCenter + deadzone) {
      txNum1 = '2';
    }
    
    //left
    else if (xPos <= xCenter - 50) {
      txNum1 = '3';
    }
    
    //right
    else if (xPos >= xCenter + deadzone) {
      txNum1 = '4';
    }

    //stop
    else {
      txNum1 = '0';
    }

    dataToSend[0] = txNum1;
    dataToSend[1] = txNum2;
    
    //Serial.println(txNum1);
    
}

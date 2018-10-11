 /*-----( Import needed libraries )-----*/
#include <SPI.h>   // Comes with Arduino IDE
#include "RF24.h"  // Download and Install (See above)
#include "nRF24L01.h"
#include "printf.h"

/*-----( Declare Constants and Pin Numbers )-----*/
int xAxis = A3, yAxis = A1;

/*-----( Declare objects )-----*/
// (Create an instance of a radio, specifying the CE and CS pins. )
RF24 myRadio(7,9); // "myRadio" is the identifier you will use in following methods

/*-----( Declare Variables )-----*/

byte add[][6] = {"Node1","Node2"}; // Create address for 1 pipe.
int joystick[4]; // 4 element array holding Joystick reading and two buttons

int buttonUp    = 2;
int buttonDown  = 3;

/*------setup file for arduino-------*/

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(56700);//set the communication baud rate
  Serial.print("\n RF24/Simple Transmit mode");  
   printf_begin();
    pinMode(buttonUp,INPUT);//declaring the input pin for movement
    pinMode(buttonDown,INPUT);//declaring the input pin for the direction
  //Declaration for my radio receiver
  
  myRadio.begin();  // Start up the physical nRF24L01 Radio
  myRadio.setChannel(108);  // Above most Wifi Channels
  //myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setPALevel(RF24_PA_MAX);  // Uncomment for more power
  myRadio.setAutoAck(1);                    // Ensure autoACK is enabled
  myRadio.enableAckPayload();               // Allow optional ack payloads
  myRadio.setRetries(0,15);                // Here we are sending 1-byte payloads to test the call-response speed
  myRadio.setPayloadSize(5);                // Here we are sending 1-byte payloads to test the call-response speed
  myRadio.openWritingPipe(add[0]);          // Both radios listen on the same pipes by default, and switch when writing
  myRadio.openReadingPipe(1,add[1]);
 // myRadio.startListening();                 // Start listening  
   myRadio.stopListening();
}

//--(end setup )---

/*------loop function of arduino------*/
 void loop()
{ 

    unsigned long time=micros();
    
/*--------taking the sensor value from joystick -------*/

    joystick[0] = analogRead(xAxis);
    joystick[1] = analogRead(yAxis);
    joystick[2] = digitalRead(buttonUp);
    joystick[3] = digitalRead(buttonDown);

    myRadio.write(joystick, sizeof(joystick) );
    delay(20);
    
      Serial.print("X = ");
      Serial.print(analogRead(xAxis));
      Serial.print(" Y = ");  
      Serial.print(analogRead(yAxis));
      
      }
     


/*
 
 */

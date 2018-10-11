 /*-----( Import needed libraries )-----*/
#include <SPI.h>   // Comes with Arduino IDE
#include "RF24.h"  // Download and Install (See above)
#include "nRF24L01.h"
#include "printf.h"

/*-----( Declare Constants and Pin Numbers )-----*/
int mov=A3,dir=A1;//pin for the fwd,back,left and right
int fwd1=523,back1=523,left1=508,right1=508;//default value for over the radio 
int joy1=523,joy2=508;//default value for theh joystick on start
int joy11,joy22;//value return from the receiver
int test=2;
byte counter = 1;

/*-----( Declare objects )-----*/
// (Create an instance of a radio, specifying the CE and CS pins. )
RF24 myRadio(7,9); // "myRadio" is the identifier you will use in following methods
/*-----( Declare Variables )-----*/
byte add[][6] = {"Node1","Node2"}; // Create address for 1 pipe.
int data=0;  // Data that will be received from the transmitter
/*--------declaring the changing value the movement over the radio--------*/


//blink test program
void bl()
{
  
  digitalWrite(test, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(test, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}


/*--------taking the sensor value from joystick -------*/
//fuction for the forward and backward movement of cleaner

int movem()
{
  joy1=analogRead(mov);
  joy1=joy1-1025;
  
  printf(" joy1 value : %d",joy1);

 if(!myRadio.write(joy1,sizeof(joy1)))
  {
 
   return true;
  }
 else
   {
    return false;
   }
}

//function for the left and right movement tof the robot

int direc()
{
  joy2=analogRead(dir);
  joy2=joy2+1;
   printf("  joy2 value : %d /n ",joy2);
  
 if(!myRadio.write(joy2,sizeof(joy2)))
   {
      return true ;
   }
 else
   {
    return false;
   }
}

/*------setup file for arduino-------*/

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(56700);//set the communication baud rate
  Serial.print("\n RF24/Simple Transmit mode");  
   printf_begin();
    pinMode(test,OUTPUT); 
    pinMode(mov,INPUT);//declaring the input pin for movement
    pinMode(dir,INPUT);//declaring the input pin for the direction
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
}

//--(end setup )---

/*------loop function of arduino------*/
 void loop()
{ 
    myRadio.stopListening();//again go to transmit mode
   //printf("Now sendind %d as payload",dir);
    
    unsigned long time=micros();
    if(!movem()&& !direc())
     {
      Serial.println(F("failed"));
     }
     if(!myRadio.available())
        {
          Serial.println(F("\n Blank payload at ack"));
        }
      else
      {
        while(myRadio.available())
        {
         //Serial.println(F("\n Exit tramit mode"));
          unsigned long tim=micros();
          myRadio.read(&joy11 ,1);
         // myRadio.read(&joy22 ,1);
          printf("Got response mov: %d, direction: %d round-trip delay: %lu microseconds\n\r",joy11,joy22,tim-time);
           delay(300); 
        }
      
      }
     
}

/*
 
 */

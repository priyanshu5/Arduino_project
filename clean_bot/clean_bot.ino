 /*-----( Import needed libraries )-----*/
#include <SPI.h>   // Comes with Arduino IDE
#include "RF24.h"  // Download and Install (See above)
#include "nRF24L01.h"
#include "printf.h"

/*-----( Declare Constants and Pin Numbers )-----*/
int ana0=A0,ana1=A1,ana2=A2,ana3=A3,ana4=A4,ana5=A5,ana6=A6,ana7=A7;
int dig2=2, dig3=3, dig4=4, dig5=5, dig6=6;
int test=2;
int intensity=140,lite=130;
int joy1=0, joy2=0;

/*-----( Declare objects )-----*/
// (Create an instance of a radio, specifying the CE and CS pins. )
RF24 myRadio (9, 10); // "myRadio" is the identifier you will use in following methods
/*-----( Declare Variables )-----*/
byte add[][6] = {"Node1","Node2"}; // Create address for 1 pipe.
int data =0;

/*-----for testing the program-------*/

void bl()
{
  
  digitalWrite(test, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(test, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}


/*--------declare any function--------*/


//here the code foe running the vechicle start assuming ideal condition 
//to move the robot in fordware direction
void fwd()

{
  analogWrite(ana0,intensity);
  analogWrite(ana1,LOW);
  analogWrite(ana2,intensity);
  analogWrite(ana3,LOW);
  analogWrite(ana4,intensity);
  analogWrite(ana5,LOW);
  analogWrite(ana6,intensity);
  analogWrite(ana7,LOW);
  delay(400);
}
//to move the robot in backward direction
void back()

{
  analogWrite(ana0,LOW);
  analogWrite(ana1,intensity);
  analogWrite(ana2,LOW);
  analogWrite(ana3,intensity);
  analogWrite(ana4,LOW);
  analogWrite(ana5,intensity);
  analogWrite(ana6,LOW);
  analogWrite(ana7,intensity);  
  delay(400);
}
//to move the robot in left direction
void left()
{
  analogWrite(ana0,LOW);
  analogWrite(ana1,lite);
  analogWrite(ana2,LOW);
  analogWrite(ana3,lite);
  analogWrite(ana4,lite);
  analogWrite(ana5,LOW);
  analogWrite(ana6,lite);
  analogWrite(ana7,LOW); 
  delay(100); 
}
//to move the robot in right direction
void right()
{
  analogWrite(ana0,lite);
  analogWrite(ana1,LOW);
  analogWrite(ana2,lite);
  analogWrite(ana3,LOW);
  analogWrite(ana4,LOW);
  analogWrite(ana5,lite);
  analogWrite(ana6,LOW);
  delay(100);
  analogWrite(ana7,lite);
}
//to stop the robot  
void stp()
{
  analogWrite(ana0,LOW);
  analogWrite(ana1,LOW);
  analogWrite(ana2,LOW);
  analogWrite(ana3,LOW);
  analogWrite(ana4,LOW);
  analogWrite(ana5,LOW);
  analogWrite(ana6,LOW);
  analogWrite(ana7,LOW);  
}


//function to be called for rx execution command
int exe(int data)
{
//  if (data <= 0)
//  {
    joy2 = data;
    joy1 = data;
    if (joy1<= -10 && joy1>= -645)
     {
     // intensity=int(((645+joy1)/635)*1023);
      fwd();
      Serial.println("forward");
      }
    else if(joy1 >= 29256 && joy1<= 31743)
    {
      //intensity=int((((31743-joy1)/2487)*1023));
      back();
      Serial.println("backward");
    }

    
    
    else if(joy2 > 10 && joy2 <= 300)
     {
    //  lite=(((300-joy2)/290)*1023);
      right();
      Serial.println("right");
     }
    else if(joy2 >= 31760 && joy2 <=32542)
     {
      //lite=(((32542-joy2)/782)*1023);
      left();
      Serial.println("left");
     }
   
  else
    {
      stp();
    }
}



/*------setup file for arduino-------*/

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(57600);//set the communication baud rate
  Serial.print("\n RF24/Simple Receive mode");
   printf_begin();
  //pin declaration
  pinMode(ana0,OUTPUT);//left motor pin connection
  pinMode(ana1,OUTPUT);//left motor pin connection
  pinMode(ana2,OUTPUT);//right motor pin connection
  pinMode(ana3,OUTPUT);//right motor pin connection
  
  pinMode(ana4,OUTPUT);//left motor pin connection
  pinMode(ana5,OUTPUT);//left motor pin connection
  pinMode(ana6,OUTPUT);//right motor pin connection
  pinMode(ana7,OUTPUT);//right motor pin connection
   
  pinMode(dig2,OUTPUT);//left motor enableA pin connection
  pinMode(dig3,OUTPUT);//left motor pin connection
  pinMode(dig4,OUTPUT);//right motor pin connection
  pinMode(dig5,OUTPUT);//right motor pin connection
   
   
  

  
  //Declaration for my radio receiver
  
  myRadio.begin();  // Start up the physical nRF24L01 Radio
  myRadio.setChannel(108);  // Above most Wifi Channels
  //myRadio.setPALevel(RF24_PA_MIN);
   myRadio.setPALevel(RF24_PA_MAX);  // Uncomment for more power
  myRadio.setAutoAck(1);                    // Ensure autoACK is enabled
  myRadio.enableAckPayload();               // Allow optional ack payloads
  myRadio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  myRadio.setPayloadSize(5);                // Here we are sending 1-byte payloads to test the call-response speed
  myRadio.openWritingPipe(add[1]);          // Both radios listen on the same pipes by default, and switch when writing
  myRadio.openReadingPipe(1,add[0]);
  myRadio.startListening();                 // Start listening  
}

//--(end setup )---

 /****** LOOP: RUNS CONSTANTLY ******/
void loop()  
{
  
  byte pipe;
  int got;  // Data that will be received from the transmitter
    
    while( myRadio.available(&pipe))  // While there is data ready
  {
    myRadio.read( &data, sizeof(data));
    exe(data);
    
    //Serial.println(data);
    Serial.print(" data recived = ");   
    printf("recieved %d \n",data);
    
    myRadio.writeAckPayload(pipe,&data, 1 );
    
  }   
}



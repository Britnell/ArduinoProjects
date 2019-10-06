#include <SoftwareSerial.h>


SoftwareSerial mySerial(8, 9); // RX, TX

#define DONT__ASK_FOR_BAUDRATE      1

String command = ""; 
int keyPin = 4;
uint8_t keyVal;

void setup() 
{
   // Init
   Serial.begin(38400);

   if(DONT__ASK_FOR_BAUDRATE) {
     mySerial.begin(38400);
   }
   else 
   {
     Serial.println("Choose baudrate , send:\n 1 >> 1200\n 2 >> 2400 \n 3 >> 4800 \n 4 >> 9600 (Default) \n 5 >> 19200 \n 6 >> 38400 \n 7 >> 57600 \n 8 >> 115200 \n 9 >> 230400 ");
     while(! Serial.available() ){
       
     }
     if( Serial.available() ) {
       char BR = Serial.read();
       Serial.print("got selection : ");  Serial.println(BR);
       
       if(BR=='4')
         mySerial.begin(9600);
       else if(BR=='5')
         mySerial.begin(19200);
       else if(BR=='6')
         mySerial.begin(38400);
       else if(BR=='7')
         mySerial.begin(57600);
       else if(BR=='8')
         mySerial.begin(115200);
       else if(BR=='9')
         mySerial.begin(230400);
       //-
     }
   }
   Serial.println("\n--> Ready!\n\nType AT commands!");
   //
}

void loop()
{
   // Read device output if available.
   if (mySerial.available()) {
     Serial.write( mySerial.read() ); 
     //Serial.write(',');
   }
//   if (mySerial.available()) {
//     while(mySerial.available()) { // While there is more to be read, keep reading.
//       command += (char)mySerial.read();
//       delay(3);
//     }
//     Serial.println(command);
//     command = ""; // No repeats
//   }
   
  
   // Read user input if available.
   if (Serial.available() ){
       char Ch = Serial.read();
       mySerial.write(Ch);
       //delay(10); // The DELAY!
   }

   //delay(3);
   // END loop()
}


#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9); // RX, TX
String command = ""; // Stores response of bluetooth device
                                     // which simply allows \n between each
                                     // response.

int keyPin = 4;
uint8_t keyVal;

void setup() 
{
   // Open serial communications and wait for port to open:
   digitalWrite(keyPin, LOW);
   pinMode(keyPin, OUTPUT);
   keyVal = 0;
   
   
   Serial.begin(38400);
   Serial.println("Type AT commands!");
   // SoftwareSerial "com port" data rate. JY-MCU v1.03 defaults to 9600.
   mySerial.begin(38400);
}

void loop()
{
   // Read device output if available.
   if (mySerial.available()) {
     while(mySerial.available()) { // While there is more to be read, keep reading.
       command += (char)mySerial.read();
     }
     Serial.println(command);
     command = ""; // No repeats
   }
  
   // Read user input if available.
   if (Serial.available() ){
       char Ch = Serial.read();
       
       if(Ch=='|'){
         
         if(keyVal){
          keyVal = 0;
          digitalWrite(keyPin, LOW);
          Serial.println("KEY LOW");
         }
         else {
          keyVal = 1;
          digitalWrite(keyPin, HIGH);
          Serial.println("KEY HIGH");
         }
       }
       else{
         delay(10); // The DELAY!
         mySerial.write(Ch);
       }
   }
   // END loop()
}


//  Sketc: basicSerialWithNL_001
// 
//  Uses hardware serial to talk to the host computer and software serial 
//  for communication with the Bluetooth module
//  Intended for Bluetooth devices that require line end characters "\r\n"
//
//  Pins
//  Arduino 5V out TO BT VCC
//  Arduino GND to BT GND
//  Arduino D9 to BT RX through a voltage divider
//  Arduino D8 BT TX (no need voltage divider)
//
//  When a command is entered in the serial monitor on the computer 
//  the Arduino will relay it to the bluetooth module and display the result.
//
 
 
#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
 
const long baudRate = 9600;   // HM-10 Default : 9600


char PCbuff[50];
char BTbuff[50];
int PCi=0;
char c;

void setup() 
{
    Serial.begin(baudRate);
    BTserial.begin(baudRate);  
    
    Serial.print("BTserial started at "); Serial.println(baudRate);
    Serial.println(" ");
  
//    PCbuff = "";
//    BTbuff = "";
}


void loop()
{
 
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (BTserial.available())
    {
        c = BTserial.read();
        Serial.write(c);
    }
    
    if( Serial.available())
    {
      c = Serial.read();
      BTserial.write(c);
    }
    // Read from the Serial Monitor and send to the Bluetooth module
//    if (Serial.available())
//    {
//        c = Serial.read();
//        PCbuff[PCi] = c;
//        PCi++;
//        //if(c!=10 && c!=13)
//        if(c==10 || c==13)  
//        {
//          Serial.print("> ");
//          for( uint8_t x=0; x<PCi; x++){
//            BTserial.write(c);   
//            Serial.print( PCbuff[x] );
//          }
//          Serial.println();
//          PCi = 0;
//        }        
//    }
 
}

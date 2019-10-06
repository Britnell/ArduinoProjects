/*
 *   1. use hc05_ser_bridge as laptop serial to blueooth interface to configure
 *   
 *   2. use this bridge that just sends data to test connection.
 *   
 *   3. now sending sequential test data
 *   
 */
 
#include <SoftwareSerial.h>
SoftwareSerial BTserial(8, 9); // RX | TX
 
const long baudRate = 38400; 
char c=' ';
boolean NL = true;

void setup() 
{
    Serial.begin(38400);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    BTserial.begin(baudRate);  
    Serial.print("BTserial started at "); Serial.println(baudRate);
    Serial.println(" ");
}

uint8_t X = 0;

void loop()
{
    
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (BTserial.available())
    {
        c = BTserial.read();
        Serial.write(c);
    }
 
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();
        if(c=='p') {
          for(uint8_t i=0; i<255; i++) {
             BTserial.print(i);
             BTserial.write(',');
          }
        }
        else if(c=='w'){
          for(uint8_t i=0; i<255; i++) {
             BTserial.write(i);
             BTserial.write(',');
          }
        }
        else {
          BTserial.write(c); 
        }
        // if available
    }
 
}

/*    
 *     HM-10 testing
 *     
 *     Default baud 9600 \n\c
 *     AT     OK
 *     
 *     AT+ADDR?   
 *        =00:13:AA:00:5E:F3
 *        
 *     AT+VERSION
 *        =Firmware V4.2.0,Bluetooth V4.0 LE
 *        
 *     AT+NAME
 *        +NAME=HM10
 *        
 *     AT+PASS
 *      +PASS=1234
 *      
 *     AT+BAUD
 *      +BAUD=4
 *      
 *     AT+ROLE
 *      +ROLE=0     0 slave , 1 master
 *      
 *     AT+IMME
 *      +IMME=0     0 immediately, 
 *      
 *     AT+NOTI
 *      +NOTI=0     0 notific. off , 1 on
 *      
 *     AT+TYPE
 *       +TYPE=0
 *       
 * 
 */ 


#include <SoftwareSerial.h>

// BAUDs      9600      38400     
#define   BAUD   9600

SoftwareSerial HM10(2, 3);    // ( Rx, Tx ) 


void setup() {
  Serial.begin(BAUD);

  // Serial1 , pin 18,19 
  // Serial2 , pin 17,16 
  // Serial3 , pin 15,14 
  
  HM10.begin(BAUD);

}

void loop() {
  // -

  // * fowrard USB to BT
  while(Serial.available()){
    HM10.write( Serial.read() );
  }
  
  // * forward BT to USB
  while(HM10.available()){
    Serial.write(HM10.read());
  }  

  // * Eo loop
}

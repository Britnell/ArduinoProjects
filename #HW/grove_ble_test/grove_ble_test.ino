#include <SoftwareSerial.h>   //Software Serial Port

#define RxD 2
#define TxD 3

#define pt(x)   Serial.print(x)
#define pn(x)   Serial.println(x)

SoftwareSerial BLE(RxD,TxD);


void setup()
{
  Serial.begin(115200);
  BLE.begin(115200); //
  
  pt(" ready. starting BT connecction");
  
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  
 //  setupBleConnection();
  
//  BLE.print("AT+ADDR"); //clear all previous setting
//  BLE.print("AT+ROLE0"); //set the bluetooth name as a slaver
//  BLE.print("AT+SAVE1");  //don't save the connect information
  
}


void loop()
{
  char recvChar;
  while(1){
    if(BLE.available()){ 
      recvChar = BLE.read();
      uint8_t dec = (int)(recvChar);
      Serial.println(dec);
    }
    if(Serial.available()){//check if there's any data sent from the local serial terminal, you can add the other applications here
      recvChar  = Serial.read();
      BLE.print(recvChar);
    }
  }
}

void setupBleConnection()
{
  BLE.begin(9600); //Set BLE BaudRate to default baud rate 9600
  BLE.print("AT+CLEAR"); //clear all previous setting
  BLE.print("AT+ROLE0"); //set the bluetooth name as a slaver
  BLE.print("AT+SAVE1");  //don't save the connect information
}


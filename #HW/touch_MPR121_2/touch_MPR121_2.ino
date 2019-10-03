/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
char buff[100];

void setup() {
  Serial.begin(115200);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  //if (!cap.begin(0x5A)) {
  if (!cap.myBegin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found! ");
  
  Print_Regs( (uint8_t)(0x00), 30);

  //    **    Eo setup
}



void loop() {       //    **      Loop
  
  //Print_Regs( (uint8_t)(0x00), 30);

    // get vals
  uint16_t *touches = getTouches(); 

    // printout
  Serial.print("Filt:\t");
      printTouches(touches);
  
    // averagizer
  uint16_t *avrged = averaging(touches);
  
  Serial.print(" \n Avrg:\t");
      printTouches(avrged);
  
  
  // * 
  delay(1000);
  
  //    ***     Eo Looooop
}




//    ***     Averaging

#define AVRG    3
uint16_t averager[12][AVRG];
uint8_t av_i = 0;


uint16_t * averaging(uint16_t *data){
  
  //
  uint16_t averaged[12];

  // * average array
  for(uint8_t t=0; t<12; t++)  
  {
      averager[t][av_i] = data[t];  // fill touch in for each average array at AVRG index
//      if(1){
//      sprintf(buff, " Adding %d to Touch %d / %d \n ", data[t] , t, av_i );
//          Serial.print(buff);
//      }
  }
  // * Step
  av_i++;
  if(av_i==AVRG)    av_i = 0;
  
  // * Calc avrg  
  
      //uint8_t t=0;
  for(uint8_t t=0; t<12;   t++)     // for each touch
  for(uint8_t a=0; a<AVRG; a++)     // for all averages : 
  {
    if(a==0) {
      averaged[t] = averager[t][a];
//      if(0){
//      sprintf(buff, "F T=%d / %d \tval=%u \t So avrg=%u \n ", t,a, averager[t][a], averaged[t] );
//        Serial.print(buff);
//      }
    }
    else if(a==AVRG-1) {
      averaged[t] += averager[t][a];  
//      if(1){
//      sprintf(buff, "= T=%d / %d \tval=%u \t So avrg=%u \t", t,a, averager[t][a], averaged[t] );
//        Serial.print(buff);
//      } 
      averaged[t] /= AVRG; 
//      if(1){
//      sprintf(buff, "%d \t ", averaged[t] );
//        Serial.print(buff);     
//      }
    }
    else {
      averaged[t] += averager[t][a];  
//      if(1){
//        sprintf(buff, "+ T=%d / %d \tval=%u \t So avrg=%u \n ", t,a, averager[t][a], averaged[t] );
//          Serial.print(buff);
//      }
    }
    // EO for for
  }

  return averaged;
  // Eo Averaging
}

void printTouches( uint16_t *vals){
  //  
  for(uint8_t t=0; t<12; t++){
    Serial.print( vals[t] );
    Serial.print("\t");
//    sprintf(buff, "%d \t ", vals[t] );
//        Serial.print(buff);  
  }
  Serial.println();
}

uint16_t  * getTouches( ) {
  uint16_t vals[12];
  
  for(uint8_t i=0; i<12; i++){
    vals[i] = cap.readRegister16(MPR121_FILTDATA_0L + i*2);
  } 
  return vals;
}


void Print_Regs( uint8_t start, uint8_t len ){
  sprintf(buff," Reading  %d of config : \n", len );    Serial.print(buff);
  
  //  uint8_t len = 30;
  uint8_t read_config[len];
  *read_config = cap.readRegisters( start, len);
  if(*read_config == 0){
    Serial.println(" Reading cconfig failed");
  }
  else {
    Serial.print("TCH:");
      Serial.print("\t");
      Serial.println(read_config[0], BIN);
      
    if(1){
      Serial.print("PRX:");
      Serial.print("\t");
      Serial.println(read_config[1], BIN);}
      
    for( uint8_t i=2; i<len; i++) {
      Serial.print(i, HEX);
      Serial.print("\t");
      Serial.println(read_config[i]);
    }
  }
  // Eo Read MPR
}


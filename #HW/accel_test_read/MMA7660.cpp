/*
 * MMA7660.cpp - Interface a MMA thermometer to an AVR via i2c
 * Version 0.1 - http://www.timzaman.com/
 * Copyright (c) 2011 Tim Zaman
 * All rights reserved.
 *
 */
 
#include "MMA7660.h"
//#include "WProgram.h"
#include <Wire.h>

#define MMA7660_ADDRESS 0x4C //This is the I2C address for our chip.
 
/* PUBLIC METHODS */

MMA::MMA()
{
}

void MMA::init()
{
	Wire.begin();       //Initiate the Wire library and join the I2C bus as a master	
	MMA7660_I2C_SEND(0x07,0x00); // Setting up MODE to Stand by to set SR
  
  //MMA7660_I2C_SEND(0x06,0x10);    // GINT
  //MMA7660_I2C_SEND(0x06,0x04);    // 2-PDINT
  
  MMA7660_I2C_SEND(0x06,0x05);    // 0-FBINT,  2-PDINT
  
  MMA7660_I2C_SEND(0x08,0x00);    // 
  
  MMA7660_I2C_SEND(0x09,0xE0);    // 7-ZDA, 6-YDA, 5-XDA, 
  
  MMA7660_I2C_SEND(0x07,0x01);    // Setting up MODE Active to START measures
  //
}

uint8_t MMA::readReg( uint8_t ADRS ){
  //
  while( Wire.available() ){
    Wire.read();
  }
  
  Wire.beginTransmission(MMA7660_ADDRESS);
  Wire.write(ADRS);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(MMA7660_ADDRESS, 1); // read a byte
  
  uint8_t resp = 0xFF;

//  Serial.print('a');
//  while(!Wire.available() ) 
//    {     }
//  Serial.print('b');

//  while(Wire.available() ){
//    Serial.println( Wire.read() );
//  }
  
  if(Wire.available() )   {
    resp = Wire.read();
//    Serial.println( Wire.read() );
//    Serial.println( Wire.read() );
//    Serial.println( Wire.read() );
  }
  
  return resp;
   
   // Eo readReg
}

void MMA::getAccel(int *x,int *y,int *z)
{
  Wire.beginTransmission(MMA7660_ADDRESS);
  Wire.write(0x00);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(MMA7660_ADDRESS, 3); // read a byte

   if(Wire.available() ) 
   {
     *x=Wire.read();
     *y=Wire.read();
     *z=Wire.read();
   }
   *x=MMA_Accel[*x];
   *y=MMA_Accel[*y];
   *z=MMA_Accel[*z];
}

void MMA::getRegs(int *ray)
{
  Wire.beginTransmission(MMA7660_ADDRESS);
  Wire.write(0x00);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(MMA7660_ADDRESS, 9); // read a byte

   if(Wire.available() ) 
   {
     for(int x=0; x<9; x++) {
      ray[x] = Wire.read();
     }
   }
   //
}

void MMA::getAccel8(int8_t *x,int8_t *y,int8_t *z)
{
  Wire.beginTransmission(MMA7660_ADDRESS);
  Wire.write(0x00);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(MMA7660_ADDRESS, 3); // read a byte

   if(Wire.available()){
     *x=Wire.read();
     *y=Wire.read();
     *z=Wire.read();
   }
   *x=MMA_Accel[*x];
   *y=MMA_Accel[*y];
   *z=MMA_Accel[*z];
}

// This can be called at 100ms intervals to get new data
void MMA::getValues(int *x,int *y,int *z)
{
  Wire.beginTransmission(MMA7660_ADDRESS);
  Wire.write(0x00);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(MMA7660_ADDRESS, 3); // read a byte

   if(Wire.available()){
     *x=Wire.read();
     *y=Wire.read();
     *z=Wire.read();
   }
   *x=MMA_XYar[*x];
   *y=MMA_XYar[*y];
   *z=MMA_Zar[*z];
}

void MMA::getValues_t(int8_t *x,int8_t *y,int8_t *z)
{
  Wire.beginTransmission(MMA7660_ADDRESS);
  Wire.write(0x00);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(MMA7660_ADDRESS, 3); // read a byte

   if(Wire.available()){
     *x=Wire.read();
     *y=Wire.read();
     *z=Wire.read();
   }
   *x=MMA_XYar[*x];
   *y=MMA_XYar[*y];
   *z=MMA_Zar[*z];
}


void MMA::MMA7660_I2C_SEND(unsigned char REG_ADDRESS, unsigned  char DATA){  //SEND data to MMA7660
  Wire.beginTransmission(MMA7660_ADDRESS);
  Wire.write(REG_ADDRESS);
  Wire.write(DATA);
  Wire.endTransmission();
}


// Set the default object
MMA MMA7660 = MMA();











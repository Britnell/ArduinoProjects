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

int MMA_XYar[64] = {0,3,5, 8,11,14, 16,19,22, 25,28,31, 34,38,41, 45,49,53, 58,63,70, 78,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,-80,-70,-63,-58,-53,-49,-45,-41,-38,-34,-31,-28,-25,-22,-19,-16,-14,-11,-8,-5,-3};

int MMA_Zar[64] = { 90,87, 84,82,79, 76,74,70, 68,65,62, 59,56,52, 49,45,41, 37,32,27, 20,10,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-10,-20,-27,-32,-37,-41,-45,-49,-52,-56,-59,-52,-65,-68,-71,-74,-76,-79,-82,-85,-87};

int MMA_Accel[64] = { 0,47,94,141,188,234,281,328,375,422,469,516,563,609,656,703,750,   797,844,891,938,984,1031,1078,1125,1172,1219,1266,1313,1359,1406,1453,   -1500,-1453,-1406,-1359,-1313,-1266,-1219,-1172,-1125,-1078,-1031,-984,-938,-891,-844,    -797,-750,-703,-656,-609,-563,-516,-469,-422,-375,-328,-281,-234,-188,-141,-94,-47 };

 
/* PUBLIC METHODS */

MMA::MMA()
{
}

void MMA::init()
{
	Wire.begin();       //Initiate the Wire library and join the I2C bus as a master	
	MMA7660_I2C_SEND(0x07,0x00); // Setting up MODE to Stand by to set SR
    MMA7660_I2C_SEND(0x06,0x10);
    MMA7660_I2C_SEND(0x08,0x00);  // Setting up SR register to 120 samples active and auto sleep mode
    MMA7660_I2C_SEND(0x07,0x01); //Setting up MODE Active to START measures
}

void MMA::getAccel(int *x,int *y,int *z)
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











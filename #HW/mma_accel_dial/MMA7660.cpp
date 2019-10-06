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

// Conversion tables : MMA data to angle & accel values
int MMA_XYar[64] = {0,3,5, 8,11,14, 16,19,22, 25,28,31, 34,38,41, 45,49,53, 58,63,70, 78,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,-80,-70,-63,-58,-53,-49,-45,-41,-38,-34,-31,-28,-25,-22,-19,-16,-14,-11,-8,-5,-3};
int MMA_Zar[64] = { 90,87, 84,82,79, 76,74,70, 68,65,62, 59,56,52, 49,45,41, 37,32,27, 20,10,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-10,-20,-27,-32,-37,-41,-45,-49,-52,-56,-59,-52,-65,-68,-71,-74,-76,-79,-82,-85,-87};
int MMA_Accel[64] = { 0,47,94,141,188,234,281,328,375,422,469,516,563,609,656,703,750,   797,844,891,938,984,1031,1078,1125,1172,1219,1266,1313,1359,1406,1453,   -1500,-1453,-1406,-1359,-1313,-1266,-1219,-1172,-1125,-1078,-1031,-984,-938,-891,-844,    -797,-750,-703,-656,-609,-563,-516,-469,-422,-375,-328,-281,-234,-188,-141,-94,-47 };

#define READ_REGS     4
uint8_t mma_regs[READ_REGS];


/* PUBLIC METHODS */

MMA::MMA()
{
}

void MMA::init()
{
	Wire.begin();       //Initiate the Wire library and join the I2C bus as a master	

  // 1. Setting up MODE to Standby to write to setup registers
	standby_mode(); 
  
  //      ##  INTSU 
  // 0- front/back Int
  // 1 - UpDoLeRi Int
  // 2 - tap det int
  // 3 - exiting autosleep int
  // 4 - GINT
  // 0xE0 - 5:7 ShakeXYZ int 
  MMA7660_I2C_SEND(0x06, 0x00  );   // no interrupts
  
  //      ## SR 
  // ~~ AMSR[0:2]
  // 0x00 - 120 S/s tap detect mode sample rate
  // 0x01 - 64 S/s
  // ... 32, 16, 8, 4, 2, 
  // 0x07 - 1 S/s
  // 
  // ~~ FILT[5:7] debounce of filter change Int
  // (x<<5)
  //   0 = disable 
  //   1 .. 7 = 2 .. 8 samples debounce
  MMA7660_I2C_SEND(0x08, 0x00 | (7<<5) ); 
  
  //      ## PDET 
  //  PDTH[4:0] pulse det int thresh : X counts
  //  Pulse det on axes :
  //    5-XDA,    6-YDA,      7-ZDA, 
  //    All x,y,z = 0xE0
  MMA7660_I2C_SEND(0x09,0x00);
  
  active_mode();    // Setting up MODE Active to START measures
  
  //
}


/*  Read all (relevant) internal registers of MMA
 *    read once, then use .get___() to retrieve parameters
 */
void MMA::readRegs()
{
  Wire.beginTransmission(MMA7660_ADDRESS);
  Wire.write(0x00);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(MMA7660_ADDRESS, READ_REGS); // read a byte

   if(Wire.available() ) 
   {
     for(int x=0; x<READ_REGS; x++) {
      mma_regs[x] = Wire.read();
     }
   }
   //
}

// see TILT $03 register, datasheet p.15
uint8_t MMA::getTilt() {
  return mma_regs[3];
}

// see TILT reg
uint8_t MMA::isFront() {
  return (mma_regs[3] & BIT0 );
}
uint8_t MMA::isBack() {
  return (mma_regs[3] & BIT1 );
}
uint8_t MMA::isTap() {
  return (mma_regs[3] & BIT5 );
}
uint8_t MMA::isShake() {
  return (mma_regs[3] & BIT7 );
}


uint8_t MMA::polar() {
  return ( 0x07 & (mma_regs[3]>>2) );
}

// return char which way accel is "pointing"
char MMA::pointing() {
  switch( 0x07 & (mma_regs[3]>>2) )
  {
    case 0:
      return 'n'; // no
      break;
    case 1:
      return 'l'; // left
      break;
    case 2:
      return 'r'; // right
      break;
    case 5:
      return 'd'; // down
      break;
    case 6:
      return 'u'; // up
      break;
    default:
      return 'n'; // no
      break;
  }
  // Eo point
}



int MMA::getAngleX(){
  return MMA_XYar[ mma_regs[0] ];
}
int MMA::getAngleY(){
  return MMA_XYar[ mma_regs[1] ];
}
int MMA::getAngleZ(){
  return MMA_Zar[ mma_regs[2] ];
}

int MMA::getAccelX(){
  return MMA_Accel[ mma_regs[0] ];
}
int MMA::getAccelY(){
  return MMA_Accel[ mma_regs[1] ];
}
int MMA::getAccelZ(){
  return MMA_Accel[ mma_regs[2] ];
}

// Reads accel intro 3 pointers
void MMA::readAccel(int *x,int *y,int *z)
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


void MMA::standby_mode() {
  MMA7660_I2C_SEND(0x07,0x00);
}

void MMA::active_mode() {
  MMA7660_I2C_SEND(0x07,0x01);
}

void MMA::MMA7660_I2C_SEND(unsigned char REG_ADDRESS, unsigned  char DATA){  //SEND data to MMA7660
  Wire.beginTransmission(MMA7660_ADDRESS);
  Wire.write(REG_ADDRESS);
  Wire.write(DATA);
  Wire.endTransmission();
}


// Set the default object
MMA MMA7660 = MMA();



char MMA::orientation(int x, int y, int z)
{
  int Xa, Ya, Za;
  
  Xa = abs(x);
  Ya = abs(y);
  Za = abs(z);

  if( Xa>1300 || Ya>1300 || Za>1300 ) {
    // shake
    return 's';
  }
  else if( x<0  &&  Xa>Ya  &&  Za<800 )   {
    // Up
    return 'u';    
  }
  else if( x>0  &&  Xa>Ya  &&  Za<800 )  {
    // Down
    return 'd';
  }
  else if( y<0  &&  Ya>Xa  &&  Za<800 )  {
    // Right
    return 'r';
  }
  else if( y>0  &&  Ya>Xa  &&  Za<800 )  {
    // Left
    return 'l';
  }
  else if( z>250)  {
    // front
    return 'f';   
  }
  else if( z<-250 ){
    // back
    return 'b';
  }
  else  {
    // unknown
    return 'x';
  }
  // Eo orientation
}

//    #

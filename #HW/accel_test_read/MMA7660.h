/*
 * MMA7660.h - Interface a MMA7660 accelerometer to an AVR via i2c
 * Version 0.1 - http://www.timzaman.com/
 * Copyright (c) 2011 Tim Zaman
 * All rights reserved.
 *
 */

//#include "WProgram.h"
#include <Arduino.h>

#ifndef MMA7660_h
#define MMA7660_h


int MMA_XYar[64] = {0,3,5, 8,11,14, 16,19,22, 25,28,31, 34,38,41, 45,49,53, 58,63,70, 78,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,-80,-70,-63,-58,-53,-49,-45,-41,-38,-34,-31,-28,-25,-22,-19,-16,-14,-11,-8,-5,-3};
int MMA_Zar[64] = { 90,87, 84,82,79, 76,74,70, 68,65,62, 59,56,52, 49,45,41, 37,32,27, 20,10,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-10,-20,-27,-32,-37,-41,-45,-49,-52,-56,-59,-52,-65,-68,-71,-74,-76,-79,-82,-85,-87};
int MMA_Accel[64] = { 0,47,94,141,188,234,281,328,375,422,469,516,563,609,656,703,750,   797,844,891,938,984,1031,1078,1125,1172,1219,1266,1313,1359,1406,1453,   -1500,-1453,-1406,-1359,-1313,-1266,-1219,-1172,-1125,-1078,-1031,-984,-938,-891,-844,    -797,-750,-703,-656,-609,-563,-516,-469,-422,-375,-328,-281,-234,-188,-141,-94,-47 };

typedef int8_t   acc_int;


class MMA
{
  public:
    MMA();
    void init();
    void getValues(int *x,int *y,int *z);
    void getValues_t(int8_t *x,int8_t *y,int8_t *z);
    void getAccel(int *x,int *y,int *z);
    void getRegs(int *ray);
    void getAccel8(int8_t *x,int8_t *y,int8_t *z);
    uint8_t readReg( uint8_t ADRS );
  private:
	void MMA7660_I2C_SEND(unsigned char REG_ADDRESS, unsigned  char DATA);
};

extern MMA MMA7660;

#endif // MMA7660_h

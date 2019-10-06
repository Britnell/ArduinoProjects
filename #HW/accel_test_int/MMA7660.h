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

typedef int8_t   acc_int;


class MMA
{
  public:
    MMA();
    void init();
    void getValues(int *x,int *y,int *z);
    void getValues_t(int8_t *x,int8_t *y,int8_t *z);
    void getAccel(int *x,int *y,int *z);
    void getAccel8(int8_t *x,int8_t *y,int8_t *z);
    uint8_t readReg( uint8_t ADRS );
  private:
	void MMA7660_I2C_SEND(unsigned char REG_ADDRESS, unsigned  char DATA);
};

extern MMA MMA7660;

#endif // MMA7660_h

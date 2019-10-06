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

#define BIT0    0x01
#define BIT1    0x02
#define BIT2    0x04
#define BIT3    0x08
#define BIT4    0x10
#define BIT5    0x20
#define BIT6    0x40
#define BIT7    0x80

typedef int8_t   acc_int;


class MMA
{
  public:
    MMA();
    void init();
    void standby_mode();
    void active_mode();
    
    void readAccel(int *x,int *y,int *z);  
    
    void readRegs();
      uint8_t isFront();
      uint8_t isBack();
      uint8_t isTap();
      uint8_t isShake();
      uint8_t polar();
      char pointing();
      
    int getAngleX();
      int getAngleY();
      int getAngleZ();
    int getAccelX();
      int getAccelY();
      int getAccelZ();
    // get registers
    uint8_t getTilt();
    void getRegs(int *ray);
    char orientation(int x, int y, int z);
    
  private:
	  void MMA7660_I2C_SEND(unsigned char REG_ADDRESS, unsigned  char DATA);
};

extern MMA accel;

#endif // MMA7660_h

/*
 * Interrupt triggers on Front/ back orientation change
 *   then read accel data to get orientation
 * 
 * Issues reading TILT reg which stores orientation readily...
 * I2C problem?? probably. values seem to repeat...
 */

#include <Wire.h>
#include "MMA7660.h"

#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)
#define BIT(x)          (1<<x)

// pin D2, INT0       D3, INT1
#define INT_PIN     3

#define TILT    0x03

char p_buff[30];


void setup()
{
  Serial.begin(9600);
  Serial.println("Lets begin now..");

  // accel setup
  MMA7660.init();
  
  // int pin
  CLR(DDRD, INT_PIN);   // set input
  SET(PORTD, INT_PIN);  // set pull-up
  attachInterrupt( INT1, accel_int, FALLING );
  
  // Eo setup
}

int x,y,z;
//int8_t x,y,z;
char facing;
  
void loop()
{
  // -- 

  MMA7660.getAccel(&x,&y,&z);
  facing = orientation();
  print_xyz(1);

  
  Serial.print("Reg3: ");
  Serial.println(MMA7660.readReg(0x03), DEC);

  Serial.print("Reg4: ");
  Serial.println(MMA7660.readReg(0x04), DEC);

  Serial.print("Reg5: ");
  Serial.println(MMA7660.readReg(0x05), DEC);
  
  Serial.print("Reg6: ");
  Serial.println(MMA7660.readReg(0x06), DEC);

  Serial.print("Reg7: ");
  Serial.println(MMA7660.readReg(0x07), DEC);

  
  //Serial.println(reg, BIN );
  
//uint8_t reg = MMA7660.readReg(TILT);
//  uint8_t bafro = (reg & 0x03);
//  uint8_t polar = (reg & 0x1C) >>2; // bits2:4
//  sprintf(p_buff, "back/fron: %d\tpolar: %d\n", bafro, polar);  Serial.print(p_buff);
  
  
  //MMA7660.getValues(&x,&y,&z);
  //MMA7660.getValues_t(&x,&y,&z);
  //MMA7660.getAccel8(&x,&y,&z);
  
  //Serial.println( PIND && (1<<INT_PIN) );
  
  delay(100); // There will be new values every 100ms
  
  // Eo loop
}


void accel_int(void)
{
  Serial.println("Accel interrupt ");
  
  // Eo INT
}


char orientation(void)
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

void print_xyz(uint8_t newline)
{
  sprintf(p_buff, "x:\t%d\ty:\t%d\tz:\t%d\t%c\t", x, y, z, facing);
  if(newline)
    Serial.println(p_buff);
  else
    Serial.print(p_buff);
  
//  Serial.print("x: ");
//  Serial.print(x);
//  Serial.print(" y: ");
//  Serial.print(y);
//  Serial.print(" z: ");
//  Serial.println(z);
}

// --

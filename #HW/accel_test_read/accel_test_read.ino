/*
 * 
 */

#include <Wire.h>
#include "MMA7660.h"

#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)

// pin D2, INT0       D3, INT1
#define INT_PIN     3

char buff[40];

int x,y,z;
//int8_t x,y,z;
char facing;

//int MMA_Accel[64] = { 0,47,94,141,188,234,281,328,375,422,469,516,563,609,656,703,750,   797,844,891,938,984,1031,1078,1125,1172,1219,1266,1313,1359,1406,1453,   -1500,-1453,-1406,-1359,-1313,-1266,-1219,-1172,-1125,-1078,-1031,-984,-938,-891,-844,    -797,-750,-703,-656,-609,-563,-516,-469,-422,-375,-328,-281,-234,-188,-141,-94,-47 };

  
void setup()
{
  Serial.begin(115200);
  Serial.println("Lets begin now..");

  MMA7660.init();
  delay(1000);

  MMA7660.getAccel(&x,&y,&z);
  facing = orientation();
  print_xyz();
  delay(500);
  
  // read Accel reg content
  for(uint8_t r= 0; r< 11; r++) {
    uint8_t reg = MMA7660.readReg(r);
    sprintf(buff, "Reg %d :: %d\n", r, reg);
    Serial.print(buff);
  }  delay(1000);
  
  // Interrupt pin
  CLR(DDRD, INT_PIN);   // set input
  SET(PORTD, INT_PIN);  // set pull-up
  attachInterrupt( INT1, accel_int, FALLING );
}

int regs[10];

void loop()
{
  // -- 
  delay(100); // There will be new values every 100ms
  
  //MMA7660.getValues(&x,&y,&z);
  //MMA7660.getValues_t(&x,&y,&z);
  //MMA7660.getAccel8(&x,&y,&z);

  // Read accel values
  //MMA7660.getAccel(&x,&y,&z);

  // Read registers
  MMA7660.getRegs(regs);
  
  x = MMA_Accel[ regs[0] ];
  y = MMA_Accel[ regs[1] ];
  z = MMA_Accel[ regs[2] ];
  
  facing = orientation();
  print_xyz();
  
  for( int x=0; x<10; x++) {
     sprintf(buff, "Reg %d :: %d\n", x, regs[x]);
     Serial.print(buff);
  }
//  sprintf(buff, "Reg %d :: %d\n", 0, MMA7660.readReg(0) );
//  Serial.print(buff);
  
  //Serial.println( PIND && (1<<INT_PIN) );
  
  // Eo loop
}


void accel_int(void)
{
  Serial.println("Accel interrupt ");
  
}


char orientation(int x, int y, int z)
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

char p_buff[30];

void print_xyz(void)
{
  sprintf(p_buff, "x:\t%d\ty:\t%d\tz:\t%d\t%c\n", x, y, z, facing);
  Serial.print(p_buff);
  
//  Serial.print("x: ");
//  Serial.print(x);
//  Serial.print(" y: ");
//  Serial.print(y);
//  Serial.print(" z: ");
//  Serial.println(z);
}

// --

#include <Wire.h>
#include "MMA7660.h"

void setup()
{
  Serial.begin(9600);
  MMA7660.init();
}

int x,y,z;
//int8_t x,y,z;
char facing;
  
void loop()
{
  // -- 
  
  delay(100); // There will be new values every 100ms
  
  //MMA7660.getValues(&x,&y,&z);
  //MMA7660.getValues_t(&x,&y,&z);
  //MMA7660.getAccel8(&x,&y,&z);
  
  MMA7660.getAccel(&x,&y,&z);
  facing = orientation();
  
  print_xyz();
  
  // Eo loop
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

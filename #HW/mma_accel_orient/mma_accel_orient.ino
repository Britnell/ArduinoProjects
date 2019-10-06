/*    Demo for MMA7660 accelerometer. Determines orientation for "cube" in 6 directions / axis 
 *        and prints when orientation changes
 *    - constantly reading acc sensor and calculating orientation according to datasheet.
 *    - 'still' filter. values are only passed on if the last X=AVRG values are all equal.
 *    -  AVRG must be 2 or larger
 *      
 */

#include <Wire.h>
#include "MMA7660.h"

#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)


// pin D2, INT0       D3, INT1
#define INT_PIN     3

char buff[40];

uint8_t last_bafro = 0;
uint8_t last_polar = 0;
uint8_t accel_interrupt = 0;

                                    //  # Variables

int regs[10];
int x, y, z;

char last_facing = 0;

#define AVRG      2
char facing_avrg[AVRG];
uint8_t avrg_index = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Lets begin now..");

  accel.init();
  

  for(int a=0; a<AVRG; a++) {
    facing_avrg[a] = 0;
  }

  delay(500);
  
  // Interrupt pin
  CLR(DDRD, INT_PIN);   // set as input
  SET(PORTD, INT_PIN);  // set pull-up
  //attachInterrupt( INT1, accel_int_routine, FALLING );
  
  // Eo setup
}


void loop()
{
  // -- 

  // read XYZ acceleration values
  accel.readAccel( &x, &y, &z);
  
  // calc orientation from accel.
  char facing = accel.orientation(x, y, z);

  // additionally now the results of facing are stored and filtered.   
  facing_avrg[avrg_index] = facing;
  
  avrg_index++;
  if(avrg_index==AVRG)
    avrg_index = 0;
  
  uint8_t equal = 1;
  
  for(int a=1; a<AVRG; a++) {
    if( facing_avrg[a] != facing_avrg[a-1] ) {
      equal = 0;
    }
  }
  
  if(equal) 
  {
    // debounce
    if( facing != last_facing) 
    if( facing != 'x') {
      sprintf(buff, "now facing :  %c \n", facing);   Serial.print(buff);
      last_facing = facing;
    }
    
    //delay(400);
    // Eo equal
  }
  
  delay(10); // There will be new values every 100ms
  
  // Eo loop
}





// --

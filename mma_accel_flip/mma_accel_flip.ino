/*    MMA7660 accelerometer demo : interrupt driven to recognize flips through interrupts.
 *     Detects vertical flips, i.e. between flat facing upward / downward.
 *     named forward / backwards for the accelerometer.
 *     
 *     for accel setup registers see void MMA::init() in "MMA7660.cpp"
 *     
 */

#include <Wire.h>
#include "MMA7660.h"

#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)


// pin D2, INT0       D3, INT1
#define INT_PIN     3

char buff[40];

//int8_t x,y,z;

uint8_t last_bafro = 0;
uint8_t last_polar = 0;
uint8_t accel_interrupt = 0;


  
void setup()
{
  Serial.begin(115200);
  Serial.println("Lets begin now..");

  accel.init();
  delay(1000);
  
  // Interrupt pin
  CLR(DDRD, INT_PIN);   // set as input
  SET(PORTD, INT_PIN);  // set pull-up
  attachInterrupt( INT1, accel_int_routine, FALLING );
}

int regs[10];

void loop()
{
  // -- 
  
  // handle interrupts here so interrupts triggers can not conflict
  if( accel_interrupt ) 
  {
    // handle-function to determine the type of interrupt
    accel_handle_int();

    // CLEAR FLAG
    accel_interrupt = 0;
    
    // Eo if accel_interrupt
  }
  else {
    accel.readRegs();
  }

  delay(10); // There will be new values every x ms
  
  // Eo loop
}


                                                        // ## Accel interrupt

void accel_int_routine(void)
{
   // set FLAG, handle in loop
   accel_interrupt = 1;
}

void accel_handle_int(void)
{
    Serial.print("Accel interrupt :");
  
    // Read MMA registers
    accel.readRegs();
    
    //sprintf(buff, "\tFro : Back = %d : %d \n", accel.isFront(), accel.isBack() ); Serial.print(buff);
    
    if( accel.isFront() != last_bafro ) {
      last_bafro = accel.isFront();
      // code here..
      Serial.print( "\t flipped to : ");
      if(accel.isFront())   //accel.(accel.isBack())
        Serial.print("front");
      else  
        Serial.print("back");
              
    }
    
    Serial.println();

    // Eo func
}


// --

/*    Demo constantly reading acc sensor and calculating orientation.
 *      outputting orientation for "cube" in 6 directions
 *      
 */
 
/*  ERRORS : i currently seem to read TAP interrupts although they are DISABLED..
 *    unreliable for now
 *   Interrupts only run if MMA regs are read regularly in loop
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

#define AVRG      10
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
  attachInterrupt( INT1, accel_int_routine, FALLING );
  
  // Eo setup
}


void loop()
{
  // -- 

  // read XYZ acceleration values
  accel.readAccel( &x, &y, &z);
  
  // calc orientation from accel.
  char facing = accel.orientation(x, y, z);

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
    if( facing != last_facing) {
      //--
      hourglass_loop(facing, last_facing);
      sprintf(buff, "now facing :  %c \n", facing);   Serial.print(buff);
      last_facing = facing;
    }
    
    
    // Eo equal
  }
  
  delay(10); // There will be new values every 100ms
  
  // Eo loop
}

char current_state = 'r';
char resume_timer = 'w';
char scroll_face;
uint8_t scroll = 5;

void hourglass_loop(char new_face, char last_face )
{
  // -

  // up/down
  if( new_face=='f' )
  {
    // resume work
    current_state = 'w';
    resume_timer = 'w';
    Serial.println("back to work");
  }
  else if( new_face=='b' )
  {
    // resume break
    current_state = 'b';
    resume_timer = 'b';
    Serial.println("begin to break");
    
  }
  else 
  {
    // -- sideways
    
    if(current_state=='r') {
      // if already in roll state
      scroll += scroll_f(scroll_face, new_face );
      sprintf(buff,"scroll to %c : %d \n", scroll_face, scroll);  Serial.print(buff);
      scroll_face = new_face;
    }
    else {
      // begin roll state
      current_state = 'r';
      scroll_face = new_face;
      sprintf(buff,"begin scroll with face: %c \n", scroll_face);  Serial.print(buff);
    }
    // Eo else
  }
  
  // Eo hourglass
}

int scroll_f(char curr, char neww ){
  //
       if(curr=='u' && neww=='r')
    return 1;
  else if(curr=='u' && neww=='l')
    return -1;
  else if(curr=='r' && neww=='u')
    return -1;
  else if(curr=='r' && neww=='d')
    return 1;
  else if(curr=='l' && neww=='u')
    return 1;
  else if(curr=='l' && neww=='d')
    return -1;
  else if(curr=='d' && neww=='r')
    return -1;
  else if(curr=='d' && neww=='l')
    return 1;
  else 
    return 0;
  
  // Eo scroll
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
      if(accel.isFront())
        Serial.print("front");
      else
        Serial.print("back");
              
    }
    
    Serial.println();

    // Eo func
}



// --

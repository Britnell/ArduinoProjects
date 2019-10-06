/*     * Test sketch for accelerometer MMA7660
 *      
 *   Sets Accelerometer up in standard setting + activates some interrupts.
 *    debug print statements that can be turned of by setting the if(1) to 0
 * 
 */

/*  ERRORS : i currently seem to read TAP interrupts although they are DISABLED..
 *    unreliable for now
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
  int x,y,z;
  
  delay(100); // There will be new values every 100ms
  
  // 1. first read registers
  accel.readRegs( );
  
  // 2. Get Angle
  
  if(1) {   // print XYZ angles
    sprintf(buff, "angles:\t%d\t%d\t%d\n" 
        ,accel.getAngleX(), accel.getAngleY(), accel.getAngleZ() );
    Serial.print(buff);
  }

  // 3. Get Accel
  if(1) {   // print XYZ accel
    sprintf(buff, "accel:\t%d\t%d\t%d\n" 
        ,accel.getAccelX(), accel.getAccelY(), accel.getAccelZ() );
    Serial.print(buff);
  }

  // 4. Read orientation registers
  if(0) {
    
    if( accel.isFront() ) {
      Serial.println("BaFro: UP");
    }
    
    if( accel.isBack() ) {
      Serial.println("BaFro: DOWN");
    }

    // use .polar() for direction as INT
    Serial.println(  accel.polar() );
    
    // use .pointing() for direction as easily readable char
    sprintf(buff,"pointing : %c \n", accel.pointing() );   Serial.print(buff);

    //Serial.print("Tilt reg content: ");     Serial.println( accel.getTilt() , BIN);
    //-
  }

  if(0)
  // handle interrupts here so interrupts triggers can not conflict
  if( accel_interrupt ) 
  {
    // handle-function to determine the type of interrupt
    accel_handle_int();

    // CLEAR FLAG
    accel_interrupt = 0;
    
    // Eo if accel_interrupt
  }
  
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
  
    if(accel.isTap() ) {
      Serial.print("\tTAP");
    }
  
    if( accel.isShake() ) {
      Serial.print("\tSHAKE");
    }

    //sprintf(buff, "\tFro : Back = %d : %d \n", accel.isFront(), accel.isBack() ); Serial.print(buff);
    
    if( accel.isFront() != last_bafro ) {
      last_bafro = accel.isFront();
      // code here..
      Serial.print("\tFRONT BACK flipped");
    }
    
    if( accel.polar() != last_polar ) {
      last_polar = accel.polar();
      Serial.print("\tPOLAR changed to ");
      Serial.print(accel.pointing() );
    }

    Serial.println();

    // Eo func
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


// --

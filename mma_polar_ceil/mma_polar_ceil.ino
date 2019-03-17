/*    
 *     Can I use polar coordinated to point at the ceiling??
 *     
 *     Calculates Polar coordinates from accelerometer acceleration values.
 *       as full accel magnitude, in g * 1000 
 *       angle in X-Y-plane
 *       z-axis angle of vortical, 100 = full face up, -100 = face down
 *       
 */

#include <Wire.h>
#include "MMA7660.h"
#include <Adafruit_NeoPixel.h>

#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)

// Acce INT pin
// pin D2, INT0       D3, INT1
#define INT_PIN     3

// NEOPIX
#define PIX_pin       5   // D5
#define PIX_pixels    16
Adafruit_NeoPixel Ring = Adafruit_NeoPixel(PIX_pixels, PIX_pin, NEO_RGBW + NEO_KHZ800);

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
  Ring.begin();
  
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
  int mag, angle, phi;
  
  // read XYZ acceleration values
  accel.readAccel( &x, &y, &z);
  
  // calc orientation from accel.
  char facing = accel.orientation(x, y, z);

  spherical( &mag, &angle, &phi, x, y, z );
  
    // ** debug output
  if(0) {
    sprintf(buff, "x,y,z : %d \t %d \t %d \n", x, y, z);  Serial.print(buff);
    sprintf(buff,"  XY-magnit %d, angle %d \n", mag, angle);    Serial.print( buff);
    sprintf(buff,"  mag %d \t z %d \t   = %d \n", mag, z, phi );   Serial.print(buff);
  }
  
  // print complete Polar Coordinates
  //sprintf(buff,"Polar Coordinates : \t %d g \t  @ %d deg \t // %d phi \n", mag, angle, phi );   Serial.print(buff);
  sprintf(buff,"Polar : \t  @ %d deg \t // %d phi \n", angle, phi );   Serial.print(buff);
  
  delay(100); // There will be new values every 100ms
  
  // Eo loop
}

// --
void paint_all( uint8_t r, uint8_t g, uint8_t b ) {
  for(uint8_t p=0; p<PIX_pixels; p++ ) 
    Ring.setPixelColor( p,  r,g,b );
    Ring.show();
  //
}

// gets spherical / polar coordinates in terms of 
//    magnitude - int - hypothenuse of x/y, combined magnitude of horizontal acceleration -1300 to +1300 g
//    angle     - int - direction of x/y magnitude, -180 to +180 degree
//    phi       - int - inclination Z-axis, -Pi to + Pi scaled /Pi as -100 to +100 = Down to Up
void  spherical( int *mag, int *angle, int *phi, int x, int y, int z )
{
  int XY = hypoth( x, y);
  *angle = int( 180 *atan2(y, x) /PI );
  *phi = int( atan2( z, XY ) *200 / PI );
  *mag = hypoth( XY, z);
  //
}

int hypoth( int X, int Y) {
  long calc, sum;
  
  calc = X;
  calc *= calc;
  sum = calc;   // x2
  calc = Y;
  calc *= calc;
  
  sum += calc;  // +y2
  
  return int( sqrt( sum ) );   // route
}



// --

/*    Demo for MMA7660 accelerometer + NeoPixel
 *    - get X/Y angle, taking running average of last X values
 *    - map angle 0-360 to modify variable
 *    - scale 0 to 18 to change  
 *    - output ar on LED ring      
 */

#include <Wire.h>
#include "MMA7660.h"

#include <Adafruit_NeoPixel.h>

#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)


// pin D2, INT0       D3, INT1
#define INT_PIN     3

                                    //  # Variables

int regs[10];

int x, y, z;
char buff[40];

// NEOPIX
#define PIX_pin       5   // D5
#define PIX_pixels    16
Adafruit_NeoPixel Ring = Adafruit_NeoPixel(PIX_pixels, PIX_pin, NEO_RGBW + NEO_KHZ800);

const int dial_range = 360;

void setup()
{
  clr_avrg( );

  Serial.begin(115200);
  Serial.println("Lets begin now..");

  Ring.begin();
  //Ring.setPixelColor( 0, Ring.Color(100,0,0) );     Ring.show();
  
  accel.init();

  delay(500);
  
  // Interrupt pin
  CLR(DDRD, INT_PIN);   // set as input
  SET(PORTD, INT_PIN);  // set pull-up
  //attachInterrupt( INT1, accel_int_routine, FALLING );
  
  // Eo setup
}


uint8_t dialing = 0;
int dial_var = 10;
int last_dial = dial_var;

int init_angle;
int alpha, lastAlpha;


void loop()
{
  // -- 

  // read XYZ acceleration values
  accel.readAccel( &x, &y, &z);
  
  // calc orientation from accel.
  char facing = accel.orientation(x, y, z);

  if( facing=='f' || facing=='b') {
    // Front or back, dont do much
    dialing = 0;
    clr_avrg();
  }
  else {
                              // on the dial axis
    
    if(!dialing) {
                              // init dial
      dialing = 1;
      alpha = calc_angle( x, y);
      add_to_avrg( alpha);
      lastAlpha = alpha;
    }
    else {
                                // continue dial here
      alpha = calc_angle( x, y);  // calc
      add_to_avrg( alpha);        // add to avrg
      //alpha = get_avrg( );        // calc new avrg
      alpha = get_avrg_wrap( );        // calc new avrg
      
      sprintf(buff,"avrg %d,  \t", alpha);  Serial.print(buff);

      //add_delta( &dial_var, alpha, lastAlpha );
      //sprintf(buff, ", var %d , \t", dial_var);  Serial.print(buff);
      //sprintf(buff, " led %d \n ", dial_var/20);  Serial.print(buff);

      Ring.setPixelColor( to_LED( lastAlpha ) , Ring.Color(0,0,0) );   
      Ring.setPixelColor( to_LED( alpha ), Ring.Color(10,10,80) );
      Ring.show();
      lastAlpha = alpha;
      
      // Eo if dial
    }
    
    //wrap_var( &dial_var, dial_range);
    //Ring.setPixelColor( to_LED( last_dial ) , Ring.Color(0,0,0) );   
    //Ring.setPixelColor( to_LED( dial_var ), Ring.Color(10,10,80) );     
    //Ring.show();
    //last_dial = dial_var;
    
    // Eo Dialing
  }
  
  delay(90); // There will be new values every 100ms
  
  // Eo loop
}


// PIX_pixels 
int to_LED( int var)  {
  int LED = (var *PIX_pixels) /dial_range;
  return LED;
}


void add_delta( int *var, int curr, int last) 
{
      int diff = curr-last;\
      if( diff > 100 ) {
        // -180 to +180
        *var += ( diff-360);
        //Serial.println("\t - to + 180");
      }
      else if( diff < -100) {
        // + 180 to -180
        *var += (diff+360);
        //Serial.println("\t + to - 180");
      }
      else {
        // normal 
        *var +=  (diff);
      }
}

int calc_angle( int X, int Y) {
      float angle = atan2(Y, X);    // -2 PI to +2 PI
      angle = angle / PI;       // -1 to +1
      // inverted
      angle *= -1;
      angle += 1;             // 0 to +2
      angle *= 180;   // -180 to +180
      //sprintf(buff, "Dial x: %d\ty: %d   \tangl ", x, y );     Serial.print(buff);  //Serial.print(angle);
      alpha = int(angle);
      return alpha;
}


#define AVRG_SIZE    4
int avrg_array[AVRG_SIZE];
uint8_t avrg_index, avrg_size;

void clr_avrg( )
{
  avrg_size = 0;
  avrg_index = 0;
  for( int i=0; i<AVRG_SIZE; i++) 
  {    avrg_array[i] = 0;  }
  //
}

void add_to_avrg( int newVal) 
{
  avrg_array[avrg_index] = newVal;
  avrg_index++;
  if( avrg_index>= AVRG_SIZE )      avrg_index = 0;
  if( avrg_size < AVRG_SIZE)    avrg_size++;
  //
}

int get_avrg( )
{
  long calc = 0;
  for( int v=0; v<avrg_size; v++) 
  {     calc += avrg_array[v];  }
  calc /= avrg_size;
  
  return int(calc);
}

int get_avrg_wrap( )
{
  long calc = 0;
  int wrapping = 0;
  for( int w=1; w<avrg_size; w++) {
    if( abs(avrg_array[w-1] -avrg_array[w]) > 180 ) {
      wrapping = 1;
    }
    // Eo for
  }

  if( wrapping) {
    for( int v=0; v<avrg_size; v++) 
    {
      if( avrg_array[v] < 180) 
        calc += avrg_array[v] +360;  
      else
        calc += avrg_array[v];
    }
    calc /= avrg_size;
    if(calc >= 360)
      calc -= 360;
  }
  else {
    for( int v=0; v<avrg_size; v++) 
    {     
      calc += avrg_array[v];  
    }
    calc /= avrg_size;
  }
  
  return int(calc);
}


void wrap_var( int *var, int range) 
{
    if( *var > range )
       *var -= range;
    if( *var < 0 )
       *var += range;
}


// --

/*    Accel pickup sensor, detect when no longer stationary, i.e. moved or picked up
 *      triggers on any interrupt, orientation, tap, shake.
 *      also gets sum of accel over 5 averages and monitors changes to detect movement more subtle than 90 degree
 *      controls LEDs on pickup
 */

#include <Wire.h>
#include "MMA7660.h"
#include <Adafruit_NeoPixel.h>


#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)


// pin D2, INT0       D3, INT1
#define INT_PIN     3

char buff[40];

//int8_t x,y,z;

uint8_t last_bafro = 0;
uint8_t last_polar = 0;
uint8_t accel_interrupt = 0;

const int pickup_thr = 2000;
#define p_a   5
#define p_th  100


// NEOPIX
#define PIX_pin       5   // D5
#define PIX_pixels    16
Adafruit_NeoPixel Ring = Adafruit_NeoPixel(PIX_pixels, PIX_pin, NEO_RGBW + NEO_KHZ800);

uint8_t col_r = 90;
uint8_t col_g = 10;
uint8_t col_b = 20;

uint8_t fade_r, fade_g, fade_b;
uint8_t fadeing = 0;

void paint_all( uint8_t r, uint8_t g, uint8_t b ) {
  for(uint8_t p=0; p<PIX_pixels; p++ ) 
    Ring.setPixelColor( p,  r,g,b );
    Ring.show();
  //
}

void fade_begin( uint8_t r, uint8_t g, uint8_t b ) {
  fade_r = r;
  fade_g = g;
  fade_b = b;
  fadeing = 1;
}

#define fadeX   5
void fade_update( ) {
  if(fadeing) {
    paint_all( fade_r, fade_g, fade_b );
    //-
    if(fade_r > 10)
      fade_r -= (fade_r/fadeX);
    else if(fade_r > 0)
      fade_r--;
    //-
    if(fade_g > 10)
      fade_g -= (fade_g/fadeX);
    else if(fade_g > 0)
      fade_g--;
    //-
    if(fade_b > 10)
      fade_b -= (fade_b/fadeX);    
    else if(fade_b>0)
      fade_b--;

    if( fade_g==0 && fade_r==0 && fade_b==0 ) {
      paint_all( fade_r, fade_g, fade_b );
      fadeing= 0;
    }
  }
  //
}


void setup()
{
  clr_avrg();

  Ring.begin();
  
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
uint8_t pickup = 0;
long pickup_t = 0;
uint8_t p_i = 0;

int last_sum = 0;

void loop()
{
  // -- 
  int x, y, z, sum, absol;
    
  //sprintf(buff,"x,y,z: \t %d \t %d \t %d \t - Sum \t %d \n", x, y, z, sum );  Serial.print(buff);
      
  // handle interrupts here so interrupts triggers can not conflict
  if( accel_interrupt ) 
  {
    // handle-function to determine the type of interrupt
    accel_print_int();

    // CLEAR FLAG
    accel_interrupt = 0;

    if(!pickup) {
      pickup = 1;
      pickup_t = millis();
      Serial.println("pickup");
      fade_begin(col_r,col_g,col_b);
    }
    
    // Eo if accel_interrupt
  }
  else {
    accel.readRegs();
    
    if(pickup) {
      if( millis() -pickup_t > pickup_thr ) {
        pickup = 0;
        Serial.println("debounce end");
      }
    }
    
      x = accel.getAccelX();
      y = accel.getAccelY();
      z = accel.getAccelZ();
      sum = abs(x) +abs(y) +abs(z);
      add_to_avrg(sum);
      p_i++;

      if(p_i== p_a) {
        p_i = 0;
        sum = get_avrg();
        //sprintf( buff, "sum : %d \t diff : %d \n", sum, sum-last_sum );   Serial.print(buff);
        absol = abs(sum-last_sum);
        if( absol > p_th ) {
          if( absol < 2000 )
            if( millis() -pickup_t > pickup_thr )  {
              sprintf(buff,"Accel Sum BOUNCE %d \n", absol );  Serial.print(buff);
              pickup_t = millis();
              pickup = 1;
              fade_begin(col_r,col_g,col_b);
            }
        }
        last_sum = sum;
      }
    
    
  }

  fade_update( );
  
  delay(100); // There will be new values every 100ms
  
  // Eo loop
}



                                                          // ##   Averaging

#define AVRG_SIZE    p_a
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

                                                        // ## Accel interrupt

void accel_int_routine(void)
{
   // set FLAG, handle in loop
   accel_interrupt = 1;
}

void accel_print_int(void)
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
      Serial.print( "\t flipped to : ");
      if(accel.isFront())
        Serial.print("front");
      else
        Serial.print("back");
              
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

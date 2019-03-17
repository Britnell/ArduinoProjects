/*
    After I almost bricked the arduino with v3 - i think -
    starting here v4 (from v2_1 )
    
    choses one random led at the time and fades to a random new color, within boundaries
    
*/


#include <Adafruit_NeoPixel.h>
#include "Pixel.h"
#include "LowPower.h"


#ifdef __AVR__
  #include <avr/power.h>
  #endif


#define PIN         10
#define PIXELS      6

#define DEBUG   1
#define __D     if(DEBUG)

Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(PIXELS, PIN, NEO_RGB + NEO_KHZ800);

/*  create a pixel instance for each pixel on the neopixel strip (Adafruit_NeoPixel object) 
 *    that you want to control with fade. When creating instance, simply give 
 *    integer of position in pixel strip. 
 */
Pixel myStrip[PIXELS];

int delayval = 1000; // delay for half a second
char buff[50];

#define red_min   5
#define red_max   40
#define green_min    1
#define green_max    20

#define della         10
#define interval      50

#define fadeIn_min    10
#define fadeIn_max    30
#define fadeOut_min   fadeIn_min
#define fadeOut_max   fadeIn_max

period_t random_sleep(){
  
  int coin = random(0,5); 
  __D Serial.print("\tRandom sleep for " ); 
  __D Serial.println(coin ); 
  
  // SLEEP_120MS,   LEEP_250MS,  SLEEP_500MS,  SLEEP_1S,  SLEEP_2S,  SLEEP_4S,  SLEEP_8S,
  
  if(coin==0)
    return SLEEP_120MS;
    
  else if(coin==1)
    return SLEEP_250MS;
    
  else if(coin==2)
    return SLEEP_500MS;
    
  else if(coin==3)
    return SLEEP_1S;
    
  else if(coin==4)
    return SLEEP_2S;

  else if(coin==5)
    return SLEEP_4S;
}

void setup() {
  
  randomSeed(analogRead(0));

  Serial.begin(115200);
  
  NeoPixel.begin(); // This initializes the NeoPixel library.
  Serial.println("Initialised Neopixel strip.");
  
  // Fade speed is determined by ( interval * framerate )
  // interval sets the number of iterations / steps the fade will take to reach the target colour
  // the speed is determined by the framerate, which is how quickly the main loop is run. 
  // this can either be measured, as example here.
  // but is generally most determined by delay(x);  =  x ms at the end of loop 
  
  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x] = Pixel(x);
    myStrip[x].set( 10+10*x, 0, 0 );    draw_pixel( (&myStrip[x]) );
  }
  NeoPixel.show();
  
  delay(500);

  myStrip[0].set( 20,2,0 );    
  myStrip[1].set( 20,5,0 );    
  myStrip[2].set( 20,10,0 );    
  myStrip[3].set( 20,15,0 );    
  
  
  for( int x=0; x<PIXELS ; x++ ) {
//    myStrip[x].set( 20,0,0 );    
    draw_pixel( (&myStrip[x]) );
  }
  NeoPixel.show();
  
  // Eo setup
}



void loop() 
{
  
  uint8_t none=true;
  
  for( uint8_t fly = 0; fly<PIXELS; fly++) {
    if( myStrip[fly].fade())  { 
      draw_pixel(&myStrip[fly]);
//      print_pixel(&myStrip[fly]);
      none=false;
    }
    // Eo for
  }
  
  if(none){
    
    // * if None faded
    uint8_t rndmpxl, red, green;
    
    __D Serial.println("LEts chose random pixel and adjust");
    rndmpxl = random(PIXELS);
    red = random( red_min, red_max );
    
    if(red<8)
      green = random(0,1);
    else if(red<15)   //
      green = random(0,4);
    else
      green = random( green_min, red/3 );
    
    myStrip[rndmpxl].fadeTo( red, green, 0, interval );
    
//    uint8_t red = myStrip[rndmpxl].R();
//    red += random(-5,5);   limit(&red, red_min, red_max );
//    uint8_t gre = myStrip[rndmpxl].G();
//    gre += random(-5,5);   limit(&gre, green_min, green_max );
//    myStrip[rndmpxl].fadeTo( red, gre, 0, interval );

    __D sprintf(buff, "fading r: %d , to ( %d / %d ) \n", rndmpxl, red, green );
    __D Serial.print(buff);
    
    //__D Serial.println("New colour chosen, lets sleep once");
    //__D Serial.flush();    
//    LowPower.idle( random_sleep(), ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF,
//                                   TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
    // ** Eo None
  }
  else
  {
    // * If NOT none then we just faded something so just continue
    NeoPixel.show();
    // Eo else none
  }
  
    // * period determines the framerate and thus the  a period of time (in milliseconds).
  delay(della); 
  
  // Eo **  loop
}


void limit( int * x, int low, int high){
  if(x<low)
    x=low;
  else if(x>high)
    x=high;
//  return x;
}


void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B() ) );
}

void print_pixel( Pixel * PIX ) {
  sprintf(buff, "f:\t%d\t( %d, %d, %d) \n", PIX->ID(), PIX->G(), PIX->R(), PIX->B() );
  Serial.print(buff);
}

// *

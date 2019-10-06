/*
    After I almost bricked the arduino with v3 - i think -
    starting here v4 (from v2_1 )
    
    NExt lets cross-fade from one to the next 
    and keep others dark
    
*/


#include <Adafruit_NeoPixel.h>
#include "Pixel.h"
#include "LowPower.h"


#ifdef __AVR__
  #include <avr/power.h>
  #endif


#define PIN         10
#define PIXELS      6

#define DEBUG   0
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

#define della        125
#define fade_min     12
#define fade_max     20

uint8_t fadeIn = 0;
uint8_t fadeOut = 0;

void snooze( period_t alarm){
  LowPower.idle( alarm, ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF,
                                   TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
}

void random_sleep(int Min, int Max ){
  int coin = random(Min, Max); 
  
  __D Serial.print("\tRandom sleep for " ); 
  __D Serial.println(coin ); 
  
  // SLEEP_120MS,   LEEP_250MS,  SLEEP_500MS,  SLEEP_1S,  SLEEP_2S,  SLEEP_4S,  SLEEP_8S,
  period_t period;
  if(coin==0)
    period = SLEEP_120MS;    
  else if(coin==1)
    period = SLEEP_250MS;    
  else if(coin==2)
    period = SLEEP_500MS;    
  else if(coin==3)
    period = SLEEP_1S;    
  else if(coin==4)
    period = SLEEP_2S;
  else if(coin==5)
    period = SLEEP_4S;
  
  LowPower.idle( period, ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF,
                                   TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
  // * Eo random_sleep  
}

void setup() {
  
  randomSeed(analogRead(0));

  __D Serial.begin(115200);
  
  NeoPixel.begin(); // This initializes the NeoPixel library.
  __D Serial.println("Initialised Neopixel strip.");
  
  // Fade speed is determined by ( interval * framerate )
  // interval sets the number of iterations / steps the fade will take to reach the target colour
  // the speed is determined by the framerate, which is how quickly the main loop is run. 
  // this can either be measured, as example here.
  // but is generally most determined by delay(x);  =  x ms at the end of loop 
  
  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x] = Pixel(x);
    myStrip[x].set( 4, 1, 0 );    draw_pixel( (&myStrip[x]) );
  }
  NeoPixel.show();

  
  // * choose first FadeIn to start
  
  fadeIn = random(PIXELS);
  myStrip[fadeIn].fadeTo( random(40,80), random(0,25), 0, random(fade_min, fade_max) );
  
  if(fadeIn==0)        fadeOut=1;
  else     fadeOut = 0;
  
  delay(1000);
  
  // Eo setup
}


void loop() 
{
  uint8_t none = true;
  
  // * Fade Out
  if( myStrip[fadeOut].fade())  { 
    draw_pixel( &myStrip[fadeOut] );
    none = false;
  }

  // * Fade In
  if( myStrip[fadeIn].fade())  { 
    draw_pixel( &myStrip[fadeIn] );
    none = false;
  }

  if(none){
    // fade out what we just faded in
    fadeOut = fadeIn;
    
    // fade out
    uint8_t rr = random(4,8); // make them dark
    uint8_t gg = random(0,2); // one in 3 is RED
    myStrip[fadeOut].fadeTo( rr , gg , 0, random(fade_min, fade_max) );
    
    // chose new to fade in that ISNT fadeout
    while(fadeOut==fadeIn)    fadeIn = random(PIXELS);
    
    // fade to new color
    rr = random(80, 200);
    gg = random( rr/5, rr/3);
    myStrip[fadeIn].fadeTo( rr, gg, 0, random(fade_min, fade_max) );
    
    // * and random sleep
    // for debug anti-bricking
    //delay(random(300,800) );
    // [ 120, 250, 500, 1, 2, 4 ]
    random_sleep( 0, 3);
    
    // * Eo none
  }
  else
  {
    // * If NOT none then we just faded something so just continue
    NeoPixel.show();
    
    // Eo else none
  }
  
    // * period determines the framerate and thus the  a period of time (in milliseconds).
  //delay(della); 
  snooze(SLEEP_120MS);  // _120MS _250MS, _1S
  
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

/*
    Fireflz first test, now making fireFLIES with arraz

    NEXT : change completely. my pixel class is memory limited so only 3 pixels ofr 3 flies necessary...
    
*/


#include <Adafruit_NeoPixel.h>
#include "Pixel.h"
#include "LowPower.h"


#ifdef __AVR__
  #include <avr/power.h>
  #endif


#define PIN         4
#define PIXELS      15

#define DEBUG   1
#define __D     if(DEBUG)

Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

period_t random_sleep(){
  
  int coin = random(3); 
  // 3= up to 2s
  
  __D Serial.print("Random sleep for " ); 
  __D Serial.println(coin ); 
  
  // SLEEP_120MS,   LEEP_250MS,  SLEEP_500MS,  SLEEP_1S,  SLEEP_2S,  SLEEP_4S,  SLEEP_8S,
  
  if(coin==0)
    return SLEEP_500MS;
    
  else if(coin==1)
    return SLEEP_1S;
    
  else if(coin==2)
    return SLEEP_2S;
    
  else if(coin==3)
    return SLEEP_4S;
    
  else if(coin==4)
    return SLEEP_8S;
}


void bezier( uint8_t led0, uint8_t led1, uint8_t col0, uint8_t col1){

  
  for( uint8_t d=0; d<(led1-led0); d++){
    
    
  }
}
/*  create a pixel instance for each pixel on the neopixel strip (Adafruit_NeoPixel object) 
 *    that you want to control with fade. When creating instance, simply give 
 *    integer of position in pixel strip. 
 */
Pixel myStrip[PIXELS];

int delayval = 1000; // delay for half a second
char buff[50];

#define FLIES       1
#define green_min   40 
#define green_max   80
#define blue_max    4

#define interval      5
#define fadeIn_min    180
#define fadeIn_max    400
#define fadeOut_min   180
#define fadeOut_max   400

#define delay_min     1000
#define delay_max     2000

int flyID[FLIES];
uint8_t flying [FLIES];
uint8_t landing [FLIES];



void setup() {
  
  randomSeed(analogRead(0));

  for( int f=0; f<FLIES; f++){
    flying[f] = 0;
    landing[f] = 0;
  }

  
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
    myStrip[x].set( x, 0, 0 );
    draw_pixel( (&myStrip[x]) );
  }
  NeoPixel.show();
  
  delay(1000);
  
  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x].set( 0, 0, 0 );
    draw_pixel( (&myStrip[x]) );
  }
  NeoPixel.show();
  
  // Eo setup
}




void loop() {

  
  NeoPixel.show();
  
  delay(interval); // period determines the framerate and thus the  a period of time (in milliseconds).

  // Eo **  loop
}




void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B() ) );
}


// *

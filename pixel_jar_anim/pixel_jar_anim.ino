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


#define PIN         10
#define PIXELS      6

#define DEBUG   0
#define __D     if(DEBUG)

Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(PIXELS, PIN, NEO_RGB + NEO_KHZ800);
period_t random_sleep();

/*  create a pixel instance for each pixel on the neopixel strip (Adafruit_NeoPixel object) 
 *    that you want to control with fade. When creating instance, simply give 
 *    integer of position in pixel strip. 
 */
Pixel myStrip[PIXELS];

int delayval = 1000; // delay for half a second
char buff[50];

#define FLIES       2
#define green_min   40 
#define green_max   120
#define blue_max    4

#define interval      5
#define fadeIn_min    120
#define fadeIn_max    300
#define fadeOut_min   120
#define fadeOut_max   300

int flyID[FLIES];
uint8_t flying [FLIES];
uint8_t landing [FLIES];


void setup() {
  
  randomSeed(analogRead(0));
  
  Serial.begin(115200);
  
  NeoPixel.begin(); // This initializes the NeoPixel library.
  NeoPixel.show();
  
  set_draw_all( 5, 0, 0 );
  NeoPixel.show();
  delay(1000);

  for(uint8_t f=0; f<PIXELS; f++)
  {
    myStrip[f].fadeTo( random(30), random(10), 0,  random(1,10) );
    draw_pixel( &myStrip[f] );
  }
  NeoPixel.show();
  delay(1000);

  // Eo setup
}




void loop() {

  for(uint8_t f=0; f<PIXELS; f++)
  {
    if(!myStrip[f].fade()){
      // fade to new col
      int r = random(30,60);
      int g = random(r/3);
      myStrip[f].fadeTo( r, g, 0, random(5,10) );
//      Serial.print(" new ");  
//      Serial.print(f); Serial.print(" ] :\t"); 
//      Serial.print(myStrip[f].R()); Serial.print("\t"); 
//      Serial.println(myStrip[f].G());      
    }
    else{
//      if(f==0){
//        Serial.print(" \t stp +\t");  
//        Serial.print(myStrip[f].R()); Serial.print("\t"); 
//        Serial.println(myStrip[f].G());      }
    }
    draw_pixel( &myStrip[f] );
  }
  
  NeoPixel.show();
  delay(120); 
    
//  LowPower.idle(random_sleep(0,2), ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF, 
//        TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
  
  // Eo **  loop
}




void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B() ) );
}

void set_all( int r, int g, int b){
  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x] = Pixel(x);
    myStrip[x].set( r, g, b );  
  }
}

void set_draw_all( int r, int g, int b){
  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x] = Pixel(x);
    myStrip[x].set( r, g, b );  
    draw_pixel( (&myStrip[x]) );
  }
}

void draw_all( ){
  for( int x=0; x<PIXELS ; x++ ) {
    draw_pixel( (&myStrip[x]) );
  }
}


  

// * hue (index) is a value between 0 and 767. 
// 
period_t random_sleep(int a, int b){
  
  int coin = random(a,b+1); 
  
  __D Serial.print("Random sleep for " ); 
  __D Serial.println(coin ); 
  
//  int [] dur = [ SLEEP_120MS,   LEEP_250MS,  SLEEP_500MS,  SLEEP_1S,  SLEEP_2S,  SLEEP_4S,  SLEEP_8S ];

  if(coin==0)         return SLEEP_120MS;
  else if(coin==1)    return SLEEP_250MS;
  else if(coin==2)    return SLEEP_500MS;
  else if(coin==3)    return SLEEP_1S;
  else if(coin==4)    return SLEEP_2S;
  else if(coin==5)    return SLEEP_4S;
  else if(coin==6)    return SLEEP_8S;
  else                return SLEEP_250MS;
}


//void hsb2rgbAN2(uint16_t index, uint8_t sat, uint8_t bright, uint8_t color[3]) {
//    uint8_t temp[5], n = (index &gt;&gt; 8) % 3;
//// %3 not needed if input is constrained, but may be useful for color cycling and/or if modulo constant is fast
//    uint8_t x = ((((index &amp; 255) * sat) &gt;&gt; 8) * bright) &gt;&gt; 8;
//// shifts may be added for added speed and precision at the end if fast 32 bit calculation is available
//    uint8_t s = ((256 - sat) * bright) &gt;&gt; 8;
//    temp[0] = temp[3] =              s;
//    temp[1] = temp[4] =          x + s;
//    temp[2] =          bright - x    ;
//    color[RED]  = temp[n + 2];
//    color[GREEN] = temp[n + 1];
//    color[BLUE]  = temp[n    ];
//}


// *

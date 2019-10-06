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

#define DEBUG   1
#define __D     if(DEBUG)

Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

period_t random_sleep()   {
  int coin = random(5); 
  __D Serial.print("Random sleep for " );     __D Serial.println(coin ); 
  // 250MS
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

/*  create a pixel instance for each pixel on the neopixel strip (Adafruit_NeoPixel object) 
 *    that you want to control with fade. When creating instance, simply give 
 *    integer of position in pixel strip. 
 */
Pixel myStrip[PIXELS];

int delayval = 1000; // delay for half a second
char buff[50];

#define FLIES       2
#define green_min   35 
#define green_max   100
#define blue_max    4

#define interval      5

#define fadeIn_min    180
#define fadeIn_max    400

#define fadeOut_min   180
#define fadeOut_max   400


#define fadeT_min     50
#define fadeT_max     220

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


uint8_t fly = 0;
uint8_t going_sleep = 0;

void loop() {
  
  
  /*  
   *   do animation
   *   if done animating, fade to random brightness in random time
     *   1/10 fade to 0 and sleep
     *   // after sleep, fade to random hue 
   */

  // fade and draw
  if(myStrip[ flyID[fly] ].fade())  { 
    draw_pixel( &myStrip[ flyID[fly] ]);
    NeoPixel.show();
  }
  else
  {
    // ** We're not fading!
    if(going_sleep){
      LowPower.idle( random_sleep(), ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF,
                                   TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_ON);
      going_sleep = 0;
    }
    else if(random(100) < 30) { 
      // 80% fade again
      int fadeT = random(fadeT_min, fadeT_max );
      myStrip[flyID[fly] ].fadeTo( random(green_min,green_max), 0, random(blue_max), fadeT );
    }
    else {
      // fade to 0 & then sleep
      int fadeT = random(2*fadeT_min, 2*fadeT_max );
      myStrip[flyID[fly] ].fadeTo( 0, 0, 0, fadeT );
      going_sleep = 1;
    }
    
  
  }
  
  NeoPixel.show();
  
  delay(interval); // period determines the framerate and thus the  a period of time (in milliseconds).

  // Eo loop
}







void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B() ) );
}



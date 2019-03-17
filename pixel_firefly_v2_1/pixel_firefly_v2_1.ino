/*
    version 2.1  with longer delay time for round and fewer fade cycles
    
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

Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

/*  create a pixel instance for each pixel on the neopixel strip (Adafruit_NeoPixel object) 
 *    that you want to control with fade. When creating instance, simply give 
 *    integer of position in pixel strip. 
 */
Pixel myStrip[PIXELS];

int delayval = 1000; // delay for half a second
char buff[50];

#define FLIES       1
#define green_min   40 
#define green_max   120
#define blue_max    4

#define interval      50

#define fadeIn_min    10
#define fadeIn_max    30
#define fadeOut_min   fadeIn_min
#define fadeOut_max   fadeIn_max

int flyID[FLIES];
uint8_t flying [FLIES];
uint8_t landing [FLIES];



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
    myStrip[x].set( x, 0, 0 );    draw_pixel( (&myStrip[x]) );
  }
  NeoPixel.show();
  
  delay(1000);
  
  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x].set( 0, 0, 0 );    draw_pixel( (&myStrip[x]) );
  }
  NeoPixel.show();
  
  // Eo setup
}




void loop() {

  for( uint8_t fly = 0; fly<FLIES; fly++) 
  {
      // * 3. land first to avoid succession
    if(landing[fly]){
      // fade out
      if( myStrip[flyID[fly] ].fade())  { 
        draw_pixel(&myStrip[flyID[fly] ]);
        //NeoPixel.show();
      }
      else {
        draw_pixel(&myStrip[flyID[fly] ]);
        NeoPixel.show();
        // done fading out!
        flying[fly] = 0;
        landing[fly] = 0;
        // * Random Low power sleep
        __D Serial.flush();
        LowPower.idle( random_sleep(), ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF,
                                   TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
        //
      }
      // Eo landing
    }
    
    // * 2. Fade in
    if( flying[fly] && !landing[fly] )
    {
      // theres one flying
      // fade the flying one
      if(myStrip[ flyID[fly] ].fade())  { 
        draw_pixel( &myStrip[ flyID[fly] ]);
      }
      else{
        // its done fading
        int rampDown = random(fadeOut_min, fadeOut_max);
        myStrip[ flyID[fly] ].fadeTo( 0,0,0, rampDown);
        landing[fly] = 1;
        __D sprintf(buff,"\tlanding firefly in %d ", rampDown );
        __D Serial.print(buff);
      }
  
      // Eo flying 
    }
    
      // * 1. create a new fly
    if( !flying[fly] ){
      // create a new fly
      flyID[fly] = random(PIXELS);
      int rampUp = random(fadeIn_min,fadeIn_max);
      myStrip[flyID[fly] ].fadeTo( random(green_min,green_max), 0, random(blue_max), rampUp);
      flying[fly] = 1;
      landing[fly] = 0;
      __D sprintf(buff," # New Firefly #%d , ramp : %d ", flyID[fly], rampUp );
      __D Serial.print(buff);
    }
    // Eo for each fly
  }

  NeoPixel.show();


    // * period determines the framerate and thus the  a period of time (in milliseconds).
  delay(interval); 
//  LowPower.idle( SLEEP_120MS, ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF,
//                                   TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_ON);
        

  // Eo **  loop
}




void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B() ) );
}


// *

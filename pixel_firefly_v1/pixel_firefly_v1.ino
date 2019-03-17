/*
    Fading two pixels with two instances seems to have some problems
  
*/


#include <Adafruit_NeoPixel.h>
#include "Pixel.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            4

// How many NeoPixels are attached to the Arduino?
#define PIXELS      25

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

/*  create a pixel instance for each pixel on the neopixel strip (Adafruit_NeoPixel object) 
 *    that you want to control with fade. When creating instance, simply give 
 *    integer of position in pixel strip. 
 */

Pixel myStrip[PIXELS];


int delayval = 1000; // delay for half a second
char buff[50];

float x = 50;

uint8_t st_up = 1;
uint8_t st_down = 1;
uint8_t st_adas = 0;


int intvl = 800;
int dela = 0;

char state = 'r';
uint8_t cnter = 0;
long period = 0;


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
  float interval = 1000;

  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x] = Pixel(x);
    myStrip[x].set( x, 0, 0 );
    draw_pixel( (&myStrip[x]) );
  }
  
    //  then .show() to send data-values to neopixel & display
  NeoPixel.show();
  
  delay(1000);

  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x].set( 0, 0, 0 );
    draw_pixel( (&myStrip[x]) );
  }
  
  // Eo setup
  period = millis();
}


int flyID;
uint8_t flying = 0;
uint8_t landing = 0;

void loop() {
  
    // * 3. land first to avoid succession
  if(landing){
    // fade out
    if(myStrip[flyID].fade())  { 
      draw_pixel(&myStrip[flyID]);
      NeoPixel.show();
    }
    else {
      // done fading out!
      flying = 0;
      landing = 0;
      // plus random delay
      int del = random(500,1000);
      sprintf(buff,"\t delay of : %d \n", del );
      Serial.print(buff);
      delay( del );
    }
    
    // Eo landing
  }

  // * 2. Fade in
  if(flying)
  {
    // theres one flying
    
    // fade the flying one
    if(myStrip[flyID].fade())  { 
      draw_pixel(&myStrip[flyID]);
      NeoPixel.show();
    }
    else{
      // its done fading
      int rampDown = random(80, 150);
      myStrip[flyID].fadeTo( 0,0,0, rampDown);
      landing = 1;
      sprintf(buff,"\tlanding firefly in #%d \n", rampDown );
      Serial.print(buff);
    }

    // Eo flying 
  }
  
    // * 1. create a new fly
  if(!flying){
    // create a new fly
    flyID = random(PIXELS);
    int rampUp = random(80,150);
    myStrip[flyID].fadeTo( random(60,120), 0, random(5), rampUp);
    flying = 1;
    landing = 0;
    sprintf(buff," # New Firefly #%d , ramp : %d \n", flyID, rampUp );
    Serial.print(buff);
  }

  
  delay(10); // period determines the framerate and thus the  a period of time (in milliseconds).

  // Eo loop
}


void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B() ) );
}



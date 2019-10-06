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

//char buff[40];

//Pixel P1 = Pixel(0);
//Pixel P2 = Pixel(1);

/*  create a pixel instance for each pixel on the neopixel strip (Adafruit_NeoPixel object) 
 *    that you want to control with fade. When creating instance, simply give 
 *    integer of position in pixel strip. 
 */

Pixel myStrip[PIXELS];

//Pixel P3(2);


int delayval = 1000; // delay for half a second

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

  // Eo setup
  period = millis();
}



void loop() {
  // print framerate every 50 frames
  
  NeoPixel.show();
  
  delay(dela); // period determines the framerate and thus the  a period of time (in milliseconds).

  // Eo loop
}


void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B() ) );
}



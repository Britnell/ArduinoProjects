/*
    Fading two neoPixel with two instances seems to have some problems
  
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
#define NUMPIXELS      7

// When we setup the NeoPixel library, we tell it how many neoPixel, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel neoPixel = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);

//char buff[40];

//Pixel P1 = Pixel(0);
//Pixel P2 = Pixel(1);
#define N_PIX   7

Pixel* Pixels;

int delayval = 1000; // delay for half a second

float x = 50;

int intvl = 1000;
int ring_speed = 500;
int ring_t = 0;

int dela = 0;
int ring = 1;



void setup() {
  Serial.begin(115200);
  Serial.println("BEgin - Off" );
  delay(500);
  
  neoPixel.begin(); // This initializes the NeoPixel library.

  for( int x=0; x<N_PIX; x++){
    Pixels[x] = Pixel(x);
    Pixels[x].off();
    draw_pixel( &(Pixels[x]) );
  }
  neoPixel.show();
  
  //P2.printout();
  delay(700);

  Serial.println("\t# On" );
  for( int x=0; x<N_PIX; x++){
    Pixels[x].set( 0, 60, 100, 0 );
    draw_pixel( &(Pixels[x]) );
  }
  neoPixel.show();
  delay(500);
  
  //Pixels[ring].fadeTarget( 0, 30, 100, 10, intvl);
  ring_t = millis();
  
  //Serial.print(" fading is: ");  Serial.println(P1.is_fading() );
  // setup

}



void loop() {
  uint8_t faded;

//  if( millis() -ring_t >= ring_speed){
//    ring_t = millis();
//    // light next ring elem.
//    ring++;
//    if(ring >=N_PIX)   ring = 1;
//    
//    Pixels[ring].fadeTarget(0, 30, 100, 10, intvl);
//    
//  }

  for( int x=0; x<N_PIX; x++){
    faded = Pixels[x].fade();
    draw_pixel( &Pixels[x] );
  }
  
//  draw_pixel( &P3 );
  //P2.printout();
  
  neoPixel.show();
  
  

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of neoPixel minus one.

  // neoPixel.Color takes RGB values, from 0,0,0 up to 255,255,255
  
  delay(dela); // Delay for a period of time (in milliseconds).


}


void draw_pixel( Pixel * PIX ) {
  neoPixel.setPixelColor(PIX->ID(), neoPixel.Color(  PIX->G(), PIX->R(), PIX->B(), PIX->W() ) );
}



/*
    Fireflz first test, now making fireFLIES with arraz

    NEXT : change completely. my pixel class is memory limited so only 3 pixels ofr 3 flies necessary...
    
*/


#include <Adafruit_NeoPixel.h>
#include "Pixel.h"

#define PIN         2
#define PIXELS      16

#define DEBUG   0
#define __D     if(DEBUG)

Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

Pixel myStrip[PIXELS];

int delayval = 1000; // delay for half a second
char buff[50];


int pos = 0;

int colR = 40;
int colG = 40;
int colB = 60;


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
    myStrip[x].set( 0, 0, 0 );
    draw_pixel( (&myStrip[x]) );
  }
  NeoPixel.show();
  
  delay(100);
  
  //myStrip[pos].fadeTo(10,10,60,20);
  myStrip[pos].set(colR, colG, colB);
    draw_pixel( (&myStrip[pos]) );
  NeoPixel.show();
  //NeoPixel.setPixelColor(pos, NeoPixel.Color(  colR, colG, colB ) );
  delay(1000);
  // Eo setup
}


void loop(){
  //NeoPixel.setPixelColor(pos, NeoPixel.Color(  0, 0, 0 ) );
  pos -= 2;
  if(pos<0)   pos += PIXELS;
  myStrip[pos].set(0,0,0);
    draw_pixel( (&myStrip[pos]) );

  //NeoPixel.setPixelColor(pos, NeoPixel.Color(  0, 0, 0 ) );
  pos++;
    if(pos>=PIXELS)    pos = 0;
  myStrip[pos].set(colR, colG, colB);
    draw_pixel( (&myStrip[pos]) );
  pos++;
    if(pos>=PIXELS)    pos = 0;
  myStrip[pos].set(colR, colG, colB);
    draw_pixel( (&myStrip[pos]) );
  pos++;
    if(pos>=PIXELS)    pos = 0;
  //NeoPixel.setPixelColor(pos, NeoPixel.Color(  0, 0, 0 ) );
  myStrip[pos].set(colR, colG, colB);
    draw_pixel( (&myStrip[pos]) );
  
    
  NeoPixel.show();
  delay(800);
}


void loop1() {

  for( int x=0; x<PIXELS ; x++ )
  if( myStrip[x].fade())  { 
    draw_pixel( (&myStrip[x]) );
  }
  NeoPixel.show();
  
  if( !myStrip[pos].fade())  { 
    // were done fading
    sprintf(buff, "Done fading %d ON, now ON and fading %d ON ", pos, pos+1);    Serial.println(buff);
    myStrip[pos].fadeTo(0,0,0,   20);
    pos++;      
    if(pos >=PIXELS)   pos = 0;
    myStrip[pos].fadeTo(10,10,10, 20);    
  }
      
  delay(10); // period determines the framerate and thus the  a period of time (in milliseconds).

  // Eo **  loop
}




void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B() ) );
}


// *

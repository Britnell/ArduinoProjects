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
#define NUMPIXELS      7

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);

//char buff[40];

//Pixel P1 = Pixel(0);
//Pixel P2 = Pixel(1);

Pixel B_Up(4);
Pixel B_Down(1);
Pixel B_ADAS(0);

//Pixel P3(2);


int delayval = 1000; // delay for half a second

float x = 50;

uint8_t st_up = 1;
uint8_t st_down = 1;
uint8_t st_adas = 0;



void setup() {
  Serial.begin(115200);

  NeoPixel.begin(); // This initializes the NeoPixel library.
  
  Serial.println("off");
  
  B_Up.set(50, 0, 0, 0);
  B_Down.set(0, 0, 50, 0);
  B_ADAS.set(0,0,0, 0);
  
  draw_pixel( &B_Up );
  draw_pixel( &B_Down );
  draw_pixel( &B_ADAS );
  
  NeoPixel.show();
  delay(500);
  
  //Serial.print(" fading is: ");  Serial.println(P1.is_fading() );
  // setup

}

uint8_t col = 0;
int intvl = 800;
int dela = 0;



void loop() {
  char cmd;
  
  if( Serial.available() > 0){
    cmd = Serial.read();

    if( cmd == 'a'){
        // adas
        if(st_adas){
          st_adas = 0;
          B_ADAS.fadeTo(0,0,0, 0, intvl);
          draw_pixel( &B_ADAS );
        }
        else{
          st_adas = 1;
          B_ADAS.fadeTo(0, 255, 255, 5, intvl);
          draw_pixel( &B_ADAS );
        }
        // Eo 'a'
      }
      else if(cmd == 'u'){
      if(st_up){
        st_up = 0;
        B_Up.fadeTo(0,0,0, 0, intvl);
        draw_pixel( &B_Up );
      }
      else {
        st_up = 1;
        B_Up.fadeTo(15,0,0, 40, intvl);
        draw_pixel( &B_Up );
      }
      // Eo 'u'
  }
  else if(cmd == 'd'){
      if(st_down){
        st_down = 0;
        B_Down.fadeTo(0,0,0, 0, intvl);
        draw_pixel( &B_Down );
      }
      else {
        st_down = 1;
        B_Down.fadeTo(0,0,10, 255, intvl);
        draw_pixel( &B_Down );
      }
      // Eo 'u'
    }
    else if(cmd== '0' || cmd=='x' ){
      B_Up.fadeTo(0,0,0, 0, intvl);
      B_Down.fadeTo(0,0,0, 0, intvl);
      B_ADAS.fadeTo(0,0,0, 0, intvl);
        draw_pixel( &B_Up );
        draw_pixel( &B_Down );
        draw_pixel( &B_ADAS );
      st_down = 0;
      st_up = 0;
      st_adas = 0;
    }
    else if(cmd== '1' ){
      B_Up.fadeTo(15,0,0, 40, intvl);
      B_Down.fadeTo(0,0,10, 40, intvl);
      B_ADAS.fadeTo(0,60,60, 5, intvl);
        draw_pixel( &B_Up );
        draw_pixel( &B_Down );
        draw_pixel( &B_ADAS );
      st_down = 1;
      st_up = 1;
      st_adas = 1;
    }
    // Eo serial available
  }
  

  if(B_Up.fade()) draw_pixel(&B_Up);
  if(B_Down.fade()) draw_pixel(&B_Down);
  if(B_ADAS.fade()) draw_pixel(&B_ADAS);
  
//  Serial.println("P1");
//  f = P1.fade();
//      P1.fadeTo( 40, 0, 0, 80, intvl);
//  draw_pixel( &P1 );

  
  NeoPixel.show();
  

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  
  delay(dela); // Delay for a period of time (in milliseconds).
  
}


void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B(), PIX->W() ) );
}



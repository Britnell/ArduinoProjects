/**
   Simple animation demo for using the NoePixel painter library
   This demo does the Knight Rider scanner effect with just a few lines of code
   The speed and fadespeed need to be adjusted for different processor speeds
   Chosen settings work nicely on 60-pixels and an Arduino Duemilenove
*/

#define NUMBEROFPIXELS 6 //Number of LEDs on the strip
#define PIXELPIN 10 //Pin where WS281X LED strip data-line is connected

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include <NeoPixelPainter.h>


Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(NUMBEROFPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

//create one canvas and one brush with global scope
NeoPixelPainterCanvas pixelcanvas = NeoPixelPainterCanvas(&neopixels); //create canvas, linked to the neopixels (must be created before the brush)
NeoPixelPainterBrush pixelbrush = NeoPixelPainterBrush(&pixelcanvas); //crete brush, linked to the canvas to paint to

HSV brushcolor;
HSV backG;
RGB backRGB;

int8_t hueMax = 25;
int8_t hueMin = -2;

void setup() {

  pinMode(PIXELPIN, OUTPUT);

  neopixels.begin();

  Serial.begin(115200);

  brushcolor.h = random(hueMax); //zero is red in HSV. Library uses 0-255 instead of 0-360 for colors (see https://en.wikipedia.org/wiki/HSL_and_HSV)
  brushcolor.s = 255; //full color saturation
  brushcolor.v = 120; //about half the full brightness

  backG.h = 0;
  backG.s = 255;
  backG.v = 6;
  
  //backRGB = pixelcanvas.HSVtoRGB( backG.h, backG.s, backG.v);
  backRGB.r=4;  backRGB.g=1; backRGB.b=0;
  
  pixelbrush.setSpeed(0); //set the brush movement speed (4096 means to move one pixel per update)
  pixelbrush.setColor(brushcolor); //set the brush color
  pixelbrush.setFadeSpeed(10); //fading speed of pixels (255 max, 200 is fairly fast)
  pixelbrush.setFadein(true);
  pixelbrush.setFadeout(true); //do brightness fadeout after painting
  pixelbrush.setBounce(true); //bounce the brush when it reaches the end of the strip

}


void loop() {
  if( rand() %300 == 0 )
  {
    uint8_t hu = random(hueMin,hueMax);
    if(hu<0)    hu += 256;
    brushcolor.h = hu;
    brushcolor.s = 245 + random(10);
    brushcolor.v = 50 + random(50);
    pixelbrush.setColor(brushcolor);
    pixelbrush.moveTo(random(NUMBEROFPIXELS));
    pixelbrush.setFadeSpeed(random(5) + 1); 
    Serial.println(hu);
  }
  
  //  neopixels.clear(); 
  // background instead of clear
  for (uint8_t i = 0; i < NUMBEROFPIXELS; i++)
  {
    neopixels.setPixelColor(i, backRGB.r,backRGB.g,backRGB.b); //color in RGB: dark blue
  }

  pixelbrush.paint(); 
  pixelcanvas.transfer(); 
  neopixels.show();
  
  // **
}


//  if(rand()%100==0){
//    // background hue
//    int bH = backG.h;
//    if(bH<=0)
//      bH += random(0,2);
//    else if(bH>=10)
//      bH -= random(0,2);
//    else 
//      bH += random(-1,2);
//    backG.h = bH;
//    //backRGB = pixelcanvas.HSVtoRGB( backG.h, backG.s, backG.v);
//    //    Serial.println(backG.h);
//  }

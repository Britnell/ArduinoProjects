/*
 *  Bike indicator light for L R
 *   with cap touch two buttons for L and R
 * 
 */
 

#define NUMPIXELS 15 //Number of LEDs on the strip
#define PIXELPIN 4 //Pin where WS281X LED strip data-line is connected

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include <NeoPixelPainter.h>
#include <ADCTouch.h>


#define touchL  A0
#define touchR  A1

#define b_speed 500
#define b_fade  110
#define blinkLength 4000

int refL, refR;

Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

//create one canvas and one brush with global scope
NeoPixelPainterCanvas pixelcanvas = NeoPixelPainterCanvas(&neopixels); //create canvas, linked to the neopixels (must be created before the brush)
NeoPixelPainterBrush pixelbrushL = NeoPixelPainterBrush(&pixelcanvas); //crete brush, linked to the canvas to paint to
NeoPixelPainterBrush pixelbrushR = NeoPixelPainterBrush(&pixelcanvas); //crete brush, linked to the canvas to paint to
NeoPixelPainterBrush glowbrush = NeoPixelPainterBrush(&pixelcanvas); //crete brush, linked to the canvas to paint to



HSV brushcolorL;
HSV brushcolorR;
HSV brushcolorG;
HSV offcolor;



void setup() {

  brushcolorL.h = 8; //zero is red in HSV. Library uses 0-255 instead of 0-360 for colors (see https://en.wikipedia.org/wiki/HSL_and_HSV)
  brushcolorL.s = 255; //full color saturation
  brushcolorL.v = 125; //about half the full brightness
  
  offcolor.h=0;
  offcolor.s=0;
  offcolor.v=0;

        // OTHER SIDE
  brushcolorR.h = 230; //zero is red in HSV. Library uses 0-255 instead of 0-360 for colors (see https://en.wikipedia.org/wiki/HSL_and_HSV)
  brushcolorR.s = 255; //full color saturation
  brushcolorR.v = 125; //about half the full brightness
  //

  pinMode(PIXELPIN, OUTPUT);

  refL = ADCTouch.read(A0,500);
  refR = ADCTouch.read(A1,500);
  
  neopixels.begin();
  
  Serial.begin(115200);
  Serial.println(" ");
  Serial.println("NeoPixel Painter simple demo");

  //check if ram allocation of brushes and canvases was successful (painting will not work if unsuccessful, program should still run though)
  //this check is optional but helps to check if something does not work, especially on low ram chips like the Arduino Uno
  
  //initialize the animation, this is where the magic happens:
  
  
          // ONE SIDE
  pixelbrushL.setColor(brushcolorL); //set the brush color
  pixelbrushL.setSpeed(b_speed); //set the brush movement speed (4096 means to move one pixel per update)
  pixelbrushL.setFadeSpeed(b_fade); //fading speed of pixels (255 max, 200 is fairly fast)
  pixelbrushL.setFadein(true);  //-
  pixelbrushL.setFadeout(true); //do brightness fadeout after painting
  pixelbrushL.setBounce(false); //bounce the brush when it reaches the end of the strip

  
  pixelbrushR.setColor(brushcolorR); //set the brush color
  pixelbrushR.setSpeed(-b_speed); //set the brush movement speed (4096 means to move one pixel per update)
  pixelbrushR.setFadeSpeed(b_fade); //fading speed of pixels (255 max, 200 is fairly fast)
  pixelbrushR.setFadein(true);  //-
  pixelbrushR.setFadeout(true); //do brightness fadeout after painting
  pixelbrushR.setBounce(false); //bounce the brush when it reaches the end of the strip

  brushcolorG.H = 0;
  brushcolorG.S = 255;
  brushcolorG.V = 125;
  
  glowbrush.setColor(brushcolorG);
  glowbrush.setSpeed(0);
  glowbrush.setFadeSpeed(80);
  glowbrush.setFadein(true);
  glowbrush.setFadeout(true);
}

long t_blinkingL = 0;
long t_blinkingR = 0;
uint8_t bounceR = 0;
uint8_t bounceL = 0;
  
char state = 'b';
// b - backlgight
// l - left indicator
// r - right indicator


void loop() {
  int valL, valR;

  valL = ADCTouch.read(touchL,10);
  valR = ADCTouch.read(touchR,10);
  valL -= refL;
  valR -= refR;
  
  if( valL > 50){
    
    if(!bounceL){
      if( millis() -t_blinkingL > blinkLength ){
        pixelcanvas.clear();
        pixelbrushL.moveTo(0);
        if( millis() -t_blinkingR < blinkLength ){
          t_blinkingR -= blinkLength;
        }
      }
      bounceL = 1;
    }
    t_blinkingL = millis();  
    // if L
  }
  else{
    bounceL = 0;
  }

  if( valR > 50){
    
    if(!bounceR){
      if( millis() -t_blinkingR > blinkLength ){
        pixelcanvas.clear();
        pixelbrushR.moveTo(NUMPIXELS-1);
        if( millis() -t_blinkingL < blinkLength ){
          t_blinkingL -= blinkLength;
        }
      }
      bounceR = 1;
    }
    t_blinkingR = millis();  
  }
  else{
    bounceR = 0;
  }

  if(  millis() -t_blinkingL > blinkLength &&  millis() -t_blinkingR > blinkLength ){
    // 
    state = 'b';
    glowbrush.clear();
    glowbrush.moveTo(0)
    glowbrush.moveTo(NUMPIXELS);

    //
  }
  neopixels.clear(); //always need to clear the pixels, the canvas' colors will be added to whatever is on the pixels before calling an canvas update

  if( millis() -t_blinkingL < blinkLength ){
    pixelbrushL.paint(); //paint the brush to the canvas (and update the brush, i.e. move it a little)
    pixelcanvas.transfer(); //transfer the canvas to the neopixels  
  }
  
  if( millis() -t_blinkingR < blinkLength ){
    pixelbrushR.paint(); //paint the brush to the canvas (and update the brush, i.e. move it a little)
    pixelcanvas.transfer(); //transfer the canvas to the neopixels  
  }
  
  
  neopixels.show();
  // Eo loop
}



#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include <NeoPixelPainter.h>


#define NUMBEROFPIXELS 15  //Number of LEDs on the strip
#define PIXELPIN 4      //Pin where WS281X pixels are connected

Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(NUMBEROFPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

NeoPixelPainterCanvas pixelcanvas = NeoPixelPainterCanvas(&neopixels); //create canvas, linked to the neopixels (must be created before the brush)
NeoPixelPainterBrush brush1= NeoPixelPainterBrush(&pixelcanvas); //crete brush, linked to the canvas to paint to

long framerate;

uint8_t initialise = 1;

void setup() {
  randomSeed(analogRead(0)); //new random seed 
  pinMode(PIXELPIN, OUTPUT);

  neopixels.begin();

  Serial.begin(115200);
  Serial.println(" ");
  Serial.println("NeoPixel Painter Demo");

  //check if ram allocation of brushes and canvases was successful (painting will not work if unsuccessful, program should still run though)
  //this check is optional but helps to check if something does not work, especially on low ram chips like the Arduino Uno
  if (pixelcanvas.isvalid() == false) Serial.println("canvas allocation problem");
  else  Serial.println("canvas allocation ok");


  if (brush1.isvalid() == false) Serial.println("brush allocation problem");
  else  Serial.println("brush allocation ok");
  
  framerate = millis();
}


  // **   Speed up and limit to part of strip 0 to 50
void effect1(){
  //-
  if(initialise){
      brush1.setSpeed(600);
      //brush1.setSpeedlimit(3000);
      brush1.setFadeSpeed(180);
      brush1.setFadein(false);
      brush1.setFadeout(true);
      brush1.setBounce(false);
      HSV brushcolor;
      brushcolor.h = 10;
      brushcolor.s = 255;
      brushcolor.v = 80;
      brush1.setColor(brushcolor);
      initialise = 0;
  }
  
  if( interval(5) ){
    int s =  brush1.getSpeed();
    if( s < 2000 ){
      brush1.setSpeed( s + 0x0F );  
      Serial.println(s);
    }
  }
  // Eo func
}


  // **   set Speed(0) and move manually
void effect2(){
  //-
  if(initialise){
      brush1.setSpeed(600);
      //brush1.setSpeedlimit(3000);
      brush1.setFadeSpeed(180);
      brush1.setFadein(true);
      brush1.setFadeout(true);
      brush1.setBounce(false);
      HSV brushcolor;
      brushcolor.h = 5;
      brushcolor.s = 255;
      brushcolor.v = 80;
      brush1.setColor(brushcolor);
      initialise = 0;
  }
  
  if( interval(5) ){
    int s =  brush1.getSpeed();
    if( s < 2000 ){
      brush1.setSpeed( s + 0x0F );  
      Serial.println(s);
    }
  }
  if( brush1.getPosition() >= 50 ){
    brush1.moveTo(0);
  }
  // Eo func
}


void loop() {
  step_intvl();

  //effect1();

  effect2();
  
//  ** Check framerate
//  if( interval(10) ) {
//    Serial.print("framerate = ");    Serial.println( (millis()-framerate ) / 10 );
//    framerate = millis();
//  }
  
  neopixels.clear();        // clear
  brush1.paint();           // animate
  pixelcanvas.transfer();   // canvas 
  neopixels.show();         // pixels
}


int intvl = 0;
uint8_t interval( int freq){
  
  if( intvl % freq == 0){
    //intvl = 0;
    return 1;
  }
  else
    return 0;
}

void step_intvl(){
  intvl++;
}


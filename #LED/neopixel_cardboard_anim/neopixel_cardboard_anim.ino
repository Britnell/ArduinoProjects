/**
Demo-set of animations using the NoePixel painter library
The demo animations show different ways the library can be used
to create colorful led-strip animations with just a few lines of code.

These examples are written to run on an Arduino Duemilenove. For
other Arduino platforms, speeds may need to be adjusted to give the same effect
Also, on platforms with more available RAM (like the ESP8266), more
canvases can be used to paint even more mixing colors.

If you come up with other good animations you want to share, please
post the code on the GitHub repository by opening a new issue. 
I will then add it as a demo (with proper attribution).
*/

const int duration = 2000; //number of loops to run each animation for

#define NUMBEROFPIXELS 25 //Number of LEDs on the strip
#define PIXELPIN 4 //Pin where WS281X pixels are connected

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include <NeoPixelPainter.h>


Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(NUMBEROFPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

NeoPixelPainterCanvas pixelcanvas = NeoPixelPainterCanvas(&neopixels); //create canvas, linked to the neopixels (must be created before the brush)
NeoPixelPainterBrush pixelbrush = NeoPixelPainterBrush(&pixelcanvas); //crete brush, linked to the canvas to paint to


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


	if (pixelbrush.isvalid() == false) Serial.println("brush allocation problem");
	else  Serial.println("brush allocation ok");

}

unsigned long loopcounter; //count the loops, switch to next animation after a while
bool initialized = false; //initialize the canvas & brushes in each loop when zero
int huer=0;
int hue_min = 0;
int hue_max = 20;
void loop() {


	//SPARKLER: a brush seeding sparkles

		HSV brushcolor;
        // A - slow moving
    // pixel (15) to 25
    // Speed 30
    // fadeSpeed random(50)+20
        // B - more sparkling
    // pixels 25
    // speed 80
    // fadespeed   40 +random(100)
    
		if (initialized == false)
		{
			initialized = true;
			pixelbrush.setSpeed(80);  // 1st 30 slower 20
			pixelbrush.setFadeout(true); //sparkles fade in
			pixelbrush.setFadein(true);  //and fade out immediately after reaching the set brightness
		}
    
    brushcolor.h = random(20);
		brushcolor.s = 255; //random(130); //random but low saturation, giving white-ish sparkles
		brushcolor.v = 255;  //random(200); //random (peak) brighness

		pixelbrush.setColor(brushcolor);
		pixelbrush.setFadeSpeed(40+random(100) ); // (20)+30 // (30)+20 // (50)+20

		//neopixels.clear();
    for(int p=0; p<NUMBEROFPIXELS;p++){
      neopixels.setPixelColor(p, neopixels.Color(4,0,0) );
    }
    
		pixelbrush.paint(); //paint the brush to the canvas (and update the brush, i.e. move it a little)
		pixelcanvas.transfer(); //transfer (add) the canvas to the neopixels

		neopixels.show();

  // Eo sparkler



}//end of loop()

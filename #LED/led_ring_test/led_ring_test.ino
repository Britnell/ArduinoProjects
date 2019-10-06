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

const int duration = 10000; //number of loops to run each animation for

#define NUMBEROFPIXELS 24 //Number of LEDs on the strip
#define PIXELPIN 10 //Pin where WS281X pixels are connected

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include <NeoPixelPainter.h>


Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(NUMBEROFPIXELS, PIXELPIN, NEO_RGBW + NEO_KHZ800);

NeoPixelPainterCanvas pixelcanvas = NeoPixelPainterCanvas(&neopixels); //create canvas, linked to the neopixels (must be created before the brush)
NeoPixelPainterBrush pixelbrush = NeoPixelPainterBrush(&pixelcanvas); //crete brush, linked to the canvas to paint to


void setup() {
	randomSeed(analogRead(0)); //new random seed 
	pinMode(PIXELPIN, OUTPUT);

	neopixels.begin();

//	Serial.begin(115200);
//	Serial.println(" ");
//	Serial.println("NeoPixel Painter Demo");

	//check if ram allocation of brushes and canvases was successful (painting will not work if unsuccessful, program should still run though)
	//this check is optional but helps to check if something does not work, especially on low ram chips like the Arduino Uno
//	if (pixelcanvas.isvalid() == false) Serial.println("canvas allocation problem");
//	else  Serial.println("canvas allocation ok");


//	if (pixelbrush.isvalid() == false) Serial.println("brush allocation problem");
//	else  Serial.println("brush allocation ok");

}

unsigned long loopcounter; //count the loops, switch to next animation after a while
bool initialized = false; //initialize the canvas & brushes in each loop when zero

void loop() {  
  
	//SPARKLER: a brush seeding sparkles
  // much slower
  initialized = false;

	for(loopcounter = 0; loopcounter<duration; loopcounter++) 
	{
		HSV brushcolor;

		if (initialized == false)
		{
			initialized = true;
			pixelbrush.setSpeed(100);
			pixelbrush.setFadeout(true); //sparkles fade in
			pixelbrush.setFadein(true);  //and fade out immediately after reaching the set brightness
		}

		//set a new brush color in each loop
		brushcolor.h = random(255); //random color
		brushcolor.s = random(130); //random but low saturation, giving white-ish sparkles
		brushcolor.v = random(100); //random (peak) brighness

		pixelbrush.setColor(brushcolor);
		pixelbrush.setFadeSpeed(random(20)+30  ); //set a new fadespeed with some randomness

		neopixels.clear();

		pixelbrush.paint(); //paint the brush to the canvas (and update the brush, i.e. move it a little)
		pixelcanvas.transfer(); //transfer (add) the canvas to the neopixels

		neopixels.show();
	}



	initialized = false; //reset the variable before moving to the next loop
  
	//---------------------
  //TWINKLY STARS
  //---------------------
  //brush set to random positions and painting a fading star
	for(loopcounter = 0; loopcounter<duration; loopcounter++) 
	{

		HSV brushcolor;

		if (initialized == false)
		{
			initialized = true;
			pixelbrush.setSpeed(0); //do not move automatically
			pixelbrush.setFadein(true); //fade in 
			pixelbrush.setFadeout(true); //and fade out
		}


		if (rand() % 150 == 0) //at a random interval, move the brush to paint a new pixel (brush only paints a new pixel once)
		{
			brushcolor.h = rand();
			brushcolor.s = random(40); //set low saturation, almost white
			brushcolor.v = random(100) + 20; //set random brightness
			pixelbrush.setColor(brushcolor);
			pixelbrush.moveTo(random(NUMBEROFPIXELS)); //move the brush to a new, random pixel
			pixelbrush.setFadeSpeed(random(20) + 10); //set random fade speed, minimum of 5
		}

		//add a background color by setting all pixels to a color (instead of clearing all pixels):
		int i;
		for (i = 0; i < NUMBEROFPIXELS; i++)
		{
			neopixels.setPixelColor(i, 0, 0, 4, 0); //color in RGB: dark blue
		}


		pixelbrush.paint(); //paint the brush to the canvas 
		pixelcanvas.transfer(); //transfer (add) the canvas to the neopixels

		neopixels.show();
	}

  //end of loop()
}


// # 

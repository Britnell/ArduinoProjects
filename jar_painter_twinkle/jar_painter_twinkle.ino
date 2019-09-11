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

#define NUMBEROFPIXELS 6 //Number of LEDs on the strip
#define PIXELPIN 10 //Pin where WS281X pixels are connected

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

void twinkly_stars()
{
  //---------------------
  //TWINKLY STARS
  //---------------------
  //brush set to random positions and painting a fading star
  uint8_t initialized = false;
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


    if (rand() % 100 == 0) //at a random interval, move the brush to paint a new pixel (brush only paints a new pixel once)
    {
      brushcolor.h = rand();
      brushcolor.s = random(40); //set low saturation, almost white
      brushcolor.v = random(200) + 20; //set random brightness
      pixelbrush.setColor(brushcolor);
      pixelbrush.moveTo(random(NUMBEROFPIXELS)); //move the brush to a new, random pixel
      pixelbrush.setFadeSpeed(random(10) + 5); //set random fade speed, minimum of 5
    }

    //add a background color by setting all pixels to a color (instead of clearing all pixels):
    int i;
    for (i = 0; i < NUMBEROFPIXELS; i++)
    {
      neopixels.setPixelColor(i, 1, 0, 6); //color in RGB: dark blue
    }


    pixelbrush.paint(); //paint the brush to the canvas 
    pixelcanvas.transfer(); //transfer (add) the canvas to the neopixels

    neopixels.show();
  }
}


void hue_fader()
{
  //------------------------------
  //HUE FADER: demo of hue fading
  //------------------------------
  
  //hue fading can be done in two ways: change the color moving the shortest distance around the colorwheel (setFadeHueNear)
  //or intentionally moving around the colorwheel choosing the long way (setFadeHueFar)
  //two brushes move along the strip in different speeds, each painting a different color that the canvas will then fade to
  //a new color is set when the first brush passes pixel 0
  //both brushes act on the same canvas
  
  while(true) //create a loop with an additional brush (is deleted automatically once the loop finishes)
  {

    //create an additional brush, painting on the same canvas as the globally defined brush
    NeoPixelPainterBrush pixelbrush2 = NeoPixelPainterBrush(&pixelcanvas); 

    if (pixelbrush2.isvalid() == false) Serial.println(F("brush2 allocation problem"));
    else  Serial.println(F("brush2 allocation ok"));

     pixelcanvas.clear(); //clear the canvas

    for(loopcounter = 0; loopcounter<duration; loopcounter++) 
    {

      static unsigned int lastposition = 0; //to detect zero crossing only once (brush may stay at pixel zero for some time since it uses sub-pixel resolution movement)

      if (pixelbrush.getPosition() == 0 && lastposition > 0) initialized = false; //choose new color & speed if brush reaches pixel 0

      lastposition = pixelbrush.getPosition(); //save current position for next position check

      if (initialized == false)
      {
        initialized = true;

        HSV brushcolor;
   

        brushcolor.h = random(50); //random color
        brushcolor.s = 255; //full saturation
        brushcolor.v = 255; //medium brightness

        pixelbrush.setSpeed(random(10) + 10); //random movement speed
        pixelbrush.setFadeSpeed(random(10) + 10); //set random fading speed
        pixelbrush.setColor(brushcolor); //update the color of the brush
        pixelbrush.setFadeHueNear(true); //fade using the near path on the colorcircle
        
        //second brush paints on the same canvas
        brushcolor.h = random(255);
        pixelbrush2.setSpeed(random(150) + 150);
        pixelbrush2.setFadeSpeed(random(10) + 20);
        pixelbrush2.setColor(brushcolor);
        pixelbrush2.setFadeHueNear(true); //fade using the near path on the colorcircle
        //pixelbrush.setFadeHueFar(true); //fade using the far path on the colorcircle (if both are set, this path is chosen)
        pixelbrush2.setBounce(true); //bounce this brush at the end of the strip
      }

      neopixels.clear();

      pixelbrush.paint(); //apply the paint of the first brush to the canvas (and update the brush)
      pixelbrush2.paint(); //apply the paint of the second brush to the canvas (and update the brush)
      pixelcanvas.transfer(); //transfer the canvas to the neopixels

      neopixels.show();
    }
    break; //quit the while loop immediately (and delete the created brush)
  }

}

void loop() {

	//main program loops through different animations, allocating addidional canvas and brushes if needed

  initialized = false;
  
//  twinkly_stars();

  hue_fader();

  // * Eo loop
}

// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

#define NUMPIXELS 64 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    2
#define CLOCKPIN   3
#define pln(x)  Serial.println(x)
#define pt(x)  Serial.print(x)

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup() {
  
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  
  randomSeed(analogRead(0));
  
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int      head  = 0, tail = -2; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

void loop() {
  int8_t ran;
  uint32_t col;
  int8_t RGorB;

  // change 3 at once
  for(uint8_t x=0; x<1; x++)
  {
      ran = random(NUMPIXELS);  // chose random pixel
      RGorB = random(3)*8;  // chose r g or b
      
      //col = strip.getPixelColor(ran); // get current col
      //col &= ~( 0xFF << RGorB );  // clear 2 pixels of that color
      //col |= ( random(256) << RGorB );
      
      col = ( random(256) << RGorB );
      strip.setPixelColor(ran, col ); 
  }
  strip.show();                     // Refresh strip
  delay(20);                        // Pause 20 milliseconds (~50 FPS)
  
}

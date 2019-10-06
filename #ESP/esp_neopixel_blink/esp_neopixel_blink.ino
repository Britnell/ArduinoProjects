// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>

#define PIN            D2
#define NUMPIXELS      24

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);

int delayval = 500; // delay for half a second

uint8_t r,g,b, w;


void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  r = 0; 
  g = 85;
  b = 170;
  w = 0;
}


void loop() {
  static int schritt = 1;
  static int temp;
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  r += schritt;    // if(r>255)     r -= 256;
  g += schritt;    // if(g>255)     g -= 256;
  b += schritt;    // if(b>255)     b -= 256;
  
  
  // pixels 0 to 12 = green
  for(int i=0;i<NUMPIXELS;i++){
    if(i<8){
      pixels.setPixelColor(i, pixels.Color( (r +30 *i), 0, 0, w ) );     
    }
    else if(i<16){
      pixels.setPixelColor(i, pixels.Color(0, (g +30 *(i-8)), 0, w ) ); 
    }
    else {
      pixels.setPixelColor(i, pixels.Color(0, 0, (b +30 *(i-16)), w ) ); 
    }

    // Eo for
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  // Eo loop
  
  delay(10);
  // 10 = 100 Hz
}

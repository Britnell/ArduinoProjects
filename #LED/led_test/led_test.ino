// Test of neopixel strip with adafruit <NeoPixel.h>

#include <Adafruit_NeoPixel.h>

#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)

// pin D4 = PD4
#define LED_pin   4

#define PIXELS   144

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL, LED_pin, NEO_RGB + NEO_KHZ800);
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL, LED_pin, NEO_RGB +NEO_KHZ400);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, LED_pin, NEO_GRB +NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  
  
  strip.begin();
  strip.show();
  
  delay(1000);
}


uint8_t ind = 0;
uint32_t calla = 0;

void loop() {
  ride();
  
  delay(100);
}





void ride(void) 
{
  static uint8_t center = 3;
  static uint32_t col = strip.Color( 40, 10, 0);
  static uint32_t col_ = strip.Color( 20, 5, 0);
  static uint32_t col__ = strip.Color( 5, 2, 0);
  static uint32_t col___ = strip.Color( 2, 1, 0);
  static uint8_t dir = 1;

  wipe(0);
  strip.setPixelColor(center, col);
  strip.setPixelColor(center-1, col_);
  strip.setPixelColor(center-2, col__);
  strip.setPixelColor(center-3, col___);
  strip.show();
  
  if(dir) {
    // 1 = UP
    if(center < PIXELS) {
      center++;
    }
    else {
      //dir = 0;
      center = 3;
    }
  
  }
  else
  {
    if(center > 0) {
      center --;
    }
    else {
      dir = 1;
    }
  }
  // Eo ride
}
void wipe(uint32_t col)
{
  for( uint16_t i=0; i<PIXELS; i++)
  {
    strip.setPixelColor(i, col);
    //strip.show();
    // Eo for
  }
  
}


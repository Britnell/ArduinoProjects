/* 
 *  
 */
 
#include <Adafruit_NeoPixel.h>

#define PIN            2
#define NUMPIXELS      17
int frameSize = 3;

int pos;
uint8_t rgb;
uint8_t r,g,b;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
  Serial.println("hi this is arduino");
  
  pixels.begin();

  // blink
  pixels.setPixelColor(0, pixels.Color(0,150,0));
  pixels.setPixelColor(1, pixels.Color(150,0,0));
  pixels.show(); 

  pos = 0;
  rgb = 0;
  // * Eo setp
}


void loop() {
  if(Serial.available()>0){
    Serial.println(Serial.available());
    char chr = Serial.read();
    if(chr=='#' || chr==10 || chr==13 ){
      // endchar
      if(pos>=frameSize)
      pixels.show();
//      Serial.print("leds: "); Serial.println(pos);
      pos = 0;
      rgb = 0;
    }
    else {
      chr = (chr-128)*2;
      // put in neopixel obj
      if(rgb==0){
        r = chr;
        rgb++;
      }
      else if(rgb==1){
        g = chr;
        rgb++;
      }
      else if(rgb==2){
        b = chr;
        pixels.setPixelColor(pos, pixels.Color(r,g,b));
    //        Serial.print(r);
    //        Serial.print(",");
    //        Serial.print(g);
    //        Serial.print(",");
    //        Serial.println(b);
        rgb = 0;
        pos++;
      }
      
      // Eo else
    }
    // serial
  }

  // Eo loop
}

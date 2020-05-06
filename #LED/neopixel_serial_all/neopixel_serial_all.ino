/*   just recieve one RGB color over serial and draw on ALL leds
 *  
 */
 
#include <Adafruit_NeoPixel.h>

#define PIN            2
#define NUMPIXELS      33
int frameSize = 1;


uint8_t rgb;
uint8_t r,g,b;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("hi this is arduino");
  
  pixels.begin();

  // blink
  pixels.setPixelColor(0, pixels.Color(0,150,0));
  pixels.setPixelColor(1, pixels.Color(150,0,0));
  pixels.show(); 

  rgb = 0;
  // * Eo setp
}


void loop() {
  if(Serial.available()>0){
//    Serial.println(Serial.available());
    uint8_t chr = Serial.read();
    if(chr==255){
      for(int p=0; p<10; p++){
        pixels.setPixelColor(p, pixels.Color(r,g,b));
      }
      pixels.show();
      rgb = 0;
    }
    else {
      // put in rgb obj
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
        rgb = 0;
        //frameSize++;
      }
      
      // Eo else
    }
    // serial
  }
  
  delay(50);
  
  // Eo loop
}

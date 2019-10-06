// try to control leds with struct... not working...

#include <Adafruit_NeoPixel.h>

#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)

// pin D4 = PD4
#define LED_pin      4
#define PIXEL_SIZE   144


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_SIZE, LED_pin, NEO_GRB +NEO_KHZ800);

uint32_t BLACK = strip.Color(0,0,0);

struct Pixel {
  uint8_t id;
  uint8_t R;
  uint8_t G;
  uint8_t B;
  
  uint8_t target_red;
  uint8_t target_green;
  uint8_t target_blue;
  
  uint8_t fade_speed;
  uint8_t repaint;
}pixels[PIXEL_SIZE];

uint8_t index = 0;

void add_pixel(uint8_t ind, uint8_t red, uint8_t green, uint8_t blue){
  pixels[index].id = ind;
  pixels[index].R = red;
  pixels[index].G = green;
  pixels[index].B = blue;
  pixels[index].repaint = 1;
  index++;
}

uint32_t random_color(uint8_t lim){
  uint32_t newCol = 0;
  
  uint8_t ran1 = random(lim);
  uint8_t ran2 = random(lim-ran1);
  uint8_t ran3 = lim-ran1-ran2;

  if(ran1%3 == 0) {
    newCol |= (ran1);
    newCol |= (ran2 << 8);
    newCol |= (ran3 << 16);
  }
  else if(ran1%3 == 1) {
    newCol |= (ran1 <<8);
    newCol |= (ran2 << 16);
    newCol |= (ran3 );
  }
  else {
    newCol |= (ran1 <<16);
    newCol |= (ran2 );
    newCol |= (ran3 << 8);
  }
  return newCol;
}

char buff[50];
                                                                // ###    Setup
                                                                
void setup() {
  //
  randomSeed(analogRead(0));
  
  Serial.begin(115200);
  
  strip.begin();
  
  wipe(BLACK);
  strip.setPixelColor( random(5), 20,0,0 );
  strip.setPixelColor( random(5), 0,20,0 );
  strip.setPixelColor( random(5), 0,0,20 );
  
  //strip.show();

  
  Serial.println("lets begin");

  delay(500);
  
  //add_pixel(11, 40, 10, 0);
  //pixels[0].id = random;
  pixels[0].R = 20;
  pixels[0].G = 20;
  pixels[0].B = 20;
  
  sprintf(buff,"pixel %d {\t%d\t%d\t%d }\n", pixels[0].id, pixels[0].R, pixels[0].G, pixels[0].B );
    Serial.print(buff);
  
  //strip.setPixelColor( random(5,10) , pixels[0].R, pixels[0].G, pixels[0].B );
  
  strip.show();
  
  delay(1000);
}

uint8_t any;

void loop() {
//  
//  if( random(100) > 90)  {
//    // at 10%
//    add_pixel( random(PIXEL_SIZE), 10,40,80);
//  }
  
//  update_pixels();
//  
//  strip.show();

  Serial.print(".");
  
  delay(700);
}


void update_pixels(void) 
{
  
  for( int i=0; i<index; i++)
  {
    
    // Check for repaints & paint
    if( pixels[i].repaint) {
      // if not painted
      strip.setPixelColor( pixels[i].id, pixels[i].R, pixels[i].G, pixels[i].B );
      pixels[i].repaint = 0; 
    }


    // Eo for
  }
  // Eo func
}
void wipe(uint32_t col)
{
  for( uint16_t i=0; i<PIXEL_SIZE; i++)
  {
    strip.setPixelColor(i, col);
    //strip.show();
    // Eo for
  }
  
}


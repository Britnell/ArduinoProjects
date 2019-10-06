
#include <Adafruit_NeoPixel.h>
#include "Pixel.h"
#include "LowPower.h"

#ifdef __AVR__
  #include <avr/power.h>
  #endif


#define PIN         10
#define PIXELS      6

#define DEBUG   1
#define __D     if(DEBUG)

Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(PIXELS, PIN, NEO_RGB + NEO_KHZ800);
period_t random_sleep();

/*  create a pixel instance for each pixel on the neopixel strip (Adafruit_NeoPixel object) 
 *    that you want to control with fade. When creating instance, simply give 
 *    integer of position in pixel strip. 
 */
Pixel myStrip[PIXELS];

int delayval = 1000; // delay for half a second
char buff[50];


long timer = 0;
int BR = 10;
int BR_P = 60000/BR;  // number = P x 100 ms

void setup() {
  
  randomSeed(analogRead(0));
  
  Serial.begin(115200);
  
  NeoPixel.begin(); // This initializes the NeoPixel library.
  NeoPixel.show();
  
  // BR = x per minute
    sprintf(buff, " Starting breathing, BR = %d \t so period is %d \n", BR, BR_P);
    Serial.print(buff);

  
  timer = millis();
  // Eo setup
}

uint8_t breather = 1;
uint8_t breath_c[3];
uint16_t breath_hue = 700;    // 0 to 767
  
void loop() 
{
  // BR 14 Breaths per minute
  if( millis() -timer < BR_P){
    // normal 
    
  }
  else {
    // restart
    timer += BR_P;
    
      // random new pixel
    breather = random(PIXELS);

      // increment hue
    if(random(100)<50)
      breath_hue += 10;
    else 
      breath_hue -= 10;

    sprintf(buff,"New Hue : %d \n", breath_hue);
      Serial.print(buff);
    
    }
  
  float phase = millis() -timer;
  phase = phase /BR_P;  // 1.0 index
  
  //phase = sin(phase * PI);
  phase = linear_sawtooth( phase);
  
  //Serial.println(phase);
  
  uint8_t breath_bright = 5 + 250 * phase;
    
      //  *  background
  set_all_draw(1,1,10);

  
      //  *  breathing with brightness
  //void hsb2rgbAN2(uint16_t index, uint8_t sat, uint8_t bright, uint8_t color[3]) {  
  hsb2rgb( breath_hue , 240 , breath_bright, breath_c );
  NeoPixel.setPixelColor( breather, NeoPixel.Color( breath_c[0], breath_c[1], breath_c[2] ) );

  
  NeoPixel.show();
  
  delay(10); 

  // Eo **  loop
}

// gets 0 to 1
//  returns 0 to 1 to 0
float linear_sawtooth( float x){
  if(x<=0.5){
    return x *2;
  }
  else {
    return 1-2*(0.5-x);
  }
}

void draw_pixel( Pixel * PIX ) {
  NeoPixel.setPixelColor(PIX->ID(), NeoPixel.Color(  PIX->G(), PIX->R(), PIX->B() ) );
}

void set_all( int r, int g, int b){
  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x] = Pixel(x);
    myStrip[x].set( r, g, b );  
  }
}

void set_all_draw( int r, int g, int b){
  for( int x=0; x<PIXELS ; x++ ) {
    myStrip[x] = Pixel(x);
    myStrip[x].set( r, g, b );  
    draw_pixel( (&myStrip[x]) );
  }
}

void draw_all( ){
  for( int x=0; x<PIXELS ; x++ ) {
    draw_pixel( (&myStrip[x]) );
  }
}


  

// * hue (index) is a value between 0 and 767. 
// 
period_t random_sleep(){
  
  int coin = random(2); 
  // 2 = 0.5 or 1s
  // 3= up to 1s
  // 4 = up to 2s
  
  __D Serial.print("Random sleep for " ); 
  __D Serial.println(coin ); 
  
  // SLEEP_120MS,   LEEP_250MS,  SLEEP_500MS,  SLEEP_1S,  SLEEP_2S,  SLEEP_4S,  SLEEP_8S,
  
  if(coin==0)
    return SLEEP_500MS;
    
  else if(coin==1)
    return SLEEP_1S;
    
  else if(coin==2)
    return SLEEP_2S;
    
  else if(coin==3)
    return SLEEP_4S;
    
  else if(coin==4)
    return SLEEP_8S;
}

#define RED 0
#define GREEN 1
#define BLUE 2

void hsb2rgb(uint16_t index, uint8_t sat, uint8_t bright, uint8_t color[3])
{
  uint16_t r_temp, g_temp, b_temp;
  uint8_t index_mod;
  uint8_t inverse_sat = (sat ^ 255);

  index = index % 768;
  index_mod = index % 256;

  if (index < 256)
  {
    r_temp = index_mod ^ 255;
    g_temp = index_mod;
    b_temp = 0;
  }

  else if (index < 512)
  {
    r_temp = 0;
    g_temp = index_mod ^ 255;
    b_temp = index_mod;
  }

  else if ( index < 768)
  {
    r_temp = index_mod;
    g_temp = 0;
    b_temp = index_mod ^ 255;
  }

  else
  {
    r_temp = 0;
    g_temp = 0;
    b_temp = 0;
  }

  r_temp = ((r_temp * sat) / 255) + inverse_sat;
  g_temp = ((g_temp * sat) / 255) + inverse_sat;
  b_temp = ((b_temp * sat) / 255) + inverse_sat;

  r_temp = (r_temp * bright) / 255;
  g_temp = (g_temp * bright) / 255;
  b_temp = (b_temp * bright) / 255;

  color[RED]  = (uint8_t)r_temp;
  color[GREEN]  = (uint8_t)g_temp;
  color[BLUE] = (uint8_t)b_temp;
}


// *

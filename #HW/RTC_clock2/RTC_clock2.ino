/*
 *    Birth clock v2 with RTC - with neopixel
 */
 
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>

#define SERIAL_ENABLE     1
#define IF_SER  if(SERIAL_ENABLE)
#define p(x)   IF_SER Serial.print(x)
#define pl(x)  IF_SER Serial.println(x)

#define Neopixel_pin  5
#define PIXELS        34

#define LED   LED_BUILTIN

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, Neopixel_pin, NEO_GRB + NEO_KHZ800);
RTC_DS3231 rtc;

long simon_day = 397051200L;
unsigned long since;
  
void setup () {
  strip.begin();
  delay(1000);
  
  IF_SER    Serial.begin(57600);
  
  delay(1000);

//  pinMode(LED,OUTPUT);
  while (! rtc.begin()) {
    pl("Couldn't find RTC");
    delay(1000);
//    digitalWrite(LED,1);
//      delay(300);
//    digitalWrite(LED,0);
//      delay(300);
    
//    abort();
  }
  
//  #############################
//  pl(" REPROGRAMMING");
//  rtc.adjust(DateTime(2020,6,2,11,27,00));

  DateTime now = rtc.now();
  since =  now.unixtime() +simon_day;  
  
  p(" UNIX now  : ");  pl(now.unixtime() );
  p(now.hour());  p(" ");  p(now.minute());  p(" ");  p(now.second());
  pl();
  
  p(" Simon seconds : ");  pl(since);
  
  
  // Eo setup
}

void loop () {

  since++;
//  since += 2048;
  
//  p(" B : \n");  Serial.println(since,BIN);

  uint32_t col;
  for(uint8_t b=0; b<32; b++){
    if(get_bit_ul(since,b) ){
      col = get_col_val(b);
    }
    else{
      col = strip.Color(0,0,0);
    }
    strip.setPixelColor(b, col );
  }
  strip.show();
  
  delay(1000);
}

uint8_t get_bit_ul( unsigned long num, int b){
  //Serial.println( num & (1<<b) );
  unsigned long check = 1;
  check = check<<b;
  if ( num & check )
    return 0x01;
  else 
    return 0;
}

uint32_t get_col_val(uint8_t l){
  uint8_t r,g,b;
  if(l<6) { // bit1-6  seconds : yellow
    r = map(l, 0,5, 6,60 );
    g = map(l, 0,5, 1,15 );
    b = 0;
  }
  else if(l< 12 ) { // bit 7-12  minutes : red
    r = map(l, 6,11, 5,80 );
    g = 0;
    b = 0;
  }
  else if(l < 17) { // 13 to 17     hours : purple
    r = map(l, 12,16, 5,50 );
    g = 0;
    b = map(l, 12,16, 5,30 );
  }
  else if(l < 22) { // 18 to 22     days : blue
    r = 0;
    g = 0;
    b = map(l, 17,21, 8,80 );
  }
  else if(l<26){    // 23 to 26     1.5 months - thiel
    r = 0;
    g = map(l, 22,25, 8,50 );
    b = map(l, 22,25, 8,50 );
  }
  else {  // 26 to 32     years : white
    r = map(l, 26,31, 30,80 );
    g = map(l, 26,31, 10,60 );
    b = map(l, 26,31, 5,30 );
  }
  return strip.Color(r, g, b);
}



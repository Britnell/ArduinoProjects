/*
 *    Birth clock v1 with RTC - prints out date
 */
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>

#define p(x)   Serial.print(x)
#define pl(x)  Serial.println(x)

#define Neopixel_pin  13;
#define PIXELS        32

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

long timer = 0;
long simon_day = 397051200L;
unsigned long since;
  
void setup () {
  Serial.begin(57600);
  while (!Serial);  

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  DateTime now = rtc.now();
  since =  now.unixtime() +simon_day;  
  p(" UNIX now  : ");  pl(now.unixtime() );
  p(" Simon day : ");  pl(simon_day);
  
  
  // Eo setup
}

void loop () {

  since++;
  
//  p(" B : \n");  Serial.println(since,BIN);
  
  for(uint8_t b=0; b<32; b++){
    if(get_bit_ul(since,31-b) ){
      p("1");
    }
    else{
      p("0");
    }
  }
  pl();
  
  timer = millis(); 
  
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

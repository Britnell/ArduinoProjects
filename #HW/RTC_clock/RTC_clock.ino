// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

#define p(x)   Serial.print(x)
#define pl(x)  Serial.println(x)

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

  //  # # To reprogramm RTC
  //  rtc.adjust(DateTime(2020,5,30,21,3,0));
  //rtc.lostPower()

  DateTime now = rtc.now();
  
  // now.year() , month() , day() , hour(), minute(), second()
  // now.unixtime() seconds
  // now.unixtime() / 86400L days
  //   rtc.getTemperature()
  //   DateTime future (now + TimeSpan(7,12,30,6));
  
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

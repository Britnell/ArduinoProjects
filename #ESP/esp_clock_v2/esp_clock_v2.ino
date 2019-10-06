/**
 * TODOS:
 *  make swife fade in intensity
 *  
 *  maybe change some colours by time of day
 */

 #include <ESP8266WiFi.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "KDG-3798D";
const char* password = "fA4cXNhfB5uX";


#define PIN     15
#define PIXELS  24


Adafruit_NeoPixel neoClock = Adafruit_NeoPixel(PIXELS, PIN, NEO_RGBW + NEO_KHZ800);

int timezone = 2 * 3600;
int dst = 0;
struct tm* p_tm;


                                  ///    ***    Setup

#define ENABLE_SERIAL        0

void setup() {

      // *  Serial
  randomSeed(analogRead(A0));
  
  if(ENABLE_SERIAL)  Serial.begin(115200);
  if(ENABLE_SERIAL)  Serial.setTimeout(2000);
  
  // Wait for serial to initialize.
  if(ENABLE_SERIAL)  while(!Serial) { }
  if(ENABLE_SERIAL)  Serial.println("\n\nI'm awake. Turning on LEDs");

  // LEDs 
  neoClock.begin();
  //ref_leds();   neoClock.show();
  
      // * WiFi
      
  if(ENABLE_SERIAL)  Serial.print("Wifi connecting to ");
  if(ENABLE_SERIAL)  Serial.println( ssid );
  WiFi.begin(ssid,password);
  if(ENABLE_SERIAL)  Serial.print("Connecting");
  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      if(ENABLE_SERIAL)  Serial.print(".");        
  }
  if(ENABLE_SERIAL)  Serial.println("Wifi Connected Success!");
  if(ENABLE_SERIAL)  Serial.print("NodeMCU IP Address : ");
  if(ENABLE_SERIAL)  Serial.println(WiFi.localIP() );

      // * RTC
  
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  if(ENABLE_SERIAL)  Serial.println("\nWaiting for Internet time");
  while(!time(nullptr)){
     if(ENABLE_SERIAL)  Serial.print(".");
     delay(1000);
  }
  if(ENABLE_SERIAL)  Serial.println("\nTime response....OK");   
  if(ENABLE_SERIAL)  Serial.println("\nGetting real-time date :");  
  time_t now = time(nullptr);
  p_tm = localtime(&now);
  while( p_tm->tm_year == 70){  // year = 1970
    delay(1000);
    now = time(nullptr);
    p_tm = localtime(&now);
    if(ENABLE_SERIAL)   Serial.print('.');
    //Serial.print("re-syncing\t");
    //Serial.println(p_tm->tm_year);
    //print_date(p_tm);
  }
  if(ENABLE_SERIAL)   Serial.print("Real-time is : ");
  
  if(ENABLE_SERIAL)  print_date(p_tm);   // print out in human readable

  
          // **  Function tester
//      Serial.println("Animation test running once");
//      for(int x=0; x<60; x++ ){
//        Serial.print(x);  Serial.print(", ");
//        hour_24( p_tm->tm_hour,  x);
//        neoClock.show();
//        delay(150);
//        // Eo for
//      }
//      Serial.println();

      // ** Display actual time
  
  // just dot for 24 hours
  hour_24_v2( p_tm->tm_hour,  p_tm->tm_min);
  // & Show
  neoClock.show();

      // * Sleep
  
  //int rem_secs = 59 - p_tm->tm_sec;
  //if(ENABLE_SERIAL)   Serial.print("Going into deep sleep for [s] : ");
  //ESP.deepSleep(rem_secs *1000000); // 20e6 is 20 seconds
  
  // sleep until next :10
  long sleep_past = p_tm->tm_min % 10;
  
  sleep_past = 10-sleep_past; // sleep remaining minutes to 10 past
    
  if(ENABLE_SERIAL)   Serial.print(" Now sleep for X minutes :  ");
  if(ENABLE_SERIAL)   Serial.print(sleep_past);
  sleep_past *= 60e6;
  ESP.deepSleep(sleep_past); 
  
  // 1min = 60e6   // 2min = 12e7   // 5min = 30e7 
  //ESP.deepSleep(12e7); 
  //
}


void ref_leds(){
  //-
  int g = 0;
  int r = 0;
  int b = 0;
  int w = 10;
  
  neoClock.setPixelColor(0, neoClock.Color( g, r, b, w ) );
  neoClock.setPixelColor(6, neoClock.Color( g, r, b, w ) );
  neoClock.setPixelColor(12, neoClock.Color( g, r, b, w ) );
  neoClock.setPixelColor(18, neoClock.Color( g, r, b, w ) );
  
}


void hour_24( int hour, int mins ) {
  //
  int r, b;
  float tmp;
  
  // 0 to 60  ~~ 255   
  // 255 = 4.25
  float fact = 4.25;
  r = int( fact *(60.0-mins) );
  b = int( fact * mins );

    // * background
  
//  for( int h=0; h<hour; h++)        // paint 0 to hour in random brightness
//    neoClock.setPixelColor( h, neoClock.Color( 0, r/random(10,25), b/random(10, 25), 0 )  );

  int h_plus = 0 ;     // 1-6 >> 2-7
  int h_mult = 20;    // max = 70
  for( int h=1; h<7; h++)       // paint strife tail 
    neoClock.setPixelColor( hour-h, neoClock.Color( 0, r/((h+h_plus)*h_mult), b/((h+h_plus)*h_mult), 0 )  );
  
    // * ref points
  ref_leds();

    // * indicator LED
  neoClock.setPixelColor( hour, neoClock.Color(0, r, b, 0) );
  
  // Eo 24_hour  
}



void hour_24_v2( int hour, int mins ) {
  //
  int r, b;
  float tmp;
  
  // 0 to 60  ~~ 255   
  // 150 = 2.5
  // 180 = 3.0
  // 255 = 4.25
  float fact = 2.5;
  r = int( fact *(60.0-mins) );
  b = int( fact * mins );

    // * background
  
//  for( int h=0; h<hour; h++)        // paint 0 to hour in random brightness
//    neoClock.setPixelColor( h, neoClock.Color( 0, r/random(10,25), b/random(10, 25), 0 )  );

  // 00 = all red
  // 15 = 3/4 red
  // 30 = 1/2 red blue
  // 45 = 
  int swife = 5;
  int thr = 60 / (swife+1);
  
  for( int i=1; i<=swife; i++){ // 1 to 5
    if(mins<i * thr)      //if under then red
      neoClock.setPixelColor( hour-i, neoClock.Color( 0, 40, 0, 0 ) );
    else              // if over then blue
      neoClock.setPixelColor( hour-i, neoClock.Color( 0, 0, 40, 0 ) );
  }
  
    // * ref points
  ref_leds();
  
    // * indicator LED
  neoClock.setPixelColor( hour, neoClock.Color(0, r, b, 0) );
  
  // Eo 24_hour   V2
}


void loop() {
  
}



                                      //   ***    Funcitons


void print_date(  struct tm* dat) {
  Serial.print(dat->tm_mday);
  Serial.print("/");
  Serial.print(dat->tm_mon + 1);
  Serial.print("/");
  Serial.print(dat->tm_year + 1900);
  Serial.print(" ");
  Serial.print(dat->tm_hour);
  Serial.print(":");
  Serial.print(dat->tm_min);
  Serial.print(":");
  Serial.println(dat->tm_sec);
  // print date
}

/* 
 *  First programm calculating days alive until 2018,
 *   then summing days of year so far + seconds of day so far
 *   for a very serious and global number of seconds alive.
 *  
 *  First shelf version
 */


#include <ESP8266WiFi.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>


#define DEBUG   1
#define _P(x)     if(DEBUG)

//const char* ssid = "<your WiFi SSID>";
//const char* password = "<your WiFi Password>";
const char* ssid = "KDG-3798D";
const char* password = "fA4cXNhfB5uX";

                                                        // ** Neopixel
#define PIN     D2
#define PIXELS  34

Adafruit_NeoPixel neoStrip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int ledPin = 13;

int timezone = 2 * 3600;
int dst = 0;


unsigned long seconds_pas = 0;
unsigned long seconds_rem = 0;
long stopwatch;

struct tm* p_tm;

// true index, 1 = jan etc..
uint8_t monthDays[13]= {0,31, 28, 31, 30, // jan - april
                          31, 30, 31, 31, // may j j aug
                          30, 31, 30, 31 };

int daysUntilThisMonth( uint8_t currMonth){
  int days = 0;
  for(int d=1; d<currMonth; d++)
    days += monthDays[d];
  return days;
}

unsigned long lifetime;
int INTVL = 1000;   // 1000 ms for RTC
int Ron, Gon, Bon;
static int randomStep, randomMax;
  



//      ****************************************************************************************



void setup() {

  Ron = 0;
  Gon = 80;
  Bon = 10;

  randomStep = 5;
  randomMax = 120;

  
  //  pinMode(ledPin,OUTPUT);
  //  digitalWrite(ledPin,LOW);

  randomSeed(analogRead(0));
  
  Serial.begin(115200);
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  
  __P("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  digitalWrite( ledPin , HIGH);
  Serial.println();
  
  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
  
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting e-time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nresponse OK");   
  Serial.println("\nGetting RTC");  
   
  time_t now = time(nullptr);
  
  p_tm = localtime(&now);
  stopwatch = millis();
  
  while( p_tm->tm_year == 70){  // year = 1970
    Serial.print("...");
    delay(900);
    now = time(nullptr);
    p_tm = localtime(&now);
    stopwatch = millis();
  }
  
  // print out in human readable
  print_date(p_tm);

  // days passed from 10.10.1990 to 31.12.2017    = 9945
  // https://www.timeanddate.com/date/durationresult.html?d1=10&m1=10&y1=1990&d2=1&m2=1&y2=2018
  lifetime = 9945;    // Tommy i.e. 10.10.1990
  //lifetime = 22128;  // Si 
  //Serial.print("days from 1990 to 31.12.2017 : ");  Serial.println(lifetime);
  
  int daysThisYear = daysUntilThisMonth((p_tm->tm_mon)+1);
  daysThisYear += (p_tm->tm_mday) -1;    // -1 for days PASSED
  lifetime += daysThisYear;
  //Serial.print("days in 2018 so far : ");  Serial.println(daysThisYear);
  //Serial.print("\n thus days since 10.10.1990 :\t\t");  Serial.println(lifetime );
  
  // days x seconds per day for seconds until midnight this morning
  lifetime *= 86400;
  //Serial.print("in seconds : \t\t\t\t\t");  Serial.println(lifetime);
  
  // seconds of this day
  seconds_pas = 0;
  seconds_pas += p_tm->tm_sec;
  seconds_pas += (p_tm->tm_min *60 );
  seconds_pas += (p_tm->tm_hour *3600);
  
  //Serial.print(" Today passed :\t");
  //Serial.print(seconds_pas);
    //  Serial.print(" seconds // or \t");
    //  Serial.print(seconds_pas/60);
    //  Serial.print(" mins // or hours :\t");
    //  Serial.println(seconds_pas/3600);

  lifetime += seconds_pas;
  Serial.print("\n So serious seconds passed : \t\t");
  Serial.println(lifetime );
  neoStrip.begin();

  // Eu setup
}



// *******************************************************************************************




void loop() {
  
  if(millis() -stopwatch >= INTVL ) {
    // should be more accurate as not losing millis to loop code
    stopwatch += INTVL;
    
    lifetime++;
    
    //Serial.print( lifetime);      Serial.print(" , ");
    //Serial.print( lifetime, BIN);     Serial.print(" ,\t\t");
    
    for( int l=0; l<32; l++){
      // l = bit
      uint32_t col;
      if( get_bit_ul(lifetime, l) ){       // HI
        col = neoStrip.Color(Ron, Gon, Bon);
      }
      else {     // LO
        col = neoStrip.Color( Ron/30, Gon/30, Bon/30 );
      }
      neoStrip.setPixelColor( l+1, col );
      // Eo for
    }
    neoStrip.show();

    // randomise colors a bit
    //if( random(100) <= 20 ) {
    random_rgb();

//    Serial.print( Ron/30);  Serial.print(",\t");
//    Serial.print(Gon/30);  Serial.print(",\t");
//    Serial.print(Bon/30 );  Serial.print("\n");
    delay(100);
  }
  
//  delay(1000);
}



void random_rgb(){
      //--
      
      int which = random(240);
      if(which < 80)
        which = 0;
      else if(which < 160)
        which = 1;
      else
        which = 2;
      
      if(  which == 0){
        add_sub( &Ron, &Gon, &Bon );
      }
      else if(  which == 1){
        add_sub( &Gon, &Bon, &Ron );
      }
      else {    // ==2
        add_sub( &Bon, &Ron, &Gon );
      }

      //Serial.print("\nR: ");    Serial.print(which);  Serial.print("\t");
      
//      Serial.print(Ron);    Serial.print(", ");
//      Serial.print(Gon);    Serial.print(", ");
//      Serial.print(Bon);    Serial.print("  .\n");

    // Eo random_rgb
}



void add_sub( int* win, int *a, int *b){
  randomStep = 3;
  randomMax = 120;
  
  uint8_t d;
  if(random(256) <128)
    d = 0;
  else
    d = 1;
  
  //Serial.print("\td: ");  Serial.print(d);  Serial.print("\t");
  
  if( *win < randomMax) {
      // if we can add to win
      if( d ){ 
        // try A first
        if( *a > randomStep){
          *win += randomStep;
          *a -= randomStep;
        }
        else if( *b > randomStep){
          *win += randomStep;
          *b -= randomStep;
        }
      }
      else {
        // try B first
        if( *b > randomStep){
          *win += randomStep;
          *b -= randomStep;
        }
        else if( *a > randomStep){
          *win += randomStep;
          *a -= randomStep;
        }
      }
      // Eo win < max
  }
  else if( *win > randomStep )
  {
      // win > max ---> subtract      only if not going negative
      if( d ){ 
        // try A first
        if( *a < randomMax ){
          *win -= randomStep;
          *a += randomStep;
        }
        else if( *b < randomMax ){
          *win -= randomStep;
          *b += randomStep;
        }
      }
      else {
        // try B first
        if( *b < randomMax ){
          *win -= randomStep;
          *b += randomStep;
        }
        else if( *a < randomMax ){
          *win -= randomStep;
          *a += randomStep;
        }
      }
      // Eo win > max
  }
  // Eo add_sub
}



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


uint8_t get_bit_ul( unsigned long num, int b){
  //Serial.println( num & (1<<b) );
  if ( (num & (1<<b) ) )
    return 0x01;
  else 
    return 0;
}


uint8_t check_bit_ul( unsigned long num, int b){
  //Serial.println( num & (0x01<<b) );
  return (num & (1<<b) );
}
// #

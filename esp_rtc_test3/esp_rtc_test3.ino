/* 
 *  Based on RTC this counts the passed and remaining seconds of the day 
 *    and outputs remaining seconds in countdown on neopixel strip
 *    
 *  V3 i would like to add some colour coding.
 *    maybe some customisation through web gui
 *  
 */


#include <ESP8266WiFi.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>


//const char* ssid = "<your WiFi SSID>";
//const char* password = "<your WiFi Password>";
const char* ssid = "KDG-3798D";
const char* password = "fA4cXNhfB5uX";

                                                        // ** Neopixel
#define PIN     D2
#define PIXELS  100

Adafruit_NeoPixel neoStrip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int ledPin = 13;

int timezone = 2 * 3600;
int dst = 0;


unsigned long seconds_pas = 0;
unsigned long seconds_rem = 0;
long stopwatch;

struct tm* p_tm;


void setup() {
  
//  pinMode(ledPin,OUTPUT);
//  digitalWrite(ledPin,LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  
  Serial.print("Connecting");

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
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");   
  
  Serial.println("\nGetting real-time date :");  
   
  time_t now = time(nullptr);
  
  p_tm = localtime(&now);
  stopwatch = millis();

  while( p_tm->tm_year == 70){  // year = 1970
    delay(1000);
    now = time(nullptr);
    p_tm = localtime(&now);
    stopwatch = millis();
    //
    Serial.print("re-syncing\t");
    //Serial.println(p_tm->tm_year);
    //print_date(p_tm);
  }

  
  // print out in human readable
  print_date(p_tm);
  
  //
  seconds_pas = 0;
  seconds_pas += p_tm->tm_sec;
  seconds_pas += (p_tm->tm_min *60 );
  seconds_pas += (p_tm->tm_hour *3600);
  
  // daily there are 86.400
  seconds_rem = (86400 -seconds_pas);
  seconds_rem--;
  seconds_pas++;

  Serial.println("\n ##  A day has 86.400 seconds");
  
  Serial.print(" So today passed : ");
  Serial.print(seconds_pas);
  Serial.print(" seconds // in mins : ");
  Serial.print(seconds_pas/3600);
  
  Serial.print(" \t and remaining :  ");
  Serial.print(seconds_rem );
  Serial.print(" in mins :  ");
  Serial.println(seconds_rem/3600);

  neoStrip.begin();
  
}




int INTVL = 1000;   // 1000 ms for RTC

void loop() {
  
  if(millis() -stopwatch >= INTVL ){
    // should be more accurate as not losing millis to loop code
    stopwatch += INTVL;
    seconds_rem--;  // count down remaining       //seconds_rem += 120;
    
//    seconds_pas++;  // count passed
    Serial.print( seconds_rem);
    Serial.print(" , ");
    Serial.print( seconds_rem, BIN);
//    Serial.print(" ,\t\t");
    for( int l=0; l<20; l++){
      // l = bit
      uint32_t col = get_color(l, get_bit_ul(seconds_rem, l) );
      neoStrip.setPixelColor( (l)+30, col );
      // Eo for
    }
    
//    Serial.print("\t passed : ");
//    Serial.print(seconds_pas);
//    Serial.print("\t");
//    
//    for( int l=0; l<32; l++){
//      if(get_bit_ul(seconds_pas, l)) {
//        Serial.print('1');      }
//      else {
//        Serial.print('0');      }
//      // Eo for
//    }
    Serial.println();
     
    neoStrip.show();
    delay(100);
  }
  
//  delay(1000);
}



                                            // **
                                            // *    get colors
                                            // **

uint32_t SEC_OFF = neoStrip.Color(2, 0, 0);
uint32_t SEC_ON = neoStrip.Color(180, 5, 0);

uint32_t MIN_OFF = neoStrip.Color(1,1, 0);
uint32_t MIN_ON = neoStrip.Color(90, 50, 0);

uint32_t JUST_OFF = neoStrip.Color( 1, 5, 5);
uint32_t JUST_ON = neoStrip.Color( 40, 90, 90);

uint32_t get_color( int p, uint8_t ONOFF){
  //
  // first 5 LEDs ~ 1 to 63 = Seconds
  if(p<=5){
    // 0 to 5
    uint8_t y_factor = 8;
    //
    if(ONOFF) // On           // 8 * 5 = 40
      return neoStrip.Color(180 -(p*y_factor), 0+(p*y_factor), 0);  //-(p*6)
    else      // Off
      return neoStrip.Color(2, 0, 0);
  } 
  // then 12 bits = 4000 > 3600 = Hours
  else if(p<=11){ 
    p -= 6;     // p  6 - 11 
    if(ONOFF)             // 
      return neoStrip.Color( 8 *(5-p), 10 *p , 100  );
    else
      return neoStrip.Color( 0, 1, 3);
  }
  else 
  {
    p -= 12;    // p 12-16 
    uint8_t b_fact = 12;
    if(ONOFF)
      return neoStrip.Color( 50 +p *16, 100 +p *6,  5 +p *20 );
    else
      return neoStrip.Color(3,1,0);
  }
  // Eo get color
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

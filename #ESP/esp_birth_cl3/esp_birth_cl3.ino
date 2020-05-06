/* 
 *  First programm calculating days alive until 2018,
 *   then summing days of year so far + seconds of day so far
 *   for a very serious and global number of seconds alive.
 *  
 *  adding serial interface for Wifi connection & birthday adjustment
 */


#include <ESP8266WiFi.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define DEBUG   1
#define _D      if(DEBUG)


int str_to_int( String line);

                                                        // ** Neopixel
#define PIN     D2
#define PIXELS  34

Adafruit_NeoPixel neoStrip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int ledPin = 13;
int timezone = 2 * 3600;
int dst = 0;

// EEPROM
int SSID_L = 11;
int SSID_BEGIN = 12;
int PSK_L = 51;
int PSK_BEGIN = 52;

int BDAY = 2;
int BMONTH= 3;
int BYEAR = 4;  
int day = 0;
int month = 0;
int year = 0;


unsigned long seconds_pas = 0;
unsigned long seconds_rem = 0;
long stopwatch;

struct tm* p_tm;

// true index, 1 = jan etc..
uint8_t monthDays[13]= {0,31, 28, 31, 30, // jan - april
                          31, 30, 31, 31, // may j j aug
                          30, 31, 30, 31 };

//uint8_t leapYears[32] = {  1904, 1908, 1912, 1916, 1920, 
//                           1924, 1928, 1932, 1936, 1940, 
//                           1944, 1948, 1952, 1956, 1960,
//                           1964, 1968, 1972, 1976, 1980,
//                           1984, 1988, 1992, 1996, 2000,
//                           2004, 2008, 2012, 2016, 2020, 
//                           2024, 2028
//                         };

int daysUntilThisMonth( uint8_t currMonth){
  int days = 0;
  for(int d=1; d<currMonth; d++)
    days += monthDays[d];
  return days;
}

int daysAfterThisMonth( uint8_t currMonth){
  int days = 0;
  for(int d=12; d>currMonth; d--)
    days += monthDays[d];
  return days;
}

unsigned long lifetime;
int INTVL = 300;   // 1000 ms for RTC
int Ron, Gon, Bon;
static int randomStep, randomMax;
  

char buff[50];

//      ****************************************************************************************

void setup() {
  
  Ron = 0;
  Gon = 40;
  Bon = 5;

  randomStep = 5;
  randomMax = 120;

  
  //  pinMode(ledPin,OUTPUT);
  //  digitalWrite(ledPin,LOW);

  randomSeed(analogRead(0));
  EEPROM.begin(512);
  _D Serial.begin(115200);

  // # _D Serial setup
  _D Serial.println("\n#\n#\n# Enter any text in serial to enter SETUP MODE");
  uint8_t setup_mode = 0;
  stopwatch = millis();
  
  _D while(millis()-stopwatch<5000 && !setup_mode){
    if(Serial.available()>0){
      setup_mode=1;
      Serial.read();
      Serial.println("\t$ enterering SETUP MODE");  
    }
  }
  
  _D if(setup_mode){
    // m for menu
    // w fpr wifi
    // b for birthday
    // e for exit
    
    Serial.println("#\n#\n#\t#\t#\t SETUP MENU\n#\n# ");
    Serial.println(" - to setup new Wifi login enter '#wifi' ");
    Serial.println(" - to change birthday enter '#birth' ");
    Serial.println(" - to exit SETUP MODE enter '#exit' ");

    char setup_menu = 'M';  
    
    while(setup_mode)
    {
      if(Serial.available()>0) {
        String line = Serial.readStringUntil('\n');
        Serial.print(" $"); Serial.println(line);

        if(setup_menu=='M') {
          if(line.equals("#exit")) {
            setup_mode = 0;
          }
          else if(line.equals("#wifi")){
            setup_menu = 'W';
            Serial.println("Ok lets add a wifi\n");
          }
          else if(line.equals("#birth")){
            setup_menu = 'B';
            Serial.println("Ok lets change the birthday.");
          }
          // setup menu
        }
        if(setup_menu=='W'){
          Serial.println("1. please enter the ssid / Wifi name");
          while(!Serial.available())            {   ;   }
          String new_ssid = Serial.readStringUntil('\n');
          Serial.print(" Adding new ssid entered : ");
          Serial.println(new_ssid);
          //ser("length: ", new_ssid.length(), 1);        ser("char 1: ", new_ssid.charAt(0), 1);
          EEPROM.write(SSID_L, new_ssid.length());
          
          for(int x=0; x<new_ssid.length(); x++){
            EEPROM.write(SSID_BEGIN+x, new_ssid.charAt(x));
          }
          Serial.println("new ssid complete. \n 2. please enter you passcode / password");
          while(!Serial.available())            {   ;   }
          String new_psk = Serial.readStringUntil('\n');
          Serial.print(" new psswd entered : ");
          Serial.println(new_psk);
          //ser("length: ", new_psk.length(), 1);       //ser("char 1: ", new_psk.charAt(0), 1);
          EEPROM.write(PSK_L, new_psk.length());          
          for(int x=0; x<new_psk.length(); x++){
            EEPROM.write(PSK_BEGIN+x, new_psk.charAt(x));
          }
          EEPROM.commit();
          Serial.println("\nDone, back in menu, what do you want to do?");
          setup_menu = 'M';
          // Eo if
        }
        if(setup_menu=='B'){
          //
          Serial.println(" 1. enter you DAY of birth ");
          while(!Serial.available())            {   ;   }
          String answ = Serial.readStringUntil('\n');
          day = str_to_int(answ);
          ser("day : ", day, 1);
          EEPROM.write(BDAY, day);
          
          Serial.println(" 2. enter you MONTH of birth");
          while(!Serial.available())            {   ;   }
          answ = Serial.readStringUntil('\n');
          month = str_to_int(answ);
          ser("month: ", month, 1);
          EEPROM.write(BMONTH, month);
          
          Serial.println(" 3. enter the last two digits of your YEAR of birth only e.g. 99 for 1999, or 01 for 2001");
          while(!Serial.available())            {   ;   }
          answ = Serial.readStringUntil('\n');
          year = str_to_int(answ);
          ser("year: ", year, 1);
          EEPROM.write(BYEAR, year);
          EEPROM.commit();
          Serial.println("ok done, back to menu, type #exit to EXIT MENU");
          setup_menu = 'M';
        }
        
        
        // Eo if
      }
      delay(200);
      // Eo SETUP while
    }
    // Eo if SETUP MODE
  }
  
  
      //  **   Read EEPROM
      //  *
      
    _D Serial.print("Wifi connecting to ");
    String ee_ssid = "";
    String ee_psk = "";
      // ssid
    int len = EEPROM.read(SSID_L);
    for(int x=0; x<len; x++){
      ee_ssid += char(EEPROM.read(SSID_BEGIN+x));
    }
      // psk
    len = EEPROM.read(PSK_L);
    for(int x=0; x<len; x++){
      ee_psk += char(EEPROM.read(PSK_BEGIN+x));
    }
    day = EEPROM.read(BDAY);
    month = EEPROM.read(BMONTH);
    year = EEPROM.read(BYEAR);
    
  
      // **  get_lifetime(day, month, year);
      
//  Serial.println("ok... lets calculate days alive");
  
  
//  sprintf(buff, " %d : %d : %d \n", day, month, year);    
//    Serial.print(buff);
  
  lifetime = 0;     // bday

  // add up days to end of your first year
  lifetime += 1 +monthDays[month] -day; // to end of month
  lifetime += daysAfterThisMonth(month);    // remaining months to end of year
  //ser(" + in year u were born", lifetime, 1);

      
       //   ** now begin wifi & get realtime
       //   **
      
  //Serial.println( ssid );
  //WiFi.begin(ssid,password);
  _D Serial.print(" WIFI ssid = "); 
  _D Serial.println(ee_ssid);
  _D Serial.print(" WIFI psk = "); 
  _D Serial.println(ee_psk);    
  WiFi.begin(ee_ssid.c_str(),ee_psk.c_str());
  
  _D Serial.print("\nConnecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      _D Serial.print(".");        
  }

  digitalWrite( ledPin , HIGH);
  _D Serial.println();
  
  _D Serial.println("Wifi Connected Success!");
  _D Serial.print("NodeMCU IP Address : ");
  _D Serial.println(WiFi.localIP() );
  
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  _D Serial.println("...Waiting for e-time");
  while(!time(nullptr)){
     _D Serial.print("*");
     delay(1000);
  }
  _D Serial.println("\nresponse OK");   
  _D Serial.println("\nGetting RTC");  
  time_t now = time(nullptr);
  p_tm = localtime(&now);
  stopwatch = millis();
  while( p_tm->tm_year == 70){  // year = 1970
    //Serial.print("...");
    delay(900);
    now = time(nullptr);
    p_tm = localtime(&now);
    stopwatch = millis();
  }

        //    **  Real-time
        //    **

  // print out in human readable
  print_date(p_tm);
  
  // now adding up days for each year
  int year_begin = year+1;
  // until currnet year 
  int year_end = (p_tm->tm_year);
  
  //sprintf(buff, "adding years together from %d to %d \n", year_begin, year_end);
    //Serial.print(buff);

  for( int y=year_begin; y<year_end; y++){
    if(y%4==0)  // leapyear
      lifetime += 366;
    else
      lifetime += 365;
  }
//  Serial.print("lifetime until beginning of this year: ");
//  Serial.println(lifetime);
  
      // days passed from 10.10.1990 to 31.12.2017    = 9945
      // https://www.timeanddate.com/date/durationresult.html?d1=10&m1=10&y1=1990&d2=1&m2=1&y2=2018
      //lifetime = 9945;    // Tommy i.e. 10.10.1990
      //lifetime = 22128;  // Si 
      //Serial.print("days from 1990 to 31.12.2017 : ");  Serial.println(lifetime);

      //    ** days alive this year 
      
  int daysThisYear = daysUntilThisMonth((p_tm->tm_mon)+1);
  daysThisYear += (p_tm->tm_mday) -1;    // -1 for days PASSED
  lifetime += daysThisYear;
  //Serial.print("days in 2018 so far : ");  Serial.println(daysThisYear);
  //Serial.print("\n thus days since 10.10.1990 :\t\t");  Serial.println(lifetime );

  _D sprintf(buff, "\n\n Ok so you were born on %d. %d. %d\n ", day, month, year, lifetime);
    _D Serial.print(buff);
  _D Serial.print("So..... if my calculations are correct \n(which they are..)\n You have been alive for \t\t");
  _D Serial.print(lifetime);   
  _D Serial.print(" days\n");
  
  
  // days x seconds per day for seconds until midnight this morning
  lifetime *= 86400;
  //Serial.print("which in seconds amounts to \t\t");  Serial.println(lifetime);
  
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
  //Serial.print("\n So serious seconds passed : \t\t");  Serial.println(lifetime );
  _D Serial.print(" so you have been alive for a grand \t\t");  
      _D Serial.print(lifetime);
  _D Serial.print(" seconds. \n which in binary is \t\t");  
      _D Serial.println(lifetime,BIN);
  
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
    //Serial.print( lifetime, BIN);     Serial.print(" ,\n");
    
    for( int l=0; l<32; l++){
      // l = bit
      uint32_t col;
      if( get_bit_ul(lifetime, l) ){       // HI
        
//        col = neoStrip.Color(Ron, Gon, Bon);
          col = get_col_val(l);
      }
      else {     // LO
        col = neoStrip.Color( 0,0,0 );
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


uint32_t get_col_val(uint8_t l){
  uint8_t r,g,b;
  if(l<6) { // bit1-6
    r = map(l, 0,5, 50,10 );
    g = 0;
    b = 0;
  }
  else if(l< 12 ) { // bit 7-12
    r = map(l, 6,11, 40,20 );
    g = map(l, 6,11, 10, 5 );
    b = 0;
  }
  else if(l < 17) { // 13 to 17
    r = 0;
    g = map(l, 12,16, 40,20 );
    b = map(l, 12,16, 0,20 );
  }
  else if(l < 12) { // 18 to 22
    r = 0;
    g = 0;
    b = map(l, 16,21, 50,20 );
  }
  else {
    r = 5;
    g=5;
    b=5;
  }
  return neoStrip.Color(r, g, b);
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


void ser( String abc, int x, int bol){
  Serial.print(abc);
  if(bol)
    Serial.println(x);
  else
    Serial.print(x);
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


                                                      // **  Int
int str_to_int( String line)
{
  int res = 0;
  
  // use L-1 for base 10 calc, run loop to =L
  uint8_t L = line.length() -1;
  
  uint8_t neg = 0;
  
  if(line[0] == '-'){
    // negative number
    // start at 1, and 1 shorter
    
    for( uint8_t i=1; i<= L; i++)
    {
      res += ch_to_int( line[i] ) * decimal( L-i);
      // Eo for
    }
    res *= -1;
    
  }
  else
  {
    // -- positive number
    
    for( uint8_t i=0; i<= L; i++)
    {
      res += ch_to_int( line[i] ) * decimal( L-i);
    }
    
  }
  return res;
}


int ch_to_int( char C )
{
  int res = 0;
  switch(C)
  {
    case '0':
      res = 0;
      break;
    case '1':
      res = 1;
      break;
    case '2':
      res =  2;
      break;
    case '3':
      res =  3;
      break;
    case '4':
      res =  4;
      break;
    case '5':
      res =  5;
      break;
    case '6':
      res = 6;
      break;
    case '7':
      res =  7;
      break;
    case '8':
      res =  8;
      break;
    case '9':
      res =  9;
      break;
    default:
      res =  0;
      break;
  }
  return res;
  // Eo switch
}

int decimal( int x)
{
  int res = 1;
  for(int i=0; i<x; i++) {
    res *= 10;
  }
  return res;
}

// #

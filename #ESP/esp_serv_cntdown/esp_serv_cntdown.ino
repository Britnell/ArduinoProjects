//#include <LowPower.h>

/*********
  Rui Santos
    tutorial
  https://randomnerdtutorials.com/esp8266-web-server/
    Git
  https://raw.githubusercontent.com/RuiSantosdotme/Random-Nerd-Tutorials/master/Projects/ESP8266_Web_Server_Arduino_IDE_updated.ino
  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>


                                                        // ** Neopixel
#define PIN     D2
#define PIXELS  25

Adafruit_NeoPixel neoStrip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);


const char* ssid = "KDG-3798D";
const char* password = "fA4cXNhfB5uX";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";


// Assign output variables to GPIO pins
const int output5 = D0;





uint8_t days = 0;
uint8_t hours = 12;

uint16_t seconds = 3600;

unsigned long SECOND_CNT = 5222333444;
long second_timer = 0;





void setup() {
  Serial.begin(115200);
  
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);

  
  // 
  neoStrip.begin();
  for(int x=0; x<PIXELS; x++) {
    neoStrip.setPixelColor( x, neoStrip.Color(0, 1, 5) );
  }
              neoStrip.show();
  Serial.println("\n##\nNeopixel strip intialized");
  
  

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  second_timer = millis();
}





int slideVar = 10;


void loop(){
  if( millis() -second_timer >= 1000 ){
    second_timer += 1000;

    Serial.print("int\t");
    Serial.println( second_timer , BIN);
    Serial.print("bit\t");

    for(int x=31; x>=0; x--){
      //Serial.print(x);
      //Serial.print(" :\t\t");
      //Serial.println(get_bit(second_timer, x));
      Serial.print(get_bit(second_timer, x));
      //Serial.print(", ");
    }
    Serial.println();
    
    // Eo seconds
  }

  // Eo loop
}


uint8_t get_bit( long num, uint8_t b){
  //Serial.println( num & (0x01<<b) );
  if ( (num & (0x01<<b) ) )
    return 0x01;
  else 
    return 0;
}

void text_int( String abc, int x, uint8_t NL){
  Serial.print(abc);
  Serial.print(x);
  if(NL)
    Serial.println();
}

void text_long( String abc, long x, uint8_t NL){
  Serial.print(abc);
  Serial.print(x);
  if(NL)
    Serial.println();
}


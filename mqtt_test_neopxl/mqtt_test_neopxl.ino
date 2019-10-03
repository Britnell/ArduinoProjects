/*
 * 
 *     
*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

#define pt(x)   Serial.print(x)
#define pn(x)   Serial.println(x)

uint8_t ledPin = LED_BUILTIN;   // D4 = GPIO2

//  **    Neopixel

#define PIN            4    // D2 = GPIO4
#define NUMPIXELS      9
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);



// **      MQTT


const char* ssid = "NIO_ProtoDev";
const char* password = "1SharedProtoDevPW1";

const char* mqtt_server = "mqtt://192.168.86.2:12948";
IPAddress server(192, 168, 86, 2);
int serv_port = 1883;

char * subscriptions[5];
uint8_t subs = 0;
WiFiClient espClient;
PubSubClient client(espClient);

char msg[50];
int brightness = 125;


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");  Serial.println(ssid);
  Serial.print(" (pw) ");         Serial.println(password);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  randomSeed(micros());
  Serial.println("");
    Serial.println("WiFi connected\n(my) IP address is : ");
    Serial.println(WiFi.localIP());
}


void setPixels(int val){
  for( int p=0; p<NUMPIXELS; p++){
    pixels.setPixelColor(p, pixels.Color(val,0,0) );
  }
  pixels.show();
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  String msg = "";
  String tpc = String(topic);
  
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];      }
  
  pt("\t#MQTT [");  pt(tpc);  pt("] ");  pn(msg);
    
  if( tpc.equals("message") )
  {
    // standard message
  }
  else if( tpc.equals("kinect") )
  {
      // MSG = float X.0
      String msgInt = msg.substring(0,msg.indexOf('.'));
      int val = str_to_int(msgInt);
      //pt(" Kinect :: ");  pn(val);      
      brightness = val;
      setPixels(brightness);
      
      //char digit = msg.charAt(0);      
  }  
  // Eo callback
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      
      Serial.println("connected");
      String tmp;
      tmp="hello world, this is ESP for MQTT  with NPXLZ \t#";
      
      tmp += clientId.c_str();
      
      client.publish("message", tmp.c_str() );
      
      for( uint8_t p=0; p<subs; p++ ){
        client.subscribe( subscriptions[p] );  
      }
      
    } 
    else {
      Serial.print("failed to reconnect, =");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


int str_to_int( String line)
{
  int result = 0;
  
  // length = decimal places 
  uint8_t dec = line.length() -1;
  if(line[0] == '-'){
    // * negative number
    // 1 shorter, i.e. start at 1
    for( uint8_t i=1; i<= dec; i++)
    {
      // Result =  Integer * 10^[decimal_place]
      result += ch_to_int( line[i] ) * decimal( dec-i);
    }
    result *= -1;    
  }
  else
  {
    // * positive number
    for( uint8_t i=0; i<= dec; i++)
    {
      result += ch_to_int( line[i] ) * decimal( dec-i);
    }
    
  }
  return result;
}
int ch_to_int( char C )
{
    int res = 0;
    switch(C)
    {
      case '0':      res = 0;      break;
      case '1':      res = 1;      break;
      case '2':      res =  2;      break;
      case '3':      res =  3;      break;
      case '4':      res =  4;      break;
      case '5':      res =  5;      break;
      case '6':      res = 6;      break;
      case '7':      res =  7;      break;
      case '8':      res =  8;      break;
      case '9':      res =  9;      break;
      default:      res =  0;      break;
    }
    return res;
}
int decimal( int x)
{
    int res = 1;
    for(int i=0; i<x; i++) {
      res *= 10;
    }
    return res;
}

//    ***     Setup

void setup() {

  pinMode(ledPin, OUTPUT);     // BUILTIN_LED pn O/P
  subscriptions[0] = "test";
  subscriptions[1] = "kinect";
  subscriptions[2] = "message";
  subs = 3;

  //
  Serial.begin(115200);
      pn("\n\nHi there this is totally ESP Neopixel with NeopixelZZ ");
  
  //
  pixels.begin();
      pn("\n ----Begun Neopixels");

  setPixels(0);
  
  //
  setup_wifi();
  
  //
  client.setServer(server, serv_port);   //mqtt_server
  client.setCallback(callback);
  
  // ** Eo setup
}

void loop() {

  // * MQTT

  // * Check connection
  if (!client.connected())        reconnect();
  // * MQTT loop
  client.loop();

  // **   Eo loop
}

// **   Eo file

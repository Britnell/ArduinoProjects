/*
 * 
 *     
*/

// **     NeoPixel

    // D4 = GPIO2
    // D3 = GPIO0
    // D2 = GPIO4

#define NUMBEROFPIXELS  16 
#define PIXELPIN 2 

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include <NeoPixelPainter.h>


Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(NUMBEROFPIXELS, PIXELPIN, NEO_RGBW + NEO_KHZ800);
NeoPixelPainterCanvas pixelcanvas = NeoPixelPainterCanvas(&neopixels); 
NeoPixelPainterBrush pixelbrush = NeoPixelPainterBrush(&pixelcanvas); 


//  **    WiFi

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define pt(x)   Serial.print(x)
#define pn(x)   Serial.println(x)

const char* ssid = "NIO_ProtoDev";
const char* password = "1SharedProtoDevPW1";

const char* mqtt_server = "mqtt://192.168.86.2:12948";
IPAddress server(192, 168, 86, 2);
int serv_port = 1883;

char * subscriptions[5];
uint8_t subs = 0;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  Serial.print("\n Connecting to ");
  Serial.println(ssid);
  Serial.print(" (pw) ");
  Serial.println(password);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
    Serial.println("\nWiFi connected");
    Serial.println("(my) IP address is : ");
    Serial.println(WiFi.localIP());
}

void brush1(){
  HSV brushcolor;
  brushcolor.h = 0; //zero is red in HSV. [ 0-255 ] 
  brushcolor.s = 255; //full color saturation
  brushcolor.v = 130; //about half the full brightness

  pixelbrush.setSpeed(200); //(4096 means to move one pixel per update)
  pixelbrush.setColor(brushcolor); 
  pixelbrush.setFadeSpeed(200); //(255 max, 200 is fairly fast)
  pixelbrush.setFadeout(true); //do brightness fadeout after painting
  pixelbrush.setBounce(false); //bounce the brush when it reaches the end of the strip
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  String msg = "";
  String tpc = String(topic);
  
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];      }
  
  Serial.print("Message arrived [");
    Serial.print(tpc);
    Serial.print("] ");
    Serial.println(msg);

    
  if( tpc.equals("message") )
  {
    // standard message
  }
  else if( tpc.equals("keypad") )
  {
      // button message
      pt("[keypad]\t");    pn( msg ); 
      
  }  
  else if( tpc.equals("rgb-ring") ){
    pt("[RGB-Ring]\t");
    pn(msg);
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
      tmp="hello world, this is ESP-Mqtt with neoPXLZ \t#";
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


//    ***     Setup

void setup() {

  // **   Neopixel
  
  pinMode(PIXELPIN, OUTPUT);
  neopixels.begin();

  brush1();
  
  // **   MQTT
  
  subs = 4;
    subscriptions[0] = "test";
    subscriptions[1] = "keypad";
    subscriptions[2] = "message";
    subscriptions[3] = "rgb-ring";
  
  Serial.begin(115200);
  pn("Hi there this is an MQTT Neopixel");
  
  setup_wifi();
  
  client.setServer(server, serv_port);   //mqtt_server
  client.setCallback(callback);
  
  // ** Eo setup
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  neopixels.clear();
  pixelbrush.paint(); 
  pixelcanvas.transfer();

  neopixels.show();

  // **   Eo loop
}

// **   Eo file

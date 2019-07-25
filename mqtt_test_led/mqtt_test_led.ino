/*
 * 
 *     
*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define pt(x)   Serial.print(x)
#define pn(x)   Serial.println(x)

#define BTN 1
#define LED 2
 uint8_t THIS = LED;

#define IF_BTN   if(THIS==BTN)
#define IF_LED   if(THIS==LED)

const char* ssid = "NIO_ProtoDev";
const char* password = "1SharedProtoDevPW1";

const char* mqtt_server = "mqtt://192.168.86.2:12948";
IPAddress server(192, 168, 86, 2);
int serv_port = 1883;

uint8_t buttonPin = 4;          // D2 = GPIO4
uint8_t ledPin = LED_BUILTIN;   // D4 = GPIO2
uint8_t buttonState = 0;

  // RGB LED on pins D4,3,2
uint8_t b = 2;  // D4 = GPIO2
uint8_t g = 0;  // D3 = GPIO0
uint8_t r = 4;  // D2 = GPIO4

char * subscriptions[5];
uint8_t subs = 0;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print(" (pw) ");
  Serial.println(password);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("(my) IP address is : ");
    Serial.println(WiFi.localIP());
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
  else if( tpc.equals("esp_button_1") )
  {
      // button message
      char digit = msg.charAt(0);
      pt("   ->BUTT:");    pn( digit ); 
      
      IF_LED if(digit=='0'){
        digitalWrite(r, HIGH);
        digitalWrite(g, LOW);
        digitalWrite(b, LOW);
      }
      else if(digit=='1'){
        digitalWrite(r, LOW);
        digitalWrite(g, HIGH);
        digitalWrite(b, HIGH);
      }
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
      IF_BTN tmp="hello world, this is Button\t#";
      IF_LED tmp="hello world, this is Led\t#";
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

  // **   GPIO
  
  IF_BTN pinMode(ledPin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  IF_BTN pinMode(buttonPin, INPUT);

  IF_LED pinMode( r , OUTPUT);   
  IF_LED pinMode( g , OUTPUT);   
  IF_LED  pinMode( b , OUTPUT);   
  IF_LED  digitalWrite(r, HIGH);
  IF_LED  digitalWrite(g, LOW);
  IF_LED  digitalWrite(b, LOW);

  subscriptions[0] = "test";
  subscriptions[1] = "esp_button_1";
  subscriptions[2] = "message";
  subs = 3;
  
  Serial.begin(115200);
  IF_BTN pn("Hi there this is the button code");
  IF_LED pn("Hi there this is the LED code ");
  
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
  
      IF_BTN 
      if(!digitalRead(buttonPin)){
        if(!buttonState){
          // *  RISING EDGE
          digitalWrite(ledPin, HIGH);   // ACTIVE LOW
          snprintf (msg, 50, "%d", buttonState);
          Serial.print("Publish message: ");
            Serial.println(msg);
          client.publish("esp_button_1", msg);
          buttonState=1;
        }
      }
      else {
        if(buttonState){
          // *  FALLING EDGE
          digitalWrite(ledPin, LOW);   // ACTIVE LOW
          snprintf (msg, 50, "%d", buttonState);
          Serial.print("Publish message: ");
            Serial.println(msg);
          client.publish("esp_button_1", msg);
          buttonState=0;
        }
      }

  // **   Eo loop
}

// **   Eo file



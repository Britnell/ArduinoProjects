/*
 * 
 *     
*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Keypad.h>

#define pt(x)   Serial.print(x)
#define pn(x)   Serial.println(x)

uint8_t ledPin = LED_BUILTIN;   // D4 = GPIO2


// **     Keypad

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

  // D1, D2, D3, D4
  // 5 ,  4,  0,  2
byte rowPins[ROWS] = { 5, 4, 0, 2 }; //connect to the row pinouts of the keypad
  // Label: D5, D6, D7, D0
  // GPIO : 14, 12, 13, 16
byte colPins[COLS] = { 14, 12, 13, 16 }; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// **     WiFi & MQTT

const char* ssid = "NIO_ProtoDev";
const char* password = "1SharedProtoDevPW1";
const char* mqtt_server = "mqtt://192.168.86.2:12948";
IPAddress server(192, 168, 86, 2);
int serv_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print(" ($$) ");
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
    if( msg.substring(0,6).equals("BUTTON") )
        char digit = msg.charAt(7);                
  }  
  // Eo callback
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) 
    {
        
      Serial.println("connected");
      client.subscribe( "message" );  
      client.subscribe( "keypad" );  
      
      String tmp = "hi this is keypad \t#";
      tmp += clientId.c_str();
      client.publish("message", tmp.c_str() );
      client.publish("keypad", tmp.c_str() );
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

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      break;

    case RELEASED:
      client.publish("keypad", "r" );
      Serial.print("R");
      break;

    case HOLD:
      client.publish("keypad", "h" );
      Serial.print("H");
      break;
  }
}



//    ***     Setup

void setup() {

  // **   GPIO

  keypad.addEventListener(keypadEvent); 

  // ** Serial
  Serial.begin(115200);
    pn("This is ESP Keypad");

  // ** Wifi & MQTT
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

  char key = keypad.getKey();
  if (key) 
  {
    String msg = String(key);
    client.publish("keypad", msg.c_str() );
    
    if(key=='#')    Serial.println(key);
    else          Serial.print(key);
  }
  
  // **   Eo loop
}


// **   Eo file



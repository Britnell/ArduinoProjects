/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "NIO_ProtoDev";
const char* password = "1SharedProtoDevPW1";

//"broker.mqtt-dashboard.com";
const char* mqtt_server = "mqtt://192.168.86.2:12948";
IPAddress server(192, 168, 86, 2);
int serv_port = 1883;

uint8_t buttonPin = 4;          // D2 = GPIO4
uint8_t ledPin = LED_BUILTIN;   // D4 = GPIO2
uint8_t buttonState = 0;


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
  Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


//    ***     Setup


void setup() {
  
  pinMode(ledPin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(buttonPin, INPUT);
  
  Serial.begin(115200);
  
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

  if(!digitalRead(buttonPin)){
    if(!buttonState)
    {
      // *  RISING EDGE
      digitalWrite(ledPin, HIGH);   // ACTIVE LOW
      snprintf (msg, 50, "BUTTON-%d", buttonState);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("esp-button", msg);
      buttonState=1;
    }
  }
  else {
    if(buttonState){
      // *  FALLING EDGE
      digitalWrite(ledPin, LOW);   // ACTIVE LOW
      snprintf (msg, 50, "BUTTON-%d", buttonState);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("test", msg);
      buttonState=0;
    }
  }

  // **   Eo loop
}

// **   Eo file



/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp8266-nodemcu-access-point-ap-web-server/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

// ################################### HTML

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 DHT Server</h2>
  <p>
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
  <p>
    <button id="ledbutton" onclick="button_function()">LED</button>
  </p>
</body>
<script>
function button_function(){
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/click", true);
  xhttp.send();
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// ################################### HTML



//#include <Adafruit_Sensor.h>
//#include <DHT.h>

const char* ssid     = "ESP8266-Access-Point";
const char* password = "123456789";

#define DHTPIN 5     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

//DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// Create AsyncWebServer object on port 80

AsyncWebServer server(80);
DNSServer         dnsServer;              
IPAddress         apIP(10, 10, 10, 1);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 3000;  


// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

uint8_t LEDstate = 1;
uint8_t LEDpin = LED_BUILTIN;

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
//  dht.begin();
  
  Serial.print("Setting AP (Access Point)…");
  // .softAP(const char* ssid, const char* password, int channel, int ssid_hidden, int max_connection)
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid); // password

  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AdP IP address: ");
  Serial.println(IP);
  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // DNS & Captive
  dnsServer.start(53, "*", IP);

  // OUTPUTS

  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, LEDstate);
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  server.on("/click", HTTP_GET,  [](AsyncWebServerRequest *request){
    // switch led?
    LEDstate = !LEDstate;
//    Serial.print(LEDstate); Serial.println("CLICK");
    digitalWrite(LEDpin, LEDstate);
    //request->send_P(200, "text/plain", String(LEDstate).c_str());
    request->send_P(200, "text/html", index_html, processor);
  });

  // * Captive answer all
  server.onNotFound([](AsyncWebServerRequest *request){
    //request->send(404);
    request->send_P(200, "text/html", index_html, processor);
  });
//  server.onNotFound(NULL); // remove handler

  
  // Start server
  server.begin();
}
 
void loop(){
  
  dnsServer.processNextRequest();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    float newT = 10.1;
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // if temperature read failed, don't change t value
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t += newT;
      Serial.println(t);
    }
    // Read Humidity
    float newH = 80.8;
    // if humidity read failed, don't change h value 
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h += newH;
      Serial.println(h);
    }
  }
}


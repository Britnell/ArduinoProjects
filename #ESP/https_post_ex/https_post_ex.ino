/*
 * HTTPS Secured Client POST Request
 * Copyright (c) 2019, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */
 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
//#include <ESPAsyncWebServer.h>

#define p(x)  Serial.print(x)
#define pl(x) Serial.println(x)



const char *ssid = "Radnage";  //ENTER YOUR WIFI SETTINGS
const char *password = "Qu1ckBr0wn";
 
const char *host = "accounts.spotify.com"; 
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80

// * SHA Fingerprints https://www.grc.com/fingerprints.htm
const char fingerprint[] PROGMEM = "B9 79 6B CE FD 61 21 97 A7 02 90 EE DA CD F0 A0 44 13 0E EB";

const char clientId[] PROGMEM    = "954d83a67fde4133bcf00cc20bb618b3";
const char clientSecret[] PROGMEM = "9f6cc5fdeff24200b36c3f7d40024f1c";

const char redirectURI[] PROGMEM = "http://192.168.68.119/callback";
const char redirectEscape[] PROGMEM = "http%3A%2F%2F192.168.68.119%2Fcallback";

//AsyncWebServer server(80);
ESP8266WebServer server(80);

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() 
{
  delay(1000);
  Serial.begin(115200);

  // * Wifi
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  p("\nConnecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    p(".");
  }
 
  p("\nConnected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  
  // * Server
  
  server.on("/", [](){
    String authorize_url = "https://accounts.spotify.com/authorize?client_id=954d83a67fde4133bcf00cc20bb618b3&response_type=code&redirect_uri=http%3A%2F%2F192.168.68.119%2Fcallback&scope=user-read-private%20user-read-email&state=esp1234";
    String page = String("<!DOCTYPE HTML><html><body>");
    page += " Please open <a href=\"";
    page += authorize_url;
    page += "\">LINK</a></body></html>"; 
    server.send(200,"text/html", page);
  });

  server.on("/callback", [](){
    // get code
    String code = "";
    for (uint8_t i = 0; i < server.args(); i++) {
      if(server.argName(i).equals("code")){
        code = server.arg(i);
      }
    }    
    pl(" received auth. code "); pl(code);
    post_request(code);
      
    // Eo /callback
  });
  
  server.on("/authorize", [](){
    server.send(200, "text/plain", "YO" );      
  });  
  
  server.onNotFound(handleNotFound);
  
  pl("Starting server" );
  server.begin();
  
  // * Eo setup
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
uint8_t one = 1;

void loop() {
  delay(5000);

//  if(one){
//    one = 0;
//    post_request();
//  }

  server.handleClient();
//  MDNS.update();
}

void get_access_token(String code){
  //
  pl("Lets make the POST request ");


  
  // Eo func
}

void post_request(String code){
  
  pl(" = = = ==   LETS POST = = = =  ");
  WiFiClientSecure httpsClient;    //Declare object of class WiFiClient
 
  Serial.println(host);
 
  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000); // 15 Seconds
  delay(1000);
  
  Serial.print("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  
  String getData, Link;
  
  //POST Data
  Link = "/api/token";
  const char* clientId     = "954d83a67fde4133bcf00cc20bb618b3";
  const char* clientSecret = "9f6cc5fdeff24200b36c3f7d40024f1c";
  String redirectURI = "http://192.168.68.119/callback";
  String redirectEscape = "http%3A%2F%2F192.168.68.119%2Fcallback";
  
  Serial.print("requesting URL: ");
  Serial.println(host);
  
  /*
   POST /post HTTP/1.1
   Host: postman-echo.com
   Content-Type: application/x-www-form-urlencoded
   Content-Length: 13
  
   say=Hi&to=Mom
    
   */

  String body = String("") +"grant_type=authorization_code"+
                 "&redirect_uri=" +redirectEscape +
                 "&code="+ code+
                 "&client_id=" +clientId +
                 "&client_secret=" +clientSecret;
  
  String body_length = String(body.length());

  String post = String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded"+ "\r\n" +
               "Content-Length: "+ body_length+ "\r\n\r\n" +
               body+ "\r\n" +
               "Connection: close\r\n\r\n";

  pl(" Sending POST : ");
  pl(post);
 
  httpsClient.print(post);
  
  Serial.println("request sent");
                  
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
 
  Serial.println("reply was:");
  Serial.println("==========");
  String page = "";
  while(httpsClient.available()){        
    String line = httpsClient.readStringUntil('\n');  //Read Line by Line
    page += line;
//    Serial.println(line); //Print response
  }
  Serial.println("==========");

  server.send(200,"text/html", page);
  
  //  https.stop();
  
  // * Eo func
}


// *

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
#include <EEPROM.h>

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

#define TOKEN_ADDRESS   0

struct {
  char access_token[200];
  char refresh_token[200];
} token_data;

void setup() 
{
  // * Init
  EEPROM.begin(512);
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
  Serial.println(WiFi.localIP());  // 
  
  // * Server
  
  server.on("/", [](){
    pl("[Server] GET request: \"/" );
    // * Read tokens
    EEPROM.get(TOKEN_ADDRESS, token_data);
    pl("TOKENS:");
    pl(token_data.access_token);
    pl(token_data.refresh_token);
    
    // * Authorization
    String authorize_url = "https://accounts.spotify.com/authorize?client_id=954d83a67fde4133bcf00cc20bb618b3&response_type=code&redirect_uri=http%3A%2F%2F192.168.68.119%2Fcallback&scope=user-read-private%20user-read-email&state=esp1234";
    String page = String("<!DOCTYPE HTML><html><body>");
    page += " You need to authorize to get tokens <a href=\"";
    page += authorize_url;
    page += "\">Authorize</a></body></html>"; 
    server.send(200,"text/html", page);
  });

  server.on("/reset", [](){
    server.send(200,"text/plain", "i've reset those Tokens for you");
    strncpy(token_data.access_token, "", 200);
    strncpy(token_data.refresh_token, "", 200);
    EEPROM.put(TOKEN_ADDRESS, token_data);
    EEPROM.commit();
  });
  
  server.on("/callback", [](){
    pl("[Server] GET request: \"/callback" );
    // get code
    String code = "";
    for (uint8_t i = 0; i < server.args(); i++) {
      if(server.argName(i).equals("code")){
        code = server.arg(i);
      }
    }    
    // * use this code to get access tokens
    get_access_token(code);
      
    // Eo /callback
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
  delay(100);
  server.handleClient();
  // *
}


void get_access_token(String code){

  // * https POST
  WiFiClientSecure httpsClient;
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);
  delay(1000);
  
  int r=0;
  int retries = 10;
  while((!httpsClient.connect(host, httpsPort)) && (r < retries)){
      delay(100);     r++;
  }
  if(r==retries) {
    pl("Connection failed");
  }
  else {
    pl("Connected to web");
  }
  
  
  //POST Data
  
  const char* clientId     = "954d83a67fde4133bcf00cc20bb618b3";
  const char* clientSecret = "9f6cc5fdeff24200b36c3f7d40024f1c";
  String redirectURI = "http://192.168.68.119/callback";
  String redirectEscape = "http%3A%2F%2F192.168.68.119%2Fcallback";
  
  Serial.print("requesting URL: ");
  Serial.println(host);
  

  String body = String("") +"grant_type=authorization_code"+
                 "&redirect_uri=" +redirectEscape +
                 "&code="+ code+
                 "&client_id=" +clientId +
                 "&client_secret=" +clientSecret;
  
  String body_length = String(body.length());

  String post = String("POST") +" /api/token HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded"+ "\r\n" +
               "Content-Length: "+ body_length+ "\r\n\r\n" +
               body+ "\r\n" +
               "Connection: close\r\n\r\n";

//  pl(" Sending POST REQUEST : ");
//  pl(post);
  httpsClient.print(post);

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
//      Serial.println("headers received");
      break;
    }
  }
 
  String page = "";
  while(httpsClient.available()){        
    String line = httpsClient.readStringUntil('\n'); 
    page += line;
  }

        //  pl(" Received page = = = = = = = = ");
        //    pl(page);

  // * extract access token
  String access_key = String("\"access_token\"");
  int access_index = page.indexOf(access_key);
  if(access_index!=-1){
    int from = access_index +access_key.length() +2;
    int to = page.indexOf("\"",from+1);
    String access_token = page.substring(from,to);

    pl("Access token received! ");
    pl(access_token);
    strncpy(token_data.access_token, access_token.c_str(), 200);    
  }

  // * Extract access token
  String refresh_key = String("\"refresh_token\"");
  int refresh_index = page.indexOf(refresh_key);
  if(refresh_index!=-1){
    int from = refresh_index +refresh_key.length() +2;
    int to = page.indexOf("\"",from+1);
    String refresh_token = page.substring(from,to);
    
    pl("refresh token received! ");
    pl(refresh_token);
    strncpy(token_data.refresh_token, refresh_token.c_str(), 200);
  }
  EEPROM.put(TOKEN_ADDRESS, token_data);
  EEPROM.commit();
  server.send(200,"text/plain", "I've saved those tokens for you. Go do your thing");
  
  //  https.stop();
  
  // * Eo func
}


// *

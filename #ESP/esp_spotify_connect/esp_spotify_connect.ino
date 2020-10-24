/*
 *  Spotify Authorization
 *      https://developer.spotify.com/documentation/general/guides/authorization-guide/#authorization-code-flow
 *  EEPROM 
 *      https://arduino.stackexchange.com/questions/25945/how-to-read-and-write-eeprom-in-esp8266
 *  
 *  
 *  
*/
 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include "secrets.h"

#define DEBUG 1
#define p(x)  if(DEBUG) Serial.print(x)
#define pl(x) if(DEBUG) Serial.println(x)

const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
const char authentication_host[] = "accounts.spotify.com"; 
const char api_host[] = "api.spotify.com";

const char fingerprint[] PROGMEM = "B9 79 6B CE FD 61 21 97 A7 02 90 EE DA CD F0 A0 44 13 0E EB";
//const uint8_t fingerprint[20] = { 0xB9, 0x79, 0x6B, 0xCE, 0xFD, 0x61, 0x21, 0x97, 0xA7, 0x02, 0x90, 0xEE, 0xDA, 0xCD, 0xF0, 0xA0, 0x44, 0x13, 0x0E, 0xEB };
  // * SHA Fingerprints https://www.grc.com/fingerprints.htm

String scopes = String("user-read-playback-state")+
                 "%20user-read-private"+
                 "%20user-read-email";

String redirectURI;
String redirectEscape;

ESP8266WebServer server(80);

#define TOKEN_ADDRESS   0

struct {
  char access_token[200];
  char refresh_token[200];
} token_data;

uint8_t MUST_VALIDATE = 0;

//    *******   SETUP     *****

void setup() 
{
  // * Init
  delay(1000);
  Serial.begin(115200);

  // * Wifi
  setup_WiFi();
  
  // * Server
  setup_server();
  
  // * Tokens
  EEPROM.begin(512);    // init EEPROM
  read_tokens();
  check_tokens();
  
  // * Eo setup
}

void setup_WiFi()
{
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
 
  p("\n WiFi Connect - ");    pl(ssid);
  p("IP address: ");    pl(WiFi.localIP());  
  
  redirectURI = String("http://")+
                String(WiFi.localIP()[0])+"."+
                String(WiFi.localIP()[1])+"."+
                String(WiFi.localIP()[2])+"."+
                String(WiFi.localIP()[3])+"/callback";
  
  redirectEscape = redirectURI;
  redirectEscape.replace(":","%3A");
  redirectEscape.replace("/","%2F");
  
  // * 
}


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


void setup_server()
{
  server.on("/", [](){
    pl("[Server] GET request: \"/" );
    String page;
    
    if(MUST_VALIDATE){
      // * Show authorization
//      String authorize_url = "https://accounts.spotify.com/authorize?client_id=954d83a67fde4133bcf00cc20bb618b3&response_type=code&redirect_uri=http%3A%2F%2F192.168.68.119%2Fcallback&scope=user-read-private%20user-read-email&state=esp1234";
      String authorize_url = String("https://accounts.spotify.com/authorize")+
                             "?client_id=" +clientId +
                             "&response_type=code&redirect_uri=" +redirectEscape+
                             "&scope="+scopes;  //&state=esp1234
      page = String("<!DOCTYPE HTML><html><body>")+
             " You need to authorize to get tokens "+
             "<a href=\""+authorize_url+"\">Authorize</a>"+
             "</body></html>"; 
    }
    else {
      page = String("<!DOCTYPE HTML><html><body>")+
             "<div>Welcome! </div>"+
             "<div>" +"<a href=\"/player\">Player</a>" +"</div>"
             "<div>" +"<a href=\"/reset\">RESET</a>" +"</div>"
             +"</body></html>"; 
    }
        
    server.send(200,"text/html", page);
  });

  server.on("/player", [](){
    pl("[server]  GET /player ");
    String play_data = get_playback();
    String page = String("<!DOCTYPE HTML><html><body>")+
             "<div> player data  </div>"
             "<div>"+ play_data+"</div>"
             "</body></html>";
    server.send(200,"text/html", page);
  });
  
  server.on("/reset", [](){
    pl("[server] GET /reset ");
    String page = String("<!DOCTYPE HTML><html><body>")+
             "<div>" +"I've reset the tokens for you, go "
             +"<a href=\"/\">back to main</a>" 
             +"</div>"
             +"</body></html>"; 
    server.send(200,"text/html", page);
    strncpy(token_data.access_token, "", 200);
    strncpy(token_data.refresh_token, "", 200);
    EEPROM.put(TOKEN_ADDRESS, token_data);
    EEPROM.commit();
    MUST_VALIDATE = 1;
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
}

//  ****   EEPROM

void read_tokens()
{
  // * Read tokens
  EEPROM.get(TOKEN_ADDRESS, token_data);
}

void check_tokens()
{
  pl("Checking tokens :");
  pl(token_data.access_token);
  pl(token_data.refresh_token); 
  String acc = String(token_data.access_token);
  String refr = String(token_data.refresh_token);
  if(acc.length()==0 || refr.length()==0){
    pl(" tokens were reset");
    // --> Authorize
    // show Authorization link on Homescreen
    MUST_VALIDATE = 1;
  }
  else {
    // test_call();
  }
}

//    *********   HTTPS    ************

void test_call()
{
  
}




/*      ****************          API         **********
 *  Endpoint reference
 *    https://developer.spotify.com/documentation/web-api/reference/
 *  
 *  
 */



/* Error response 
======= headers
HTTP/1.0 401 Unauthorized
www-authenticate: Bearer realm="spotify"
access-control-allow-origin: *
access-control-allow-headers: Accept, App-Platform, Authorization, Content-Type, Origin, Retry-After, Spotify-App-Version, X-Cloud-Trace-Context, client-token
access-control-allow-methods: GET, POST, OPTIONS, PUT, DELETE, PATCH
access-control-allow-credentials: true
access-control-max-age: 604800
content-type: application/json
Content-Length: 74
strict-transport-security: max-age=31536000
x-content-type-options: nosniff
vary: Accept-Encoding
date: Thu, 22 Oct 2020 11:40:51 GMT
server: envoy
Via: HTTP/2 edgeproxy, 1.1 google
Alt-Svc: clear
 *  
 */
 
 
String get_playback()
{  
  // * https://developer.spotify.com/console/get-user-player/
  pl(" GET playback ");
  String api = "/v1/me/player";

  String req = String("GET ") +api  +" HTTP/1.1\r\n" // 
               +"Host: " +api_host +"\r\n"
               +"Accept: application/json\r\n"
               +"Content-Type: application/json\r\n"
               +"Authorization: Bearer " +token_data.access_token+"\r\n";
               +"Connection: close\r\n\r\n"; // 
  
  // * https connection hh
  WiFiClientSecure httpsClient;
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);
  delay(1000);
  int r=0;
  int retries = 10;
  while((!httpsClient.connect(api_host, httpsPort)) && (r < retries)){
      delay(100);     r++;   p(".");
  }
  if(r==retries) {
    pl("Connection failed");
  }
  else {
    pl("\tConnection established");
  }

  pl(" ===== request : ");  pl(req);
  httpsClient.print(req);
  
  pl("\n======= headers");
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    p(line);
    if (line == "\r") {
      break;
    }
  }
  String page = "";
  while(httpsClient.available()){        
    String line = httpsClient.readStringUntil('\n'); 
    page += line;
  }
  pl("\n\n======== page");
  pl(page);
  httpsClient.stop();
  
  // * Response
  int from = page.indexOf("{");
  if(from!=-1){
    int to = page.indexOf("}",from+1);
    page = page.substring(from,to+1);
  }
  else {
    pl("[WARNING] no Json ... error? ");
  }
  return page;
}



void get_access_token(String code)
{
  pl("Connecting to spotify https server");
  WiFiClientSecure httpsClient;
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);
  delay(1000);
  int r=0;
  int retries = 10;
  while((!httpsClient.connect(authentication_host, httpsPort)) && (r < retries)){
      delay(100);     r++;   p(".");
  }
  if(r==retries) {
    pl("Connection failed");
  }
  else {
    pl("\tConnection established");
  }
  
  // * https POST
  String body = String("") +"grant_type=authorization_code"+
                 "&redirect_uri=" +redirectEscape +
                 "&code="+ code+
                 "&client_id=" +clientId +
                 "&client_secret=" +clientSecret;
  String body_length = String(body.length());
//  pl(" BOdy = "); pl(body);

  // * GET & POST request formats : 
  //      https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html
  String post = String("POST") +" /api/token HTTP/1.1\r\n" 
               +"Host: " + authentication_host + "\r\n" 
               +"Content-Type: application/x-www-form-urlencoded"+ "\r\n" 
               +"Content-Length: "+ body_length+ "\r\n\r\n" 
               +body+ "\r\n"
               "Connection: close\r\n\r\n";
  
  pl(" Sending POST REQUEST : ");  pl(post);
  httpsClient.print(post);
  
  pl("\n======= headers");
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    p(line);
    if (line == "\r") {
      break;
    }
  }
  
  String page = "";
  while(httpsClient.available()){        
    String line = httpsClient.readStringUntil('\n'); 
    page += line;
  }
  httpsClient.stop();
  
  pl("\n\n======== page");
  pl(page);
  
  
  // * extract access token
  String access_key = String("\"access_token\"");
  int access_index = page.indexOf(access_key);
  if(access_index!=-1){
    int from = access_index +access_key.length() +2;
    int to = page.indexOf("\"",from+1);
    String access_token = page.substring(from,to);

    pl("Access token received! ");
//    pl(access_token);
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
  
  // * save to Memory
  EEPROM.put(TOKEN_ADDRESS, token_data);
  EEPROM.commit();
  MUST_VALIDATE = 0;

  String html = String("<!DOCTYPE HTML><html><body>")+
             "<div>" +"I've store the new tokens for you, go "
             +"<a href=\"/\">back to main</a>" 
             +"</div>"
             +"</body></html>"; 
             
  server.send(200,"text/html", html);
  
  // * Eo func
}





//=======================================================================
//                    Main Program Loop
//=======================================================================


void loop() {
  delay(100);
  server.handleClient();
  // *
}



// *

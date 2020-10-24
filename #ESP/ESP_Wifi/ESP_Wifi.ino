/*
 * Connecting to wifi and setting up server
 * 
 * Async server
 *    https://github.com/me-no-dev/ESPAsyncWebServer
 *  
 *  HTTPClient
 *    https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient
 *    
 * Spotify Authorization guide
 *    https://developer.spotify.com/documentation/general/guides/authorization-guide/#authorization-code-flow
 *  
 */

 
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
//#include <b64.h>
//#include <HttpClient.h>
#include <ESP8266HTTPClient.h>



#define p(x)    Serial.print(x)
#define pl(x)    Serial.println(x)


const char* ssid     = "Radnage";
const char* password = "Qu1ckBr0wn";
const char* clientId     = "954d83a67fde4133bcf00cc20bb618b3";
const char* clientSecret = "9f6cc5fdeff24200b36c3f7d40024f1c";
String redirectURI = "http://192.168.68.119/callback";
String redirectEscape = redirectURI;

AsyncWebServer server(80);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  // * Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    p(".");
  }
  pl();

  p("Connected, IP address: ");
  pl(WiFi.localIP());
  
  // * Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "Hello!" );
  });

  server.on("/authorize", HTTP_GET, [](AsyncWebServerRequest *request){
    redirectEscape = redirectURI;
    redirectEscape.replace(":","%3A");
    redirectEscape.replace("/","%2F");
    String req = "https://accounts.spotify.com/authorize";
    req += "?client_id=";
    req += clientId;
    req += "&response_type=code";
    req += "&redirect_uri=";
    req += redirectEscape;
    req += "&scope=user-read-private%20user-read-email";
    req += "&state=esp1234";
    String msg = String(" please open") +req;
    request->send_P(200, "text/plain", msg.c_str() );
  });

  // URL format http://192.168.68.119/callback?code=....&state=esp1234
  server.on("/callback", HTTP_GET, [](AsyncWebServerRequest *request){
    String code;
    if(request->hasParam("code")){
      code = request->getParam("code")->value();
      p(" found authorization code "); pl(code);
      String msg = " Authorization code ";
      msg += code;
      request->send_P(200, "text/plain", msg.c_str() );
      pl(" now lets get the access token");
      get_access_token(code);
    }
    else{
      request->send_P(200, "text/plain", "Error, no token, please try again" );
    }
  });
  
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Are you lost");
  });
  //  server.onNotFound(NULL); // remove handler
  server.begin();
  p(" server running");
  
  // Eo setup
}

void loop() {
  // put your main code here, to run repeatedly:

}

void get_access_token(String code)
{
  
  String tokenURI = "https://accounts.spotify.com/api/token";
  HTTPClient http;
  http.begin(tokenURI);
//  http.addHeader("Content-Type", "text/plain"); 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
  //  http.addHeader("grant_type", "authorization_code"); 
  //  http.addHeader("code", code.c_str() ); 
  //  http.addHeader("redirect_uri", redirectEscape.c_str() ); 

  String req = "/";
  req += "?grant_type=authorization_code";
  req += "&code=";
  req += code;
  req += "&redirect_uri=";
  req += redirectEscape;

  int httpCode = http.POST(req.c_str());
  if(httpCode>0){
    pl("posted");
    if (httpCode == HTTP_CODE_OK) {
      pl(" received response ");
      pl( http.getString() );
    }
    
  }
  else{
    pl(" post fail ");
    pl(http.errorToString(httpCode).c_str() );
  }

  
  

}

//const char index_html[] PROGMEM = R"rawliteral( 

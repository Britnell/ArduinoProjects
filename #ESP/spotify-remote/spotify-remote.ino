/**
 
 Ex from
 https://github.com/ThingPulse/esp8266-spotify-remote
 
 */

#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
//#include <MiniGrafx.h>
//#include <ILI9341_SPI.h>
#include <FS.h>
//#include <JPEGDecoder.h>
#include "SpotifyClient.h"
#include "settings.h"
//#include "TouchControllerWS.h"
//#include "images.h"

 // Statements like:
 // #pragma message(Reminder "Fix this problem!")
 // Which will cause messages like:
 // C:\Source\Project\main.cpp(47): Reminder: Fix this problem!
 // to show up during compiles. Note that you can NOT use the
 // words "error" or "warning" in your reminders, since it will
 // make the IDE think it should abort execution. You can double
 // click on these messages and jump to the line in question.
 //
 // see https://stackoverflow.com/questions/5966594/how-can-i-use-pragma-message-so-that-the-message-points-to-the-filelineno
 //

#define PROGMEM   ICACHE_RODATA_ATTR
//#define ICACHE_RODATA_ATTR  __attribute__((section(".irom.text")))

#define Stringize( L )     #L 
#define MakeString( M, L ) M(L)
#define $Line MakeString( Stringize, __LINE__ )
#define Reminder __FILE__ "(" $Line ") : Reminder: "
#ifdef LOAD_SD_LIBRARY
#pragma message(Reminder "Comment out the line with LOAD_SD_LIBRARY /JPEGDecoder/src/User_config.h !")
#endif


const char* host = "api.spotify.com";
const int httpsPort = 443;

int BUFFER_WIDTH = 240;
int BUFFER_HEIGHT = 160;
// Limited to 4 colors due to memory constraints
int BITS_PER_PIXEL = 2; // 2^2 =  4 colors

SpotifyClient client(clientId, clientSecret, redirectUri);
SpotifyData data;
SpotifyAuth auth;

String currentImageUrl = "";
uint32_t lastDrawingUpdate = 0;
uint16_t counter = 0;
long lastUpdate = 0;
TS_Point lastTouchPoint;
uint32_t lastTouchMillis = 0;
boolean isDownloadingCover = false;

String formatTime(uint32_t time);
void saveRefreshToken(String refreshToken);
String loadRefreshToken();
void displayLogo();

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
  if (!MDNS.begin("esp8266")) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

  
  String code = "";
  String grantType = "";
  String refreshToken = "AQC3neaealb6Jld3v8nVWhggmXbfnFFbM32f-_xCGBwZBgZAnG5ekxa4W9ZO4bkVAQL4bvfJbxck9zju1s0dAR3Xb7AgefQyvn94RKjUkybWYYmyjlsS6yPm500xn86Dcg8";
  if (refreshToken == "") {
    Serial.println("No refresh token found. Requesting through browser");
    Serial.println ( "Open browser at http://" + espotifierNodeName + ".local" );
  
    gfx.drawString(120, 10, "Authentication required.\nOpen browser at\nhttp://" + espotifierNodeName + ".local");
    gfx.commit(0, 0);

    code = client.startConfigPortal();
    grantType = "authorization_code";
  } else {
    Serial.println("Using refresh token found on the FS");
    code = refreshToken;
    grantType = "refresh_token";
  }
  client.getToken(&auth, grantType, code);
  Serial.printf("Refresh token: %s\nAccess Token: %s\n", auth.refreshToken.c_str(), auth.accessToken.c_str());
  if (auth.refreshToken != "") {
//    saveRefreshToken(auth.refreshToken);
    Serial.println("you should save this new token");
  }
//  client.setDrawingCallback(&drawSongInfoCallback);
}

void loop() {


    if (millis() - lastUpdate > 1000) {
      uint16_t responseCode = client.update(&data, &auth);
      Serial.printf("HREF: %s\n", data.image300Href.c_str());
      lastUpdate = millis();
      
      Serial.printf("--------Response Code: %d\n", responseCode);
      Serial.printf("--------Free mem: %d\n", ESP.getFreeHeap());
      
      if (responseCode == 401) {
        client.getToken(&auth, "refresh_token", auth.refreshToken);
        if (auth.refreshToken != "") {
          saveRefreshToken(auth.refreshToken);
        }
      }
      if (responseCode == 200) {

 
      }
      if (responseCode == 400) {
        Serial.println(" RESP CODE 400 - please define clientID & secret correct");
      }

    }
    
    drawSongInfo();
    
    

}

void drawSongInfo() {
  
  Serial.printf("Song %s of %s , Name: %s - %s , Activity %s, %s ", data.progressMs, data.durationMs, data.title, data.artistName, data.isPlaying, data.isPlayerActive);
}

void drawProgress(uint64_t progressMs, uint64_t durationMs, String songTitle, String artistName, boolean isPlaying, boolean isPlayerActive) {
  counter++;

  if (isDownloadingCover) {
      gfx.fillBuffer(MINI_BLACK);
      gfx.setColor(MINI_WHITE);
      gfx.drawCircle(100, 80, 5);
      gfx.drawCircle(120, 80, 5);
      gfx.drawCircle(140, 80, 5);
      gfx.fillCircle(100 + 20 * (counter % 3), 80, 5);
      gfx.commit(0,0);
  }
  
  gfx.fillBuffer(MINI_BLACK); 
  if (isPlayerActive) {
    
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
       
    uint8_t percentage = 100.0 * progressMs / durationMs;
    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.drawString(10, 70, formatTime(progressMs));
    gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
    gfx.drawString(230, 70, formatTime(data.durationMs));
    gfx.setColor(MINI_WHITE);
    gfx.setColor(MINI_GRAY);
    uint16_t progressX = 10 + 220 * percentage / 100;
    uint16_t progressY = 65;
    gfx.drawLine(progressX, progressY, 230, progressY);
    gfx.setColor(MINI_WHITE);
    gfx.drawLine(10, progressY, progressX, progressY);
    gfx.fillCircle(progressX, progressY, 5);
  
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    String animatedTitle = songTitle;
    uint8_t maxChar = 30;
    uint8_t excessChars = songTitle.length() - maxChar;
    uint8_t currentPos = counter % excessChars;
    if (songTitle.length() > maxChar) {
      animatedTitle = songTitle.substring(currentPos, currentPos + maxChar);
    }
    gfx.setTextAlignment(TEXT_ALIGN_CENTER);
    gfx.drawString(120, 10, animatedTitle);
    gfx.setColor(MINI_GRAY);
    gfx.drawString(120, 30, artistName);
    gfx.setColor(MINI_YELLOW);
    gfx.drawPalettedBitmapFromPgm(88, 90, isPlaying ?  miniPause : miniPlay);

    gfx.drawPalettedBitmapFromPgm(30, 90, miniPrevious);
    gfx.drawPalettedBitmapFromPgm(190, 90, miniNext);
    if (millis() - lastTouchMillis < SHOW_TOUCH_POINT_MILLIS) {
      gfx.setColor(MINI_YELLOW);
      gfx.fillCircle(lastTouchPoint.x, lastTouchPoint.y - 160, 5);
    }
    gfx.commit(0, 160);
  } else {
    displayLogo();
  }

}

void displayLogo() {
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setFont(ArialMT_Plain_16);
  gfx.setColor(MINI_YELLOW);
  gfx.drawPalettedBitmapFromPgm(20, 20, ThingPulseLogo);
  gfx.drawString(120, 110, "ESPotify-Remote");
  gfx.commit(0, 0);
}

void calibrationCallback(int16_t x, int16_t y) {
  gfx.fillBuffer(MINI_BLACK);

  gfx.drawString(120, 10, "Please calibrate\ntouch screen by\ntouch point");
  gfx.setColor(MINI_WHITE);
  if (y < BUFFER_HEIGHT) {
    gfx.fillCircle(x, y, 10);
    gfx.commit(0, 0);
    gfx.fillBuffer(MINI_BLACK);
    gfx.commit(0, BUFFER_HEIGHT);
  } else {
    gfx.fillCircle(x, y - BUFFER_HEIGHT, 10);
    gfx.commit(0, BUFFER_HEIGHT);
    gfx.fillBuffer(MINI_BLACK);
    gfx.commit(0, 0);
  }
}

String formatTime(uint32_t time) {
  char time_str[10];
  uint8_t minutes = time / (1000 * 60);
  uint8_t seconds = (time / 1000) % 60;
  sprintf(time_str, "%2d:%02d", minutes, seconds);
  return String(time_str);
}

void saveRefreshToken(String refreshToken) {
  
  File f = SPIFFS.open("/refreshToken.txt", "w+");
  if (!f) {
    Serial.println("Failed to open config file");
    return;
  }
  f.println(refreshToken);
  f.close();
}

String loadRefreshToken() {
  String ret;
  ret = String( FPSTR(refreshTokenMEM) );
  return ret;
}

void drawJPEGFromSpiffs(String filename, int xpos, int ypos) {
  char buffer[filename.length() + 1];
  filename.toCharArray(buffer, filename.length() + 1);
  
  JpegDec.decodeFile(buffer);
  uint8_t zoomFactor = 2;
  uint16_t  *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  Serial.printf("MCU W/H: %d, %d\n", mcu_w, mcu_h);
  // uint32_t mcu_pixels = mcu_w * mcu_h; // total pixels
  // TODO immplmenet something to track drawtime performance
  // uint32_t drawTime = millis();

  while( JpegDec.read()){
    
    pImg = JpegDec.pImage;
    int mcu_x = (JpegDec.MCUx * mcu_w) / zoomFactor + xpos;
    int mcu_y = (JpegDec.MCUy * mcu_h) / zoomFactor + ypos;
    //if ( ( mcu_x + mcu_w) <= tft_->width() && ( mcu_y + mcu_h) <= tft_->height()){
      
      tft.setAddrWindow(mcu_x, mcu_y, mcu_x + (mcu_w / zoomFactor) - 1, mcu_y + (mcu_h / zoomFactor) - 1);
      // uint32_t count = mcu_pixels; // what was this for?
      
      for (uint8_t y = 0; y < mcu_h; y++) {
        for (uint8_t x = 0; x < mcu_w; x++) {
          if (x % zoomFactor == 0 && y % zoomFactor == 0) {
            tft.pushColor(*pImg);
          }
          *pImg++;
        }
      }
      drawSongInfo();
  
  }

}


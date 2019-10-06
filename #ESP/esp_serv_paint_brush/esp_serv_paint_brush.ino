/*********

  to add :
   - vary hue in cycles +- 5 or so
   - send all 3 vars at once via POST ?&x=1&y=2
   - add extra leds?
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <NeoPixelPainter.h>


                                                        // ** Neopixel
#define PIN     D2
#define PIXELS  120

Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(PIXELS, PIN, NEO_RGB + NEO_KHZ800);
NeoPixelPainterCanvas pixelcanvas = NeoPixelPainterCanvas(&neopixels); //create canvas, linked to the neopixels (must be created before the brush)
NeoPixelPainterBrush pixelbrush = NeoPixelPainterBrush(&pixelcanvas); //crete brush, linked to the canvas to paint to


// Replace with your network credentials
const char* ssid = "KDG-3798D";
const char* password = "fA4cXNhfB5uX";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";


// Assign output variables to GPIO pins
const int output5 = D0;

int speedVar = 520;
int fadeVar = 12;
int hueVar = 159;

void setup() {
  Serial.begin(115200);
  
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  
  
  // 
  neopixels.begin();
  //

  if (pixelcanvas.isvalid() == false) Serial.println("canvas allocation problem");
  else  Serial.println("canvas allocation ok");
  
  if (pixelbrush.isvalid() == false) Serial.println("brush allocation problem");
  else  Serial.println("brush allocation ok");
  
  for(int x=0; x<PIXELS; x++) {
    neopixels.setPixelColor( x, neopixels.Color(0, 1, 5) );
  }
  neopixels.show();
  Serial.println("\n##\nNeopixel strip intialized");
  
  
  // turn on brush
  pixelbrush.setSpeed(speedVar);     // 4096 = one pixel per update
  pixelbrush.setFadeSpeed(fadeVar);  // 255 max
  pixelbrush.setFadein(true);
  pixelbrush.setFadeout(true);
  pixelbrush.setBounce(true);
    HSV brushcolor;
  brushcolor.h = hueVar;  // 0-255   ;  0=red
  brushcolor.s = 255; // 255 = 100%
  brushcolor.v = 40;  // 255 = brightness
  pixelbrush.setColor(brushcolor);
  

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}



void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if( header.indexOf("/speed/") >= 0) {
              int x = header.indexOf("/speed/") +7;
              String val;
              while( isDigit(header.charAt(x)) ) {
                val += header.charAt(x);
                x++;
              }
              //Serial.print("received integer : " );               Serial.println( val.toInt() );
              speedVar = val.toInt();
              pixelbrush.setSpeed(speedVar);
            }
            else if( header.indexOf("/fade/") >= 0) {
              int x = header.indexOf("/fade/") +6;
              String val;
              while( isDigit(header.charAt(x)) ) {
                val += header.charAt(x);
                x++;
              }
              //Serial.print("Received int : " );Serial.println( val.toInt() );
              fadeVar = val.toInt();
              pixelbrush.setFadeSpeed(fadeVar);
            }
            else if(  header.indexOf("/hue/") >= 0) {
              int x = header.indexOf("/hue/") +5;
              String val;
              while( isDigit(header.charAt(x)) ) {
                val += header.charAt(x);
                x++;
              }
              //Serial.print("Received int : " );              Serial.println( val.toInt() );
              hueVar = val.toInt();
              HSV brushcolor;
              brushcolor.h = hueVar;  // 0-255   ;  0=red
              brushcolor.s = 255; // 255 = 100%
              brushcolor.v = 40;  // 255 = brightness
              pixelbrush.setColor(brushcolor);
  
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            
            // If the output4State is off, it displays the ON button       
            
            // Adding speed slider
            client.println("<p><label for=\"speed\">Speed : " +String(speedVar) +"</label>");
            client.println("<input type=\"range\" value=\"" +String(speedVar) +"\" id=\"speed\" onchange=\"speedFunc()\" min=\"1\" max=\"4000\"></p>");
            client.println("<script  type=\"text/javascript\"> function speedFunc() { ");
            client.println("val=document.getElementById('speed').value;  ");
            client.println("window.location.href='/speed/'+val.toString();");
            client.println("} </script>" );

            // fade slider
            client.println("<p><label for=\"fade\">Fade speed: " +String(fadeVar) +"</label>");
            client.println("<input type=\"range\" min=\"1\" max=\"255\" value=\"" +String(fadeVar) +"\" id=\"fade\" onchange=\"fadeFunc()\"></p>");
            client.println("<script  type=\"text/javascript\"> function fadeFunc() { ");
            client.println("val=document.getElementById('fade').value;  ");
            client.println("window.location.href='/fade/'+val.toString();");
            client.println("} </script>" );

            // hue slider
            client.println("<p><label for=\"hue\">Hue : " +String(hueVar) +"</label>");
            client.println("<input type=\"range\" min=\"0\" max=\"255\" value=\"" +String(hueVar) +"\" id=\"hue\" onchange=\"hueFunc()\"></p>");
            client.println("<script  type=\"text/javascript\"> function hueFunc() { ");
            client.println("val=document.getElementById('hue').value;  ");
            client.println("window.location.href='/hue/'+val.toString();");
            client.println("} </script>" );
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");

    // Eo if(client)
  }

  neopixels.clear();
  pixelbrush.paint();
  pixelcanvas.transfer();
  neopixels.show();
  // * Eo loop
}



/*********
  Rui Santos
    tutorial
  https://randomnerdtutorials.com/esp8266-web-server/
    Git
  https://raw.githubusercontent.com/RuiSantosdotme/Random-Nerd-Tutorials/master/Projects/ESP8266_Web_Server_Arduino_IDE_updated.ino
  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>


                                                        // ** Neopixel
#define PIN     D2
#define PIXELS  25

Adafruit_NeoPixel neoStrip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);


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


void setup() {
  Serial.begin(115200);
  
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);

  
  // 
  neoStrip.begin();
  for(int x=0; x<PIXELS; x++) {
    neoStrip.setPixelColor( x, neoStrip.Color(0, 1, 5) );
  }
              neoStrip.show();
  Serial.println("\n##\nNeopixel strip intialized");
  
  

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

int slideVar = 10;

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
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 0 HIGH");
              output5State = "OFF";
              digitalWrite(output5, HIGH);
              
            } 
            else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 0 LOW");
              output5State = "ON";
              digitalWrite(output5, LOW); 
            }
            else if( header.indexOf("/slider/") >= 0) {
              int x = header.indexOf("/slider/") +8;
              String val;
              while( isDigit(header.charAt(x)) ) {
                val += header.charAt(x);
                x++;
              }
              //Serial.print("int string is : " + val+ "\n as digit is : " );
              //Serial.println( val.toInt() );
              slideVar = val.toInt();
              for(int x=0; x<PIXELS; x++) {
                neoStrip.setPixelColor( x, neoStrip.Color(0, slideVar>>4, slideVar) );
              }
              neoStrip.show();
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
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="OFF") {
              client.println("<p><a href=\"/5/off\"><button class=\"button\">LED ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/on\"><button class=\"button button2\">LED OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            
            // If the output4State is off, it displays the ON button       
            
            // Adding a slider
            client.println("<p><label for=\"slider\">Volume : " +String(slideVar) +"</label>");
            client.println("<input type=\"range\" min=\"0\" max=\"128\" value=\"" +String(slideVar) +"\" id=\"slider\" step=\"1\" oninput=\"slideFunc()\"></p>");
            client.println("<script  type=\"text/javascript\"> function slideFunc() { ");
            client.println("val=document.getElementById('slider').value;  ");
            client.println("window.location.href='/slider/'+val.toString();");
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
  }
}



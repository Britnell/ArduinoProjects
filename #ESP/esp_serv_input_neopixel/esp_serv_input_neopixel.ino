

/*********
  A simple html server on NodeMCU that lets you send different variables to control different states etc.
    - NodeMCU connects to your Wifi and pritns its IP address over Serial Connection
    - I added NeoPixel to have an example output device. 
    - visit website for

    * Button to toggle LED state
    * Input dropdown box to change state. auto trigger on each change 
        [ this could be used to play different animations on Neopixel   ]
    * input slider element to adjust variable. 
        [ In this case this set the LED brightness        ]
    *
  
  I  used Rui santos project as a starting point.
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>


// Replace with your network credentials
const char* ssid = "NextEV_Guest";
const char* password = "Ni@gu3st";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = D0;
const int output4 = 4;

char state = ' ';


#define PIN         D3
#define NUMPIXELS   16

Adafruit_NeoPixel NeoPixel = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



void lights_off( uint8_t showz){
  for( int i=0; i<16; i++ )
    NeoPixel.setPixelColor(i, NeoPixel.Color( 0, 0, 0 ) );
  if(showz)
    NeoPixel.show();
  //
}



uint8_t is_digit( char X ){
  X -= 48;

  if( X >= 0 && X <= 9 )
    return 1;
  else
    return 0;
}

void setup() 
{
  
  Serial.begin(115200);

  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  //  pinMode(output4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  //  digitalWrite(output4, LOW);
  Serial.println("Outputs cinfigured, intializing Neopixels");


  // Neopixel
  NeoPixel.begin(); // This initializes the NeoPixel library.
  
  // Test sequence
  NeoPixel.setPixelColor(0, NeoPixel.Color( 0, 0, 10 ) );
  NeoPixel.setPixelColor(1, NeoPixel.Color( 0, 5, 5 ) );
  NeoPixel.setPixelColor(2, NeoPixel.Color( 0, 10, 0 ) );
  NeoPixel.setPixelColor(3, NeoPixel.Color( 5, 5, 0 ) );
  NeoPixel.setPixelColor(4, NeoPixel.Color( 10, 0, 0 ) );
  NeoPixel.setPixelColor(5, NeoPixel.Color( 5, 0, 5 ) );
  NeoPixel.show();

  
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

uint8_t slideVal = 0;

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("###\n New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    //Serial.println("Received request header : ");
    
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            Serial.println("---\nReceived request header : ");
            Serial.println(header );

            
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } 
            else if(header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } 

            // state link
            if(header.indexOf("/state/") >= 0) {
              int posA = header.indexOf("/state/");
              char stateA = header.charAt(posA +7);
              
              // A 
              Serial.print(" # new char = ");
              Serial.println(stateA);
              state = stateA;
              Serial.print(" # new State = ");
              Serial.println(state);
              
              // do LEDs for state
              if(stateA=='a')
              {
                NeoPixel.setPixelColor(0, NeoPixel.Color( 0, 0, 10 ) );
                NeoPixel.setPixelColor(1, NeoPixel.Color( 0, 5, 5 ) );
                NeoPixel.setPixelColor(2, NeoPixel.Color( 0, 10, 0 ) );
                NeoPixel.setPixelColor(3, NeoPixel.Color( 5, 5, 0 ) );
                NeoPixel.setPixelColor(4, NeoPixel.Color( 10, 0, 0 ) );
                NeoPixel.setPixelColor(5, NeoPixel.Color( 5, 0, 5 ) );
                NeoPixel.show();
              }
              
              if(stateA== 'b' ) {
                // B
                NeoPixel.setPixelColor(0, NeoPixel.Color( 100, 0, 0 ) );
                NeoPixel.setPixelColor(1, NeoPixel.Color( 60, 10, 0 ) );
                NeoPixel.setPixelColor(2, NeoPixel.Color( 10, 10, 0 ) );
                NeoPixel.setPixelColor(3, NeoPixel.Color( 5, 5, 0 ) );
                NeoPixel.setPixelColor(4, NeoPixel.Color( 60, 10, 0 ) );
                NeoPixel.setPixelColor(5, NeoPixel.Color( 100, 10, 0 ) );
                NeoPixel.show();
              } 
              
              if(stateA == 'c') {
                // CN
                NeoPixel.setPixelColor(0, NeoPixel.Color( 0, 0, 80 ) );
                NeoPixel.setPixelColor(1, NeoPixel.Color( 20, 20, 20 ) );
                NeoPixel.setPixelColor(2, NeoPixel.Color( 0, 0, 80 ) );
                NeoPixel.setPixelColor(3, NeoPixel.Color( 20, 20, 20 ) );
                NeoPixel.setPixelColor(4, NeoPixel.Color( 0, 0, 80 ) );
                NeoPixel.setPixelColor(5, NeoPixel.Color( 20, 20, 20 ) );
                NeoPixel.show();
              } 
              
              if(stateA == 'd') {
                // D 
                NeoPixel.setPixelColor(0, NeoPixel.Color( 1, 0, 0 ) );
                NeoPixel.setPixelColor(1, NeoPixel.Color( 0, 1, 0 ) );
                NeoPixel.setPixelColor(2, NeoPixel.Color( 0, 0, 1 ) );
                NeoPixel.setPixelColor(3, NeoPixel.Color( 1, 0, 0 ) );
                NeoPixel.setPixelColor(4, NeoPixel.Color( 0, 1, 0 ) );
                NeoPixel.setPixelColor(5, NeoPixel.Color( 0, 0, 1 ) );
                NeoPixel.show();
              }
              //-
            }
            if(header.indexOf("/slide/") >= 0){
              int posA = header.indexOf("/slide/");
              int lenA = header.length();
              
              posA += 7;
              Serial.print("--Slider!\n A: ");
              Serial.print(posA);
              Serial.print(" , char is ");
              Serial.print(header.charAt(posA));
              
              String slideInt = "";
              
              while( is_digit( header.charAt(posA)) ) { 
                slideInt += header.charAt( posA);
                posA++;
                //  
              }
              
              Serial.print( "extracted int : ");
              Serial.print(slideInt);
              //Serial.print("  , as int : ");
              //Serial.println( slideInt.toInt() );
              slideVal = slideInt.toInt();
              
              NeoPixel.setPixelColor(7, NeoPixel.Color( 0, slideVal<<4, slideVal ) );
              NeoPixel.setPixelColor(8, NeoPixel.Color( 0, slideVal<<4, slideVal ) );
              NeoPixel.setPixelColor(9, NeoPixel.Color( 0, slideVal<<4, slideVal ) );
              NeoPixel.show();
              //
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
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } 
            else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 

            client.println("<p> Now lets try some inputs </p>");

            client.print("<p> The current state is : " );
            client.print(state);
            client.println("</p>");
            
            client.print("<p>  <select id=\"drop\" onchange=\"dropfunc()\" size=\"6\">");
            client.print("<option value=\" \">  </option>");

            
            //if(state.equals("a") )
            if( state == 'a' )
              client.print("<option value=\"a\" selected>A</option>");
            else
              client.print("<option value=\"a\">A</option>");

            //if(state.equals("b") )
            if( state == 'b' )
              client.print("<option value=\"b\" selected >B</option>");
            else 
              client.print("<option value=\"b\">B</option>"); 

            //if(state.equals("c") )
            if( state == 'c' )
              client.print("<option value=\"c\" selected>C</option>");
            else
              client.print("<option value=\"c\">C</option>");

            //if(state.equals("d") )
            if( state == 'd' )
              client.print("<option value=\"d\" selected >D</option></select>");
            else
              client.print("<option value=\"d\">D</option></select>");
            
            client.print("<script>function dropfunc() {  var sel = document.getElementById(\"drop\").value;  window.location.assign('/state/' +sel);  }   </script>" );
            client.println("</p>");
            
            client.println("<p> No how about a slider?? </p> ");
            client.print("<p> Slider value is :    ");
            client.print( slideVal );
            
            client.println("<br> <input id=\"slider\" type=\"range\" min=\"0\" max=\"255\" value=\"" +String(slideVal) +"\" onchange=\"slidefunc()\"> </p>");
            client.println("<script>function slidefunc() {  var sel = document.getElementById(\"slider\").value;  window.location.assign('/slide/' +sel);  }   </script>" );
            
            
            // Display current state, and ON/OFF buttons for GPIO 4  
//            client.println("<p>GPIO 4 - State " + output4State + "</p>");
            // If the output4State is off, it displays the ON button       
//            if (output4State=="off") {
//              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
//            }
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



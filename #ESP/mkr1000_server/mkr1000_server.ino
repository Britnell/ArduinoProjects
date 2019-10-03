/*
  WiFi Web Server LED Blink
  
 by Tom Igoe
 */
#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "toolbox";        // 
char pass[] = "Toolbox.Torino";    

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);      // initialize serial communication
  delay(1000);
  Serial.print(" Boooot");
  
  pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWiFiStatus();                        // you're connected now, so print out the status
}

String header = "";
String LEDstate = "off";

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) 
      {
        char c = client.read();             // read a byte, then
        header += c;
//        Serial.write(c);                  
        if (c == '\n') {
          if (currentLine.length() == 0) {
            //if (currentLine.endsWith("GET /H")) {
            if (header.indexOf("LEDON")!=-1) {
              LEDstate = "on";
              digitalWrite(LED_BUILTIN, HIGH);     
            }
            if (header.indexOf("LEDOFF")!=-1) {
              LEDstate = "off";
              digitalWrite(LED_BUILTIN, LOW);  
            }
            Serial.print("\n = Header :\t"); Serial.print(header);Serial.print(" = \n");
            header = "";
            client.println("\nHTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            //    ****    Serve HTML Page

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            //    **    CSS 
            
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            //     **    Web Page Heading
            
            client.println("<body><h1>Arduino Web Interface</h1>");
            
            //    **  ON OFF BUTTONs
            
            client.println("<p> LED is " + LEDstate + "</p>");
            
//            if (LEDstate=="off") {
              client.println("<p><a href=\"/LEDON\"><button class=\"button\">ON</button></a></p>");
              client.println("<p><a href=\"/LEDOFF\"><button class=\"button button2\">OFF</button></a></p>");
            
            client.println("</body></html>");
            client.println();
            break;
          }
          else {      
            currentLine = "";
          }
          // c == \n
        }
        else if (c != '\r') {   
          currentLine += c;     
        }
        //  **  Eo client available
      }
      
      // Eo while connected
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

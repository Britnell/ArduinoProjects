/**
 * An example showing how to put ESP8266 into Deep-sleep mode
 * https://www.losant.com/blog/making-the-esp8266-low-powered-with-deep-sleep
 * 
 *  Hardware :
 *     Connect pin D0 to RST
 */
 
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while(!Serial) { }

  Serial.println("\n\nI'm awake.(reset)");
  
  Serial.println("Going into deep sleep for 20 seconds");
  ESP.deepSleep(10e6); // 20e6 is 20 microseconds
}

void loop() {
}

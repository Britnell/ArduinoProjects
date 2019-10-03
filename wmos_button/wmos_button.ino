
#define dela     100


int ledpin = LED_BUILTIN;   // D4 = GPIO2
int buttonpin = 4;          // D2 = GPIO4

uint8_t initialised = 0;
uint8_t buttonState=0;


void setup() {
  //
  pinMode(ledpin, OUTPUT);
  
  pinMode(buttonpin, INPUT);     
  
  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for serial port to connect
  }

  Serial.println("Welcome to ze Blinkz");
  Serial.print("Blinking LED on pin : ");
  Serial.println(ledpin);
  Serial.print("Button LED on pin : ");
  Serial.println(ledpin);
  delay(100);
  //
}


void loop() {
  // *
  
//  Serial.println( digitalRead(buttonpin) );
  
  if(!digitalRead(buttonpin)){
    digitalWrite(ledpin, HIGH);  
  }
  else {
    digitalWrite(ledpin, LOW);    
  }
  delay(50);
  
  //Serial.println(millis() );
  
  // Eo loop
}



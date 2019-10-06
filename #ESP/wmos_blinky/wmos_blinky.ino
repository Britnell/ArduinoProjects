
#define dela     100


int ledpin = LED_BUILTIN;   //D4 = GPIO2

uint8_t initialised = 0;

void setup() {
  //
  pinMode(ledpin, OUTPUT);

  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for serial port to connect
  }

  Serial.println("Welcome to ze Blinkz");
  Serial.print("Blinking LED on pin : ");
  Serial.println(ledpin);
  delay(100);
  //
}


void loop() {
  // *
  digitalWrite(ledpin, HIGH);
  delay(dela);
  digitalWrite(ledpin, LOW);
  delay(dela);

  Serial.println(millis() );

  //  if(Serial.available() >0){
  //    char newled = Serial.read()-48;
  //    ledpin = newled;
  //    pinMode(ledpin,OUTPUT);
  //    Serial.print("new led : ");
  //    Serial.println(ledpin);
  //  }

  // Eo loop
}



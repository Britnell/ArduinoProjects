

#define SET(reg, pin)   reg |= (1<<pin)
#define CLR(reg, pin)   reg &= ~(1<<pin)

// LED on pin
//   PB5 //  D13
#define LED_pin   5
#define LED_port  PORTB
#define LED_ddr   DDRB

void setup() {
  Serial.begin(115200);
  
  // put your setup code here, to run once:
  SET(LED_ddr,  LED_pin);
  Serial.println("Let's begin");
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  SET(LED_port, LED_pin);
  delay(500);
  CLR(LED_port, LED_pin);
  delay(500);
}

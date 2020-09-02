
#include "LowPower.h"

uint8_t relay_pin = 15;
int dellay = 5000;

uint8_t relay_state = 0;

void setup() {
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, 0);

  delay(1000);
}

void loop() {

  // * Blink
//  digitalWrite(relay_pin, 1);
//  delay(dellay);
//  digitalWrite(relay_pin, 0);
//  delay(dellay);
  
  // SLEEP_1S , _2S, _4S, _8S 
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
  
  relay_state = !relay_state;
  digitalWrite(relay_pin, relay_state);
  
  // Eo loop
}

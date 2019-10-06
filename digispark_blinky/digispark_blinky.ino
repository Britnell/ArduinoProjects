/*
 *  Digispark board Json : http://digistump.com/package_digistump_index.json
 *    board : Digispark ( Default 16MHz )
 */

void setup() {
  // put your setup code here, to run once:
  DDRB |= (1<<1);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  PORTB |= (1<<1);
  delay(300);
  PORTB &= ~(1<<1);
  delay(300);
}

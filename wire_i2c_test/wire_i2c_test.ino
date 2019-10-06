#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Wire.begin();
  Serial.println("Wire connected");
}

byte val = 0;

void loop() {
  // put your main code here, to run repeatedly:

  Wire.beginTransmission(44);
  Wire.write(byte(0x00));
  Wire.write(val);
  Wire.endTransmission();

  val++;
  if( val == 64) 
    val = 0;

  delay(500);
  //
}

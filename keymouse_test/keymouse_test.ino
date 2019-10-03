#include "Keyboard.h"
#include "Mouse.h"

// set pin numbers for the five buttons:

void setup() { // initialize the buttons' inputs:

  Serial.begin(9600);

  // initialize mouse control:
  //  Mouse.begin();

  delay(1000);

  Keyboard.begin();

  delay(800);

  for (int x = 0; x < 5; x++) {
     Mouse.move(0, -100);
    Keyboard.print("x");
    delay(800);
  }
  
  delay(500);

}

int x = 0;

void loop()
{

}

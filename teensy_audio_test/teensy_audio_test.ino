#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1(A2);             //default for adc1 is A2
AudioAnalyzeFFT256      myFFT;       // AudioAnalyzeFFT1024
AudioConnection          patchCord1(adc1, myFFT);
// GUItool: end automatically generated code

void setup()
{
  AudioMemory(12);
  
//  myFFT.windowFunction(AudioWindowHanning1024);
  myFFT.windowFunction(NULL);

  
  // Eo setup
}

void loop()
{
  // - 
  
  float n;
  
  if (myFFT.available()) {
    
    Serial.print("FFT: ");
    for (int i=0; i<127; i++) {
      n = myFFT.read(i);
      if (n >= 0.05) {
        Serial.print(n);    Serial.print(" ");
      } else {
        Serial.print("  -  ");   // don't print "0.00"
      }
    }
    Serial.println();
  }

  // **    Eo loop
}


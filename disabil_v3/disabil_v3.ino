#include "MPU9250.h"
#include "Keyboard.h"

// Keys https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
// KEY_UP_ARROW    KEY_RETURN   KEY_F1    

#define SEND_KEYBOARD   1
#define BLOW_KEY    KEY_UP_ARROW
#define SHAKE_KEY   KEY_UP_ARROW
#define KEY_TIME    330

#define pt(x)   Serial.print(x)
#define pl(x)   Serial.println(x)
MPU9250 mpu;

#define ADC   A0
#define N_AVRG  10

//  **    IMU

float accLim = 2.6;
long accBounce = 0;
int accTime = 300;

// MIC
uint8_t blow = 0;
uint8_t av = 0;
int avrg[N_AVRG];
long last_average = 0;
long blow_time = 0;


void setup()
{
    Serial.begin(115200);
    delay(1000);
    
    if(SEND_KEYBOARD) Keyboard.begin();
    
    Wire.begin();
    mpu.setup();
    pt(" begin "); pl(123);
}



void loop()
{
    //  **  IMU
    mpu.update();
    
    // * ACC
    // float getAcc(uint8_t i) const { return (i < 3) ? a[i] : 0.f; }
    float x = mpu.getAcc(0);
    float y = mpu.getAcc(1);
    float z = mpu.getAcc(2);
    float mag = abs(x)+abs(y)+abs(z);
    
    if(mag>accLim){
      if(millis()-accBounce>accTime){
        pt("SHAKE ");    pl(mag);
        if(SEND_KEYBOARD){
          Keyboard.press(SHAKE_KEY);
          delay(KEY_TIME);
          Keyboard.release(SHAKE_KEY);
        }
        accBounce = millis();
      }
    }
    //-printRawData();


    //    **    MIC

    // * Read
  int vai = analogRead(ADC);
  avrg[av] = vai;
  av++;   if(av>=N_AVRG)  av = 0;

  // * Diff and check
  int diff = vai-last_average; //
  if(diff>200)
  {
    if(!blow)
    if(millis()-blow_time>400)
    {
      if(SEND_KEYBOARD)  {
        Keyboard.press(BLOW_KEY);
        delay(KEY_TIME);
        Keyboard.release(BLOW_KEY);
      }
      pt(" # BLOW "); pl(diff);
      blow = 1;
    }    
  }
  else{
    if(diff<50)
    if(blow){
      blow = 0;
      blow_time = millis();
//      Serial.println("__");
    }
  }

  // * Get average
  long average = 0;
  for(uint8_t x=0;x<N_AVRG;x++){
    average += avrg[x];
  }
  average /= N_AVRG;
  last_average = average;
  
  //delay(10);
    
  // * Eo loop
}


#include "MPU9250.h"
#include "Keyboard.h"

// Keys https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
// KEY_UP_ARROW    KEY_RETURN   KEY_F1    

#define SEND_KEYBOARD   1
#define BLOW_KEY    KEY_UP_ARROW
#define SHAKE_KEY   KEY_UP_ARROW
#define KEY_TIME    330

#define SW_PIN_L    2
#define SW_PIN_R    3

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
    pinMode(SW_PIN_L,INPUT_PULLUP);
    pinMode(SW_PIN_R,INPUT_PULLUP);
    
    Serial.begin(115200);
    delay(1000);
    
    if(SEND_KEYBOARD) Keyboard.begin();
    
    Wire.begin();
    mpu.setup();
    pt(" begin "); pl(123);
}

uint8_t sw_l=0;
uint8_t sw_r=0;

void loop()
{
    if(!digitalRead(SW_PIN_L)){
      if(!sw_l){
        pl("LEFT");
        if(SEND_KEYBOARD)   Keyboard.press(KEY_LEFT_ARROW);
        sw_l = 1;
      }
    }
    else{
      if(sw_l){
        if(SEND_KEYBOARD)   Keyboard.release(KEY_LEFT_ARROW);
        sw_l = 0;
      }
    }
    if(!digitalRead(SW_PIN_R)){
      if(!sw_r){
        pl("RIGHT");
        if(SEND_KEYBOARD)   Keyboard.press(KEY_RIGHT_ARROW);
        sw_r = 1;
      }
    }
    else{
      if(sw_r){
        if(SEND_KEYBOARD)   Keyboard.release(KEY_RIGHT_ARROW);
        sw_r = 0;
      }
    }
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
      pt("# BLOW\t");pt(last_average);   pt("\t"); pl(diff);
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


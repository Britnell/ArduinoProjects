#include "MPU9250.h"
#include "Keyboard.h"

#define SEND_KEYBOARD   0

#define pt(x)   Serial.print(x)
#define pl(x)   Serial.println(x)

// IMU
MPU9250 mpu;
float accLim = 2.6;
long accBounce = 0;
int accTime = 300;


void setup()
{
    if(SEND_KEYBOARD) Keyboard.begin();
    
    Serial.begin(115200);
    delay(800);
    
    Wire.begin();
    delay(800);
    
    mpu.setup();
    
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
        Serial.println("SHAKE");
        if(SEND_KEYBOARD)  Keyboard.press(KEY_UP_ARROW);
        accBounce = millis();
      }
    }
    //-printRawData();

    // * Eo loop
}


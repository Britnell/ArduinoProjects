#include "MPU9250.h"
#include "Keyboard.h"

#define SEND_KEYBOARD   0

MPU9250 mpu;

float thre = 45.0;

void setup()
{
    
    if(SEND_KEYBOARD) Keyboard.begin();
    
    Serial.begin(115200);

    Wire.begin();

    delay(2000);
    mpu.setup();
}

char state = 'c'; // c- center l-left r-right

void loop()
{
    static uint32_t prev_ms = millis();
    
    if ((millis() - prev_ms) > 16)
    {
        mpu.update();
//        mpu.print();
        
        //Serial.print("roll  (x-forward (north)) : ");
//        Serial.println(mpu.getRoll());
        float roll = mpu.getRoll();

        if(state=='c'){
          
          if(roll>thre){
            if(SEND_KEYBOARD)  Keyboard.press(KEY_RIGHT_ARROW);
            Serial.println("RIGHT");
            state = 'r';
          }
          else if(roll<-thre){
            if(SEND_KEYBOARD)  Keyboard.press(KEY_LEFT_ARROW);
            Serial.println("LEFT");
            state = 'l';
          }
        }
        else if(state=='l'){
          if(roll>-thre){
            state='c';
            if(SEND_KEYBOARD)  Keyboard.releaseAll();
            Serial.println("CENTER");
          }
        }
        else if(state=='r'){
          if(roll<thre){
            Keyboard.releaseAll();
            Serial.println("CENTER");
            state='c';
          }
        }
        
        
//        Serial.print("pitch (y-right (east))    : ");
//        Serial.println(mpu.getPitch());
//        Serial.print("yaw   (z-down (down))     : ");
//        Serial.println(mpu.getYaw());

        prev_ms = millis();
    }
}


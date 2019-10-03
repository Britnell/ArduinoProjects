#include "MPU9250.h"

MPU9250 mpu;

#define BUZZPIN   10


void setup()
{
    pinMode(BUZZPIN,OUTPUT);
    digitalWrite(BUZZPIN,0);
    
    Serial.begin(115200);

    Wire.begin();
    delay(2000);
    mpu.setup();
  
    // 1040 2080 1035
    //
}


long fallTime = 0;
uint8_t falling = 0;

void loop()
{
    static uint32_t prev_ms = millis();
    
    if ((millis() - prev_ms) > 16)
    {
        mpu.update();
//        mpu.print();
        float accel []= { mpu.getAcc(0), mpu.getAcc(1),    mpu.getAcc(2) };
        float totalAcc = abs(accel[0])+abs(accel[1])+abs(accel[2]);

        
        
        if(totalAcc<1.0){
          //Serial.println(totalAcc);
          if(!falling){
            // BEGIN OF FALL
            fallTime= millis();
            falling = 1;
          }
          else{
            // STILL FALLING
            if(millis()-fallTime > 200){
              tone(BUZZPIN,1034,1000);
              Serial.print("F!\t");
              Serial.println(fallTime); 
            }
          }
          //
        }
        else{
          if(falling){
            falling = 0;
          }
        }
        
//        Serial.print(accel[0]);
//        Serial.print(" ,\t");
//        Serial.print(accel[1]);
//        Serial.print(" ,\t");
//        Serial.print(accel[2]);
//        Serial.print(" ,\t\t");
                
//        Serial.print(mpu.getRoll());
//        Serial.print(" ,\t");
//        Serial.print(mpu.getPitch());
//        Serial.print(" ,\t");
//        Serial.println(mpu.getYaw());
        
        prev_ms = millis();
    }
}


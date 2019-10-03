#include "MPU9250.h"

MPU9250 mpu;


void setup()
{
    
    Serial.begin(115200);

    Wire.begin();
    delay(2000);
    mpu.setup();
}



void loop()
{
    static uint32_t prev_ms = millis();
    
    if ((millis() - prev_ms) > 16)
    {
        mpu.update();
//        mpu.print();
        float accel []= { mpu.getAcc(0), mpu.getAcc(1),    mpu.getAcc(2) };
        float totalAcc = abs(accel[0])+abs(accel[1])+abs(accel[2]);

        Serial.println(totalAcc);

        
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


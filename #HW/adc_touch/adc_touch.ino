#include <ADCTouch.h>

int ref0, ref1;     //reference values to remove offset

void setup() 
{
    // No pins to setup, pins can still be used regularly, although it will affect readings

    Serial.begin(9600);
    delay(1000);
    
    ref0 = ADCTouch.read(A0, 500);    //create reference values to 
    Serial.println(" ADC initialised");
} 

void loop() 
{
    int value0 = ADCTouch.read(A0);   //no second parameter

    value0 -= ref0;       //remove offset

    Serial.print(value0 > 40);    //send (boolean) pressed or not pressed
    Serial.print("\t");           //use if(value > threshold) to get the state of a button
    Serial.print(value0);             //send actual reading
    Serial.print("\n");
	
    delay(100);
}

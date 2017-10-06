#include <ADCTouch.h>

int debug = 0;

int ref0, ref1, ref2;     //reference values to remove offset
char buff[50];
int state0, state1, state2;

void setup() 
{
    // No pins to setup, pins can still be used regularly, although it will affect readings

    if(debug)
      Serial.begin(9600);
    else
      Mouse.begin();
      
    ref0 = ADCTouch.read(A0, 500);    
    ref1 = ADCTouch.read(A1, 500);    
    ref2 = ADCTouch.read(A2, 500);

    ref0 += ADCTouch.read(A0, 500);    
    ref1 += ADCTouch.read(A1, 500);    
    ref2 += ADCTouch.read(A2, 500);

    ref0 /= 2;
    ref1 /= 2;
    ref2 /= 2;

    state0 = 0;
    state1 = 0;
    state2 = 0;
    
    if(debug)
      Serial.println("Ok now, Let's begin!");
    
    //
} 

void loop() 
{
    int T0 = ADCTouch.read(A0);   //no second parameter
    int T1 = ADCTouch.read(A1);   //   --> 100 samples
    int T2 = ADCTouch.read(A2);   //   --> 100 samples
    
    T0 -= ref0;       // remove offset
    T1 -= ref1;
    T2 -= ref2;

    if(debug)
    {  
      sprintf(buff,"%d,\t%d,\t%d", T0, T1, T2);
      Serial.print(buff);
  
      sprintf(buff,"\t\t%d,\t%d,\t%d", T0>80, T1>80, T2>80);
      Serial.println(buff);
    }
    else
    {
      // Mouse.click(button); 
      // Mouse.press(button) 
      // Mouse.release(button); 
      //    MOUSE_LEFT (default)
      //    MOUSE_RIGHT
      //    MOUSE_MIDDLE 

      // LEFT MOUSE CLICK
      if(T0 > 80)
      {
        if(!state0) // if state = low
        {
          state0 = 1;   // go high
          Mouse.press(MOUSE_LEFT);  // press
        }
        
      }   // eo if
      else
      {
        if(state0)  // if was high
        {
          Mouse.release(MOUSE_LEFT);    // release
          state0 = 0;   // go low
        }
      }   //eo else
        
      // Middle MOUSE click
      if(T2 > 80)
      {
        if(!state2)
        {
          Mouse.click(MOUSE_RIGHT);  // press
          state2 = 1;
        }
      }   // eo if
      else
      {
        if(state2)  // if was high
        {
          state2 = 0;   // go low
        }
      }   //eo else
      
      // RIGHT mouse click
      if(T1 > 80)
      {
        if(!state1) // if state = low
        {
          state1 = 1;   // go high
          Mouse.press(MOUSE_MIDDLE);  // press
        }
        
      }   // eo if
      else
      {
        if(state1)  // if was high
        {
          Mouse.release(MOUSE_MIDDLE);    // release
          state1 = 0;   // go low
        }
      }   //eo else
    }
    
    //Serial.println(value1);
    delay(50);
}


// ##

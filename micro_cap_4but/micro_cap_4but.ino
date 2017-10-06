/*
 *  Capacitive Touch test - 4 cap touch field
 *  
 *  reading, calculating & evaluation cap touch pads round robin in array.
 *  
 *      tested with 4 buttons : A0 to A3
 */

#include <ADCTouch.h>

#define PADS    4

//  A0,  A1,   A2,   A3
//  18,  19,   20,   21

int debug = 1;

int ref[PADS];     //reference values to remove offset
int state[PADS];
int T[PADS];

char buff[50];
int count = 1;

void setup() 
{
    // No pins to setup, pins can still be used regularly, although it will affect readings

    if(debug)
      Serial.begin(9600);
    else
      Mouse.begin();

    for(int i=0; i<PADS; i++)
    {
      ref[i] = ADCTouch.read(i+18, 500);
      state[i] = 0;
    }
    
    // ref2 /= 2;
    
    if(debug)
      Serial.println("Ok now, Let's begin!");
    
    //
} 


void loop() 
{
    // Read buttons
    count = 0;
    for(int p=0; p<PADS; p++)
    {
      // ---- 
      T[p] = ADCTouch.read(p+18);
      Serial.println(millis() );
    }
    
    // figure out presses
    for(int p=0; p<PADS; p++)
    {
      T[p] -= ref[p];
      if(T[p]>60){
        state[p] = 1;
        count++;
      }
    }

    
    if(debug)
    {  
      sprintf(buff,"%d,\t%d,\t%d,\t%d", T[0], T[1], T[2], T[3]);
      Serial.print(buff);
  
      sprintf(buff,"\t\t%d,\t%d,\t%d,\t%d", T[0]>80, T[1]>80, T[2]>80, T[3]>80);
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
      /*
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
    */
    }
    //Serial.println(value1);
    delay(50);
}


// ##

/*
 *  Capacitive Touch scroll bar
 *  
 *  reading, calculating & evaluation cap touch pads 
 *      tested with 4 buttons : A0 to A3
 *      Sending Mousewheel signal to scroll
 *      between and from keys
 *      step layout with 2 intermediate steps between two keys.
 *      thus 10 steps in total
 *      wrap around works so circular or repeating pads would possible for HW layout
 */

#include <ADCTouch.h>
#include <Mouse.h>
#include <Keyboard.h>

#define THRESHOLD   100
#define PADS        4

//  A0,  A1,   A2,   A3
//  18,  19,   20,   21

int debug = 0;

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

    for(int i=0; i<PADS; i++)
    {
      ref[i] = ADCTouch.read(i+18, 500);
      state[i] = 0;
    }
    
    // ref2 /= 2;
    
    if(debug)
      Serial.println("Ok now, Let's begin!");

    Mouse.begin();
    //Keyboard.begin();
    //
} 


void loop() 
{
    // Read buttons
    static uint8_t p_count = 0;
    static float i_finger = 0;
    static float l_finger;
    static float slider=0;
    
    float finger;
    
    count = 0;
    for(int p=0; p<PADS; p++)
    {
      // ---- 
      // 84 readings ~~ 20 ms each > 100 Hz
      T[p] = ADCTouch.read(p+18, 84) - ref[p];
      //T[p] -= ref[p];
      // figure out press
      if(T[p]>THRESHOLD){
        state[p] = 1;
        count++;
      }
      else
      {
        state[p] = 0;
      }
      //Serial.println(millis() );
    }
    
    if(debug)
    {  
      sprintf(buff,"%d,\t%d,\t%d,\t%d", T[0], T[1], T[2], T[3]);
      Serial.print(buff);
  
      //sprintf(buff,"\t\t%d,\t%d,\t%d,\t%d", T[0]>80, T[1]>80, T[2]>80, T[3]>80);
      //Serial.print(buff);
    }

    //uint8_t smudge = state[0]+state[1]+state[2]+state[3] ;
    if(count==0)
    {
      // if theres none
      finger = 0.0;
      l_finger = 0.0;
      i_finger = 0.0;
    }
    else if( count==1 )
    {
      // only one, find right one
      if(state[0])
        finger = 1.0;
      else if(state[1])
        finger = 2.0;
      else if(state[2])
        finger = 3.0;
      else if(state[3])
        finger = 4.0;
        
      if(!p_count)
      {
        // debouncer
        // last_count was 0, this is positive edge
        // this is slider starting position
        i_finger = finger;
        slider = 0;
      }
        
    }
    else if( count==2)
    {
        // Between 1 & 2
        if( state[0] & state[1])
        {
          //finger = 1.5;
          if( T[1] > T[0])
            finger = 1.333;
          else
            finger = 1.666;
          // --
        }
        else if( state[1] & state[2])
        {
          //finger = 2.5;
          if( T[1] > T[2])
            finger = 2.333;
          else
            finger = 2.666;
        }
        if( state[2] & state[3])
        {
          //finger = 3.5;
          if( T[2] > T[3] )
            finger = 3.333;
          else
            finger = 3.666;
        }
        if( state[3] & state[0])
        {
          //finger = 4.5;
          if( T[3] > T[0] )
            finger = 4.333;
          else
            finger = 4.666;
        }
          
        if(!p_count)
        {
          // debouncer
          // last_count was 0, this is positive edge
          // this is slider starting position
          i_finger = finger;
          slider = 0;
        }
    }

    // diff
    float  diff = finger -l_finger;
    
    if( ( diff > 0) && (diff <= 1.0) )     // || (finger -l_finger>=3) )
    {
      //FUNC_F(7);
      Mouse.move(0,0,1);
    }

    if( diff >=3.0)
      FUNC_F(7);
      
    if( ( diff < 0 ) && ( diff >= -1.0) )     // || (finger -l_finger>=3)
    {
      //FUNC_F(7);
      Mouse.move(0,0,-1);
    }
      
    /*
    if( diff <= -3.0)
    {
      //FUNC_F(8);
      Mouse.move(0,0,1);
    }
    */
    
    slider = finger - i_finger;

    if(debug)
    {
      //Serial.print("\tN:");
      //Serial.print(count);
      Serial.print("\tF- ");    
      Serial.print(finger);
      Serial.print(" \tin:");
      Serial.print(i_finger);
      Serial.print(",\tscroll: ");
      Serial.println(slider);
    }
    
    
    l_finger = finger;
    p_count = count;
    
    //delay(50);
}


void FUNC_F(uint8_t f_key){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.write(f_key+193);
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_CTRL);
}


// # 

// ##

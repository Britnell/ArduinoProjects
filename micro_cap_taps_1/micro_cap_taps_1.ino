/*
 *    Cap Touch Tap gestures v1.1
 *    
 *    Single touch pad connected to pin A0
 *    Recognizes differently timed double taps.
 *      measures timing between taps to measure
 *      - length of touch of first tap
 *      - length of gap between taps
 *      - length of touch of second tap
 *      
 *      set debug = 1 to turn on / off Serial debug output
 * 
 */

#define   tap_thresh      135   // threshold for long tap
#define   gap_thresh      205   // threshold for long wait

#define L_ARRAY    3
#define SET(num, bits)      num |= (1<<bits) 
#define CLEAR(num, bits)    num &= ~(1<<bits)

#include <ADCTouch.h>
#include <Keyboard.h>

int debug = 0;

int ref0;     //reference values to remove offset
char buff[50];
int state0;

int a;
unsigned long down[L_ARRAY];
unsigned long up[L_ARRAY];
unsigned int * Taps;

void setup() 
{

    if(debug)
    {
      Serial.begin(38400);
      Serial.println("Kallibratin");      
    }

    Keyboard.begin();
    
    // reading 100 Values takes 23/24 ms
    // ~~ 0.235 ms per reading
    // for 10 Hz : 425 readings 
    // for 50 Hz : 85 readings
    // for 100 readings : 42 Hz - SAWA
    
    ref0 = ADCTouch.read(A0, 500);    
    ref0 += ADCTouch.read(A0, 500);    
    ref0 /= 2;
    state0 = 0;
    
    rst();
    //a=0;
    
    if(debug)
      Serial.println("Ok now, Let's begin!");
    
    //
} 

void loop() 
{
    unsigned long diff;
    int bpm;
    uint8_t tap;
    
    int T0 = ADCTouch.read(A0);   
    //no second parameter -> 100 Samples
    // 100 samples = 42 Hz
    
    T0 -= ref0;       // remove offset

    // if push detected
    if(T0 > 80)
    {
      // A. push-edge, down event
      if(!state0) 
      {
        // debouncer
        down[a] = millis();
        state0 = 1;
            
      } // Eo if(!state0)
      else
      {
        // B. Holding down
        // - do nothing
        //
      } // Eo else
      
    }   // Eo (T0>80)
    else  
    {   // if no push

      if(state0)
      {
        // C. release-edge, up event
        //
        up[a] = millis();
        state0 = 0;

        // best do calculations, right here, at release
        if(++a > 1)
        {
          // second tap, lets calc
          Taps = double_taps(a, down, up);  
          a = 0;
          
          if(debug)
          {
            log_ui("T1:\t\t", Taps[0]);
            log_ui("Gap:\t\t", Taps[1]);
            log_ui("T2:\t\t", Taps[2]);
          }

          tap = clicks( Taps);

          if(debug)
          {
            log_i("tap: ",tap);
            log_b("bin: ",tap);
          }

          switch(tap){
            // Signals here
            // s | s
            // 000
            case 0:
              if(debug)Serial.println("impacient double click");
              FUNC_F(9);
              break;
            // s -- s
            // 0 1 0
            case 2:
              if(debug)Serial.println("relaxed double click");
              FUNC_F(10);
              break;
            // s | L
            // 0 0 1
            case 4:
              if(debug)Serial.println("tap+HOLD---");
              FUNC_F(11);
              break;
            // L | s
            // 1 0 0
            case 1:
              if(debug)Serial.println("long + tap");
              FUNC_F(12);
              break;
          }
          //
        }
        /*
        //
        else if(a==1)
        {
            // catch long tap right when it happens
            // falling edge, a = 1, we just counted up
            if( up[0]-down[0] > tap_thresh )
            {
                if(debug)Serial.println("long single");
                FUNC_F(12);
                // reset tap counter
                a = 0;
            }
            //
        }
        //
        */
        // a++;
        //dev_action( a, puls_length, 0);
      }
      else
      {
        // button is off and was off
        // - wait
        if(a>0)
        {
          if( millis() - up[a-1] > 1000 ) // time since release
          {
            a = 0;
            if(debug)Serial.println('a');
          }
        }
        // clear loose single taps
        
      }
      // Eo else (T0>80)
    }
    // no delay(50);
    // Eo loop()  
} 


/*  Kets
 *   KEY_F1   0xC2  194 = 1 + 193
 *  KEY_F12   0xCD  205 = 12 + 193
 * 
 */
void FUNC_F(uint8_t f_key){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.write(f_key+193);
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_CTRL);
}

uint8_t clicks( unsigned int* tapses){
  
  uint8_t nary = 0;
  
  if( tapses[0] < tap_thresh)
    CLEAR(nary,0);
  else
    SET(nary,0);

  if( tapses[1] < gap_thresh)
    CLEAR(nary, 1);
  else
    SET(nary, 1);

  if( tapses[2] < tap_thresh)
    CLEAR(nary, 2);
  else
    SET(nary, 2);
  
  //for(int x=0;x<3;x++)
    //Serial.print(nary);
    
  //for(int i=1;i<256;i=i<<1)
  //  log_b("bin\t\t", i);
  
  return  nary;
}

unsigned int* double_taps(int puls, unsigned long* down, unsigned long* up)
{
  // puls = 1 for 1 puls
  // 2 for 2 pulses
  static unsigned int pulses[3];
  //int gap_length[3];

  pulses[0] = int(up[0] - down[0]);    // down1 to up1
  pulses[1] = int(down[1] -up[0]);   // up1 to down2
  pulses[2] = int(up[1] - down[1]);    // down2 to up2
  
  if(debug)
  {
    /*
    // See original times here for debugging
    log_ul("down1\t", down[0]);
    log_ul("up1\t", up[0]);
    log_ul("down2\t", down[1]);
    log_ul("up2\t", up[1]);
    Serial.println();
    */
        
    log_ui("press1\t", pulses[0] );
    log_ui("gap1\t", pulses[1] );
    log_ui("down2\t", pulses[2] );
    
  }
  
  return pulses;
  
  // Eo dev_action()
}   

void rst(){
  for(int i=0; i<L_ARRAY; i++)
  {
    down[i] = 0;
    up[i] = 0;
  }
  a = 0;
  
  //puls_length = 0;
  //gap_length = 0;
}

void log_i( String txt, int x){
  Serial.print(txt);
  Serial.println(x);
}

void log_ui( String txt, unsigned int x){
  Serial.print(txt);
  Serial.println(x);
}

void log_l( String txt, long x){
  Serial.print(txt);
  Serial.println(x);
}

void log_ul( String txt, unsigned long x){
  Serial.print(txt);
  Serial.println(x);
}

void log_f( String txt, float x){
  Serial.print(txt);
  Serial.println(x);
}

void log_c( String txt, char x){
  Serial.print(txt);
  Serial.println(x);
}

void log_b(String txt, uint8_t bin){
  
  Serial.print(txt);
  for(int x=0; x<8; x++)
  {
    if( ( bin & (0x01 << x) ) )
      Serial.print(1);
    else
      Serial.print(0);
      
    // Eo for
  }
  Serial.println();
}

//  ##

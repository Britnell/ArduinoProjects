/*
 *  Takes Serial Pixel data and draws onto LED matrix pad
 *    treats matrix like a single-led-strip
 *    draws every collumn the same colour, 
 *    so uses light curve design sketch 
 *  
 */

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            2
#define NUMPIXELS      256
#define MATRIX_Y       8
#define MATRIX_X       32
#define SAVE_TO_EEPROM  0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(MATRIX_X*MATRIX_Y, PIN, NEO_RGB + NEO_KHZ800);

int delayval = 50; 
int tail;
int last_pix;
int this_pix;

char buff[50];

// ** Memory

#define EEPROM_ADDR     6
#define BITE            3
int EEPROM_LEN;

void load_EEPROM( ){
  // = NUMPIXELS * (R+G+B __W )
  EEPROM_LEN = NUMPIXELS*BITE;
  int strip_i=0;
  int r,g,b,w;
  for(int e=0; e<EEPROM_LEN; e++){
    byte val = EEPROM.read(EEPROM_ADDR+e);
    if(e%BITE==0)           r=val;
    else if(e%BITE==1)      g=val;
    else if(e%BITE==2)   
    {
      b=val;
      
      if(strip_i<NUMPIXELS) // only write if within pixels
      {   
        //sprintf(buff,"%d(%d,%d,%d)\n",strip_i, r, g, b); Serial.print(buff);
        pixels.setPixelColor(strip_i, r, g, b );
        strip_i++;        
      }
      
    }
    // Eo for 
  }
  pixels.show();
  //--
}



void setup() {
  // ---
  Serial.begin(115200);
  pixels.begin(); 

  if(SAVE_TO_EEPROM)    load_EEPROM();
  
  tail = 1;
  last_pix = 0-tail;
  this_pix = 0;
}

int len = 0;
int col_i = 0;
int color = 0;
int red = 0;
int green = 0;
int blue = 0;
uint8_t reframe=0;

void loop() 
{
  if (Serial.available())
  {
    byte s = Serial.read();
    if (s == 10 )
    {
        if(!reframe) {
          // - Show strip
          pixels.show();
        }
        col_i = 0;      len = 0;
        if(reframe) {
          //Serial.println("#FrE");
          reframe=0;
        }
        // if EOL
    }
    else
    {
      // - its an RGB value
      //    fill in accordingly

      // only set Pixel if within length
      if ( len <= NUMPIXELS )
      {
        // add to color
        if (col_i == 0) {
          s = ( s-128)*2;
          green = s;
          col_i++;
        }
        else if (col_i == 1) {
          s = ( s-128)*2;
          red = s;
          col_i++;
        }
        else if (col_i == 2) {
          s = ( s-128)*2;
          blue = s;
          col_i++;
        }
        else if(col_i==3)
        {
          if(s==',')
          {
            if(!reframe)
            {
              // - Send the Pixel
              pixels.setPixelColor(len , red, green, blue );    
              // - and save
              if(SAVE_TO_EEPROM)
              if(EEPROM_ADDR+len*BITE+2 < EEPROM.length() ){
                EEPROM.update(EEPROM_ADDR+len*BITE+0, red);
                EEPROM.update(EEPROM_ADDR+len*BITE+1, green);
                EEPROM.update(EEPROM_ADDR+len*BITE+2, blue);
              }
              //strip.setPixelColor(len, color);
              // - count num pixels
              len++;
              col_i = 0;
          }
        }
        else
        {
          if(!reframe){
            reframe = 1;
          }
        }

        // - i=3
      }

      }
      // - if not EOL
    }

    // - if serial available
  }

  // ** Eo loop
}

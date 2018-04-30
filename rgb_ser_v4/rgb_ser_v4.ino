/*
    Fading two pixels with two instances seems to have some problems
  
*/


#include <Adafruit_NeoPixel.h>
  
#include "Pixel.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12

#define LOGO_PIXELS     7


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
// ( pixels, pin, ... )

Adafruit_NeoPixel NeoLeft = Adafruit_NeoPixel( 3, 3, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel NeoRight = Adafruit_NeoPixel( 5, 4, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel NeoLogo = Adafruit_NeoPixel( LOGO_PIXELS, 5, NEO_RGBW + NEO_KHZ800);

//char buff[40];

#define OFF     'o'
#define WHITE   'w'
#define DIMM    'd'
#define BLUE    'b'



// *  Define Neopixel objects to Neoipixel ID number in pixel-chain
// *
Pixel L_Up(0);
Pixel L_Down(2);
Pixel L_Pilot(1);
//
Pixel R_Up(1);
Pixel R_Down(3);
Pixel R_Left(0);
Pixel R_Right(4);
Pixel R_Center(2);
// 
Pixel logo(0);


//Pixel P3(2);

int delayval = 1000; // delay for half a second



void setup() {
  Serial.begin(115200);

  delay(200);
  
  NeoLeft.begin(); // This initializes the NeoLeft library.
  NeoRight.begin(); // This initializes the NeoLeft library.
  NeoLogo.begin(); // This initializes the NeoLeft library.
  
  //Serial.println("off");
  
  
  NeoLeft.show();
  NeoRight.show();
  NeoLogo.show();
  
  delay(200);
  
  //Serial.println(" lighting arduino Ready" );

  //
}


int intvl = 800;

int dela = 3;
uint8_t fadespeed = 127;

void loop() {
  char id, col, cmd;
  
  if( Serial.available() >= 2 ) {
    // we need TWO chars9
    
    id = Serial.read(); // char
    id -= 48;   // convert to INT
    
    if( 0<= id && id <= 7 )
      col = Serial.read();
    
    switch(id){

      case 0: 
        // All
        turn_led_col( &L_Up, col);
        draw_pixel( &NeoLeft, &L_Up );  
        turn_led_col( &L_Pilot, col);
        draw_pixel( &NeoLeft, &L_Pilot ); 
        turn_led_col( &L_Down, col);
        draw_pixel( &NeoLeft, &L_Down );   
        
        turn_led_col( &R_Up, col);
        turn_led_col( &R_Down, col);
        draw_pixel( &NeoRight, &R_Up);  
        draw_pixel( &NeoRight, &R_Down);  
        
        turn_led_col( &R_Center, col);
        draw_pixel( &NeoRight, &R_Center );  
        
        turn_led_col( &R_Left, col);
        turn_led_col( &R_Right, col);
        draw_pixel( &NeoRight, &R_Left );  
        draw_pixel( &NeoRight, &R_Right );  
        
        turn_led_col( &logo, col);
        draw_pixel( &NeoLogo, &logo );  
        break;
      case 1: 
        // l-up
        turn_led_col( &L_Up, col);
        draw_pixel( &NeoLeft, &L_Up );  
        break;
      case 2: 
        // l-pilot
        turn_led_col( &L_Pilot, col);
        draw_pixel( &NeoLeft, &L_Pilot );  
        break;
      case 3: 
        // l-down
        turn_led_col( &L_Down, col);
        draw_pixel( &NeoLeft, &L_Down );  
        break;
      case 4: 
        // r-up+down
        turn_led_col( &R_Up, col);
        turn_led_col( &R_Down, col);
        draw_pixel( &NeoRight, &R_Up);  
        draw_pixel( &NeoRight, &R_Down);  
        break;
      case 5: 
        // r-left+right
        turn_led_col( &R_Left, col);
        turn_led_col( &R_Right, col);
        draw_pixel( &NeoRight, &R_Left );  
        draw_pixel( &NeoRight, &R_Right );  
        break;
      case 6: 
        // r-center
        turn_led_col( &R_Center, col);
        draw_pixel( &NeoRight, &R_Center );  
        break;
      case 7: 
        // logo
        turn_led_col( &logo, col);
        draw_pixel( &NeoLogo, &logo );  
        break;
      default:
        Serial.print("Wrong LED ID:");
        Serial.println(char(id+48));
        break;
      // Eo Switch
    }
      
   
    // Eo serial available
  }
  
  // Fade Pixels
  // Left
  if(L_Up.fade()) draw_pixel( &NeoLeft, &L_Up);
  if(L_Down.fade()) draw_pixel( &NeoLeft, &L_Down);
  if(L_Pilot.fade()) draw_pixel( &NeoLeft, &L_Pilot);
  //
  if(R_Up.fade()) draw_pixel( &NeoRight, &R_Up);
  if(R_Down.fade()) draw_pixel( &NeoRight, &R_Down);
  if(R_Left.fade()) draw_pixel( &NeoRight, &R_Left);
  if(R_Right.fade()) draw_pixel( &NeoRight, &R_Right);
  if(R_Center.fade()) draw_pixel( &NeoRight, &R_Center);
  //
  if(logo.fade()) draw_all( &NeoLogo, &logo);
  
  
  // Write new values to neoPixel
  NeoLeft.show();
  NeoRight.show();
  NeoLogo.show();

  
  delay(dela); // Delay for a period of time (in milliseconds).

  // Eo -- Loop
}



/*
 * 
 */
void turn_led_col( Pixel * PIX, char col){
  if( col == 'o') { 
    // Off
    PIX->fadeTo( 0,0,0, 0, fadespeed );
  }
  else if( col == 'w') {
    // White
    PIX->fadeTo( 10, 10, 100, 255, fadespeed );
  }
  else if( col == 'd' ) {
    // dimm white
    PIX->fadeTo( 30, 10, 0, 100, fadespeed );
  }
  else if( col == 'b' ) {
    // blue
    PIX->fadeTo( 0, 120, 255 , 0, fadespeed );
  }
  else {
    Serial.print("UnknownCOlor ");
    Serial.println(col);
  }
}

void draw_pixel( Adafruit_NeoPixel * PixStrip, Pixel * PIX ) {
  PixStrip->setPixelColor(PIX->ID(), PixStrip->Color(  PIX->G(), PIX->R(), PIX->B(), PIX->W() ) );
}


void draw_all( Adafruit_NeoPixel * PixStrip, Pixel * PIX ) {
  for(uint8_t p = 0; p<LOGO_PIXELS; p++ ){
    PixStrip->setPixelColor(p, PixStrip->Color(  PIX->G(), PIX->R(), PIX->B(), PIX->W() ) );  
  }
  
}


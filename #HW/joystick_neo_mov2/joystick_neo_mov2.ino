// 2. version. I want a L/R moving thing



#include <math.h>

#include <Adafruit_NeoPixel.h>
#define PIN            4
#define NUMPIXELS      150

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


 int ledPin = 13;
 int joyPin1 = 0;                 // slider variable connecetd to analog pin 0
 int joyPin2 = 2;                 // slider variable connecetd to analog pin 1
 int value1 = 0;                  // variable to read the value from the analog pin 0
 int value2 = 0;                  // variable to read the value from the analog pin 1

int center1 = 0;
int center2 = 0;


void all_pixels( int r, int g, int b){
  for( int i=0; i<NUMPIXELS; i++)
    pixels.setPixelColor( i, pixels.Color(r, g, b) );
}

void x_pixels( int beg, int num, int r, int g, int b){
  for( int i=beg; i<beg+num; i++)
    pixels.setPixelColor( i, pixels.Color(r, g, b) );
}



 void setup() {
  pinMode(ledPin, OUTPUT);              // initializes digital pins 0 to 7 as outputs
  Serial.begin(115200);

  pixels.begin();
  
  center1 = (analogRead(joyPin1));
  center2 = (analogRead(joyPin2));
 }


int x = 0;
int y = 0;


int lightpos = 20;
int lightint = 8;

int strip_width = 20;

long width_int = 0;

void joystick(){
  // 16 for 0 to +-32
  // 32 for 0 to +- 16
  // 80 for 1 to 6
  int scaledown = 80;
    
  value1 = (analogRead(joyPin1)) -center1;
  value1 /= scaledown;
  
  // this small pause is needed between reading
  // analog pins, otherwise we get the same value twice
  //delay(100);            
  // reads the value of the variable resistor
  value2 = (analogRead(joyPin2)) -center2;
  value2 /= scaledown;
  
  if( x != value2 || y!= value1)
  {
    x = value2;
    y = value1;
    
    if( 1)
    {
        int max_int = 18;
//        double theta = atan2(y, x);
//        int led = int( ((theta+PI) / TWO_PI)  * NUMPIXELS ) -10;
        
        Serial.print("J\t");    Serial.print(x);
        Serial.print("\t");     Serial.print(y);
        
//        Serial.print("\t @ ");     Serial.print(theta);
//        Serial.print("\tL ");     Serial.print(led);

        if(x<0){
          // negative
          if(x<-5)
            lightint = -1;
          else if(x<-3)
            lightint = -4;
          else if(x<0)
            lightint = -12;         
        }
        else if(x==0)
          lightint = 0;
        else if(x<3)
          lightint = 12;
        else if(x<5)
          lightint = 4;
        else
          lightint = 1;
        
        
//        if( y < -2 ) {
//          // Y - Up
//          if(millis() -width_int> 600 ) {
//              strip_width++;
//              width_int = millis();
//              Serial.print("\tU");
////              all_pixels(0,0,0);
////              x_pixels( lightpos, strip_width,  0,5,80);
////              pixels.show();
//          }
//          //-
//        }
//        else if( y > 2 ) {
//          // Y - down
//          if( millis() -width_int > 600 ) {
//              strip_width--;
//              width_int = millis();
//              Serial.print("\tD");
////              all_pixels(0,0,0);
////              x_pixels( lightpos, strip_width,  0,5,80);
////              pixels.show();
//          }
//        }
        
        Serial.print("\tsped : ");    Serial.println(lightint);
        
        //
    }
  }
  // Eo joystick
}




 void loop() {
  // reads the value of the variable resistor

  joystick();

  // joystick sets speed
  
  // speed adjusts postition
  if(interval != 0)
  if( interval(abs(lightint)) ) {
    //
    if(lightint>0) {
      lightpos++;
      if(lightpos >= NUMPIXELS )  lightpos= -strip_width;
    }
    else if(lightint<0) {
      lightpos--;
      if( lightpos < -strip_width )  lightpos = NUMPIXELS;
    }
    
    all_pixels(0,0,0);
    x_pixels( lightpos, strip_width,  0,5,80);
    pixels.show();
  }
  
  // draw
  
  count();
  
  delay(10);
  // Eo loop
 }


int intvl = 0;
uint8_t interval(int per){
  if(intvl%per==0)
    return 1;
  else
    return 0;
}

void count(){
  intvl++;
}


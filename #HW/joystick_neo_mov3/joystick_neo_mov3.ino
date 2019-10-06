// 2. version. I want a L/R moving thing



#include <math.h>

#include <Adafruit_NeoPixel.h>
#define PIN            4
#define NUMPIXELS      150

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


 int ledPin = 13;
 int joyPin1 = 0;                 // slider variable connecetd to analog pin 0
 int joyPin2 = 1;                 // slider variable connecetd to analog pin 1
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


void joystick(){
  value1 = (analogRead(joyPin1)) -center1;
  value1 /= 80;
  
  // this small pause is needed between reading
  // analog pins, otherwise we get the same value twice
  //delay(100);            
  // reads the value of the variable resistor
  value2 = (analogRead(joyPin2)) -center2;
  value2 /= 80;
  
  if( x != value2 || y!= value1)
  {
    x = value2;
    y = value1;

    if( 1)
    {
        double theta = atan2(y, x);
        int led = int( ((theta+PI) / TWO_PI)  * NUMPIXELS ) -10;
        int sped;
        
        Serial.print("J\t");    Serial.print(x/1);
        Serial.print("\t");     Serial.print(y/1);
        Serial.print("\t @ ");     Serial.print(theta);
        Serial.print("\tL ");     Serial.print(led);

        if(x>0){
          lightint = 12 -x;
        }
        if(x<0){
          lightint = -12 -x;
        }
        
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
  if( interval(abs(lightint)) ){
    //
    if(lightint>0)
      lightpos++;
    else if(lightint<0)
      lightpos--;
    
    if(lightpos >= NUMPIXELS )
      lightpos= -19;
      
    all_pixels(0,0,0);
    x_pixels( lightpos, 20,  0,5,80);
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


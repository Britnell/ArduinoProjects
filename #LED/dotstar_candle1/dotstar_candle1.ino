

#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET


#define NUMPIXELS 64 // 
#define DATAPIN    2
#define CLOCKPIN   3
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

#define PT_EN     0
#define pt(x)    if(PT_EN) Serial.print(x)
#define pl(x)    if(PT_EN) Serial.println(x)

void setup() {
  if(PT_EN) Serial.begin(115200);
  if(PT_EN) delay(1000);
  
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  
  randomSeed(analogRead(A0));
}

long t = 0;

int      head  = 0, tail = -2; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)
int led = 0;

double candleSlope = 0.2;
float baseX = 4.5;

float baseY = 2.2;
float flameWaist = 4.4;
float flameBase = 1.2;
float flameTip = 0.6;

// * Vary flame base
  int baseRanFreq = 30;
  int baseRanStep = 30;  // mag of random var
//  int baseXret = 2000;
  
  
// * Vary flame slope
  int slopeRanFreq = 60;
  float slopeLim = 2.8;
  float slopeStep = 0.015;
  float slopeReturn = 0.3;
  

void loop() {

  
  
  led = 0;
  for( int r=0; r<8; r++)
  {
    for(int c=0; c<8; c++)
    {
      float bright = 0;
      
      if(r<=baseY)
      {
        // * bottom rows 
        if( abs(baseX-c)<flameBase ){ // if inside flame base
          bright = map_f(r,0.0,baseY, 1.0,255.0);
        }
        else{
          bright = 0.0;
        }
        draw_base(bright);
      }
      else
      {
        // flame up

        // pos of flame on that row
        float rowFlameX = baseX + (r-baseY) * candleSlope / 10;
        
        // bright by X dist from there
        float dist = abs(rowFlameX-c);
        float width = flameBase;
        
        if(r>flameWaist)
          width = map_f(r, flameWaist,7, flameBase,flameTip );
        
        if( dist <width ){
          bright = map_f(dist,0,width,255,0);
        }
        else{
          bright = 0;
        }   
        
        draw_brightness(bright);
      }
      
      
    }
  }

  // * Random Flame Slope
  
  //candleSlope -= 0.1;
  //if(candleSlope<-slopeLim)    candleSlope = slopeLim;
    
  
  // * Flame random slope
  if(random(1000)<slopeRanFreq) {
    candleSlope = float(random(-30,30)) / 10;
  }
  // * flame slope return
  if(candleSlope<-slopeReturn)
      candleSlope += slopeStep;
  else if(candleSlope>slopeReturn)
      candleSlope -= slopeStep;

  // * random base
  if(random(1000)<baseRanFreq)
  {
    pt("R:\t");
    baseX += float(random(-baseRanStep,baseRanStep)) /100;
    if(baseX > 5.4)  baseX = 5.4;
    if(baseX < 1.6)  baseX = 1.6;

  }
  // * base return
//  if(baseX > 3.52)  baseX -= 1.0 / baseXret;
//  if(baseX < 3.48)  baseX += 1.0 / baseXret;
  
  
  if(millis()-t>100)
  {
    pt(candleSlope); pt("\t"); pt(baseX);       pl();
    t = millis();
  }
//  pt(candleSlope); pt("\t"); pt(baseX);       pl();
  
  strip.show();                     // Refresh strip
//  delay(10);

  // Eo loop
}


void draw_base(float bright)
{
  uint8_t r,g,b;
  float dim = 2.0;
  r = int(bright/dim);
  g = int(bright/dim*2/3);
  
  //b = int((bright+16)/dim/5);
  if(bright>0)     b = int((bright)/20)+8;
  else             b = 0;
  

  uint32_t col = g;
  col = (col<<8);
  col += r;
  col = (col<<8);
  col += b;
  
  strip.setPixelColor(led, col);
  led++;
}

void draw_brightness(float bright){
  
  uint8_t r,g,b;
  float dim = 2.0;
  r = int(bright/dim);
  g = int(bright/dim*2/3);
  b = 0;

  uint32_t col = g;
  col = (col<<8);
  col += r;
  col = (col<<8);
  col += b;
  
  strip.setPixelColor(led, col);
  led++;
}


float map_f( float val, float Min, float Max, float OMin, float OMax)
{
  float ratio = (val-Min) / ( Max-Min);
  float res = OMin + ratio * (OMax-OMin);
  return res;
}


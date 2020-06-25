

#include <Adafruit_DotStar.h>
#include <SPI.h>        

#define PT_EN    0
#define pt(x)    if(PT_EN) Serial.print(x)
#define pl(x)    if(PT_EN) Serial.println(x)

#define NUMPIXELS 64 // 
#define DATAPIN    2
#define CLOCKPIN   3
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

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

float dim = 1.6;

double candleSlope = 0.2;
float baseX = 3.1;
float baseMin = 1.8;
float baseMax = 6.2;

float baseY = 2.2;      // static base drawn up to here
float flameWaist = 4.9; // height from which is narrows
float flameBase = 1.6;  // width of base, below waist
float flameTip = 0.8;   // width at tip

// * Vary flame base
  int baseRanFreq = 60;
  int baseRanStep = 20;  // mag of random var
//  int baseXret = 2000;
  
  
// * Vary flame slope
  int slopeRanFreq = 30;  // prob /1000 of adding random slope
  float slopeLim = 2.8;
  float slopeStep = 0.029;  // 0.015
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
          //bright = map_f(r,0.0,baseY, 1.0,255.0);
          bright = map_f( abs(baseX-c), 0,flameBase, 80.0*r, 1.0 );
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
          bright = map_f(dist,0,width,255,30);
        }
        else{
          bright = 0;
        }   
        
        draw_brightness(bright,r);
      }
      
      
    }
  }

  // * Random Flame Slope
  
  //candleSlope -= 0.1;
  //if(candleSlope<-slopeLim)    candleSlope = slopeLim;
    
  
  // * Flame random slope
  if(random(1000)<slopeRanFreq) {
    candleSlope = float(random(-35,35)) / 10;
    pt(" slope ran : ");  pl(candleSlope);
  }
  
  // * flame slope return
  if(candleSlope<-slopeReturn)
      candleSlope += slopeStep;
  else if(candleSlope>slopeReturn)
      candleSlope -= slopeStep;

  // * random base
  if(random(1000)<baseRanFreq)
  {
    baseX += float(random(-baseRanStep,baseRanStep)) /100;
    if(baseX > baseMax)  baseX = baseMax;
    if(baseX < baseMin)  baseX = baseMin;
    pt("Ran base :\t"); pl(baseX);
  }
  
  
  if(millis()-t>100)  {
    pt("pt: ");  pt(candleSlope); pt("\t"); pt(baseX);       pl();
    t = millis();
  }
  
  strip.show();                     // Refresh strip
//  delay(5);

  // Eo loop
}



void draw_base(float bright)
{
  uint8_t r,g,b;
//  float dim = 2.0;
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

void draw_brightness(float bright, int coll){
  
  uint8_t r,g,b;
  
  r = int(bright/dim);
  g = int(bright/dim*2/3);
    
  if(coll>flameWaist){
    // quite yellowy
    b = map_f(bright,0,255,0,(coll-flameWaist)*40) / dim;
//    if(bright>230) b = map_f(bright,220,255,0,30);
  }
  else{
    // slightly more red-ish
    b = 0;
  }
  

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

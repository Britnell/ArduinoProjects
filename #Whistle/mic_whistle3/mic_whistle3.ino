/*
 *      Runs fix_fft on mic data
 *    finds highest and second-highest peak in fft band
 *    uses ratio to judge noise / clarity
 *    interpolates to get analog whistle freq
 *    tracks consecutive whistles (each whistle only one note
 *    
 *      min band increased to 9/10 as some interference seems 
 *    to cause peaks at 6 after a whistle , prob from serial?
 *    
 */

#include <fix_fft.h>
#define ADC   A0
#define pt(x)   Serial.print(x)
#define pl(x)   Serial.println(x)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  #ifdef USE_LEDS
  pixels.begin(); 
  #endif
  
}


uint8_t av = 0;
const int N_AVRG = 100;
int avrg[N_AVRG];

// FFT
#define FFT_N   128
byte data[FFT_N], im[FFT_N], data_avgs[8];


byte bandMin = 9;       byte bandMax = 28;  

long lastWhistle = 0;
int whistleStop = 400;  // wait for wh to end
int whistleJoin = 900;  // time until which wh can be continued
float whistleJoinThr = 0.9;  // if below thresh
float whistleAv = 0;
int whistleL = 0;
uint8_t whistleFlag = 0;

float melody[10];
uint8_t Lmel = 0;


void loop() {
  int i, j;
  
  // * Read

  // ADjustable gain : 
  // average ~511
//  int val = analogRead(ADC);
  //  if(abs(val-512)> 100)    pl(val);

  // * FFT
  
  for(i = 0; i < FFT_N; i++) {
    data[i] = analogRead(ADC);
    im[i] = 0;
  }
  fix_fft(data,im,7,0); // ; 7= m for order ; 0 for forward FFT

  // * Amplitude spectrum
  
  for (i = bandMin; i <= bandMax; i++){
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); 
  }
  
  // * find peak & vice band
  
  int Max = -1;
  int MaxB = -1;
  int Vice = -2;
  int ViceB = -1;
  for( int s=bandMin; s<=bandMax; s++){
    if(Max==-1){
      // first  -is peak
      Max = data[s];
      MaxB = s;      
    }
    else if(Vice==-2){
      // second - check for peak or vice
      if(data[s]>Max){
        // new peak & vice
        Vice = Max;
        ViceB = MaxB;
        Max = data[s];
        MaxB = s;
      }
      else {
        // is vice
        Vice = data[s];
        ViceB = s;
      }
    }
    else {
      // rest - check peak
      if(data[s]>Max){
        // new peak & vice
        Vice = Max;
        ViceB = MaxB;
        Max = data[s];
        MaxB = s;
      }
      else if(data[s]>Vice){
        // new vice
        Vice = data[s];
        ViceB = s;
      }
    }
    // for - peak
  }

  // * peak ratio
  if(Max>5)
  {
    
    float ratio = 1.0 * Max / Vice;
    uint8_t WHIS = 0;
    
    // for high ratio or if neighbours
    if(ratio > 2.0 ){
      WHIS = 1;
    }
    else if( abs(MaxB-ViceB) < 2 && ratio > 1.0 ){
      WHIS = 1;
    }

    if(WHIS){
    
        // * print spectrum
    //      for(i=bandMin; i<=bandMax; i++) 
    //      {    pt(data[i]); pt("\t");      }
    //      pt("\n");
      
        // * print peaks
    //      pt(" # Ratio \t"); pt(ratio);
    //      pt("\tPeak\t");  pt(MaxB);  pt("  :  ");  pt(Max); 
    //      pt("\t   Vice\t");  pt(ViceB);  pt("  :  ");  pt(Vice);
    //      pl();
    
      // * interpolate with neighbours
      int total = 0;    // get total
      total += data[MaxB];
      total += data[MaxB-1];
      total += data[MaxB+1];
      
      float freq = 0;   // get distribution / total
      freq += 1.0*(MaxB-1) *data[MaxB-1] /total;
      freq += 1.0*(MaxB) *data[MaxB] /total;
      freq += 1.0*(MaxB+1) *data[MaxB+1] /total;    

//      pt(" # Whistle : ");    pt(freq);      pl();

      // * Whistle Track
      if(!whistleFlag){
        // begin whistle
        if(millis()-lastWhistle<whistleJoin  &&  abs(freq-whistleAv)<whistleJoinThr ){
          // continue last whistle
          pt(" c ");
        }
        else {
          pt("\nW:   ");   pt(millis()-lastWhistle); pt(" , "); pt(abs(freq-whistleAv)); pt("\t");
          whistleAv = freq;
          whistleL = 1;
        }
        whistleFlag = 1;
      }
      else{
        // continue whistle
        whistleAv += freq;
        whistleL++;
      }
      lastWhistle = millis();
      // * 
    }
    // if Max lim
  }
  
  // * NOT a whistle
  if(whistleFlag)
  if(millis()-lastWhistle > whistleStop){
    // end whistle
    whistleAv /= whistleL;
    pt("e: ");  pt(whistleAv);
    whistleFlag = 0;
  }
  
  // * Eo loop 
}

//


//#define USE_LEDS   1     // UNCOMMENT to remove LEDs
//#ifdef USE_LEDS
//#include <Adafruit_NeoPixel.h>
//#define PIN            10
//#define LEDs      17
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LEDs, PIN, NEO_GRB + NEO_KHZ800);
//#endif
//  #ifdef USE_LEDS
//  int offs = 13;
//  for( int l=0; l<LEDs; l++){
//    if( abs(freq-l-offs)<1.0 ){
//      pixels.setPixelColor(l, pixels.Color(0,10,120)); 
//    }
//    else 
//      pixels.setPixelColor(l, pixels.Color(0,0,0)); 
//  }
//  pixels.show();
//  delay(500);
//  #endif

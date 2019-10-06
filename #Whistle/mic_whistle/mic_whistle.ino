/*
 *    Runs fix_fft on mic data
 *    finds peak freq band
 *    interpolates to get analog value between 2 bands
 *    
 */

#include <fix_fft.h>

#define ADC   A0

#define pt(x)   Serial.print(x)
#define pl(x)   Serial.println(x)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}


uint8_t av = 0;
const int N_AVRG = 100;
int avrg[N_AVRG];

// FFT
#define FFT_N   128
byte data[FFT_N], im[FFT_N], data_avgs[8];

#define LPF     3
int whistleGap = 500;

long lastWhistle = 0;
float whistles[LPF];
int Lw = 0;
int iw = 0;
uint8_t whistleFlag = 0;

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
  
  byte bandMin = 5;       byte bandMax = 30;  
  
  for (i = bandMin; i <= bandMax; i++){
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); 
  }
  
  // * find max band
  
  int Max = -1;
  int MaxB = -1;
  for( int s=bandMin; s<=bandMax; s++){
    if( data[s] >Max){
      Max = data[s];
      MaxB = s;
    }
  }
  
  // * interpolate with neighbours
  int Np = 1; // neighbours to check
  
  int total = 0;    // get total 
  total += data[MaxB];
  total += data[MaxB-1];
  total += data[MaxB+1];

  float peak = 0;   // get proportion
  peak += 1.0*(MaxB-1) *data[MaxB-1] /total;
  peak += 1.0*(MaxB) *data[MaxB] /total;
  peak += 1.0*(MaxB+1) *data[MaxB+1] /total;
  
  if(Max>7)
  {
    // * print spectrum
    for(i=bandMin; i<=bandMax; i++) 
    {    pt(data[i]); pt("\t");      }
    pt("\n");

    // * print Peak
    pt(" # Peak\tband:\t");  pt(MaxB);   
    pt("\t mag  ");  pt(Max); 
    pt("\t Analog :   ");  pt(peak);
    pl();
    
    // Eo peak thresh
  }  
   
}

//

/*
 *   This calculates FFT with 8bit fix_fft algorithm library
 *     prints out the amplitude spectrum (entire or grouped or sub-band)
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

void loop() {
  int i, j;
  
  // * Read
  
  // * FFT
  for(i = 0; i < FFT_N; i++) {
    data[i] = analogRead(ADC);
    im[i] = 0;
  }
  fix_fft(data,im,7,0); // ; 7= m for order ; 0 for forward FFT

  // * your applic will most likely only use a fraction of the entire 64 spectrum
  byte bandMin = 5;
  byte bandMax = 30;
  
  // * AMplitude Spectrum
  for (i = bandMin; i <= bandMax; i++){
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); 
  }
  
  // * Form into bands
  // * for a wide spectrum, form frequencies into bands
  //  int BW = 2;
  //  for (i = 0; i < 64/BW; i++) {
  //    j = i *BW;
  //    data_avgs[i] = 0;
  //    for(int b=0; b<BW; b++){
  //      data_avgs[i] += data[j+b];
  //    }
  ////    if (i == 0)      data_avgs[i] >>= 1;  // KK: De-emphasize first audio band (too sensitive)
  ////    data_avgs[i] = map(data_avgs[i], 0, 20, 0, 7); // Map for output to 8x8 display
  //  }
  
  // print spectrum
  for(i=8; i<30; i++) 
  {    pt(data[i]); pt("\t");      }
  //  {    pt(data_avgs[i]); pt("\t");      } // print bands
  pt("\n");
   
}

//

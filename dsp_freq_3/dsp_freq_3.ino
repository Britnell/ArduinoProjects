
/*  Muhahahaa
 *
 */

#include "Arduino.h"
#include "Cnotes.h"

#define LENGTH 512

#define TRUE     1
#define FALSE    0

#define   SET(port, pin)    port |= (1<<pin)
#define   CLEAR(port, pin)    port &= ~(1<<pin)

#define VREF    3.3

#define led_pin   5
#define led_port  PORTB
#define led_set   DDRB

#define adc_pin   0

  
float get_volt(int adc){
  return ( ( VREF * adc ) / 255 );     }

float get_adc(float v){
  return ( ( v * 255) / VREF );        }


  
// HW pin D-6
// arduino pin dig 6


/* *** Function declarations          */
void ADC_channel_select( int ch);
void ADC_Setup( );
int ADC_read( int Pin );
void ADC_start_conversion(  );
void ADC_Enable();
void ADC_Disable();
void cereal(String Msg, int Var);
void cereaf(String Msg, float Var);


// #################################


byte rawData[LENGTH];
int count;

// Sample Frequency in kHz
const float sample_freq = 62100;

int len = sizeof(rawData);
int i,k;
long sum, sum_old;
int thresh = 0;
float freq_per = 0;
byte pd_state = 0;



void setup(){
  //
  // led_pin 
  // adc_pin 

  // Set LED as output
  SET(led_set, led_pin);   
  SET(led_port, led_pin);
  
  //   -  ADC 1
  ADC_Setup();
  
  ADC_channel_select( adc_pin );
  
  ADC_Enable();

  count = 0;
  
  Serial.begin(115200);
  //Serial.println("Setup complete");
  
  cereal("\n len is ", len);  
  cereal("\n LENGTH is ", LENGTH);  

}


int period;
long start_sampl;

void loop(){
  
  if (count < LENGTH) {
    // collest 512 samples for AutoCorrel.
    rawData[count] = ADC_readpin();    //analogRead(A0)>>2;
    count++;
  }
  else {
    //start_sampl = micros() -start_sampl;
    //cereal("\ntime: ", start_sampl);
    
    // Evaluate Samples
    sum = 0;
    pd_state = 0;
    period = 0;
    //Serial.print("\n correl \t");
    
    for(i=0; i < LENGTH; i++)
    {
      // Autocorrelation
      sum_old = sum;
      sum = 0;
      for(k=0; k < len-i; k++) sum += (rawData[k]-128)*(rawData[k+i]-128)/256;
      //cereal("\n sum : ", sum);
      //cereal("\n pd state : ", pd_state);
      
      // Peak Detect State Machine
      if (pd_state == 2 && (sum-sum_old) <=0) 
      {
        period = i;
        pd_state = 3;
      }
      if (pd_state == 1 && (sum > thresh) && (sum-sum_old) > 0) pd_state = 2;
      if (!i) {
        thresh = sum * 0.5;
        pd_state = 1;
      }
    }
    //for(i=0; i < len; i++) Serial.println(rawData[i]);
    
    
    
    if (thresh >66) {  //thresh. magnitude for detected frequency
      //SET(led_port, led_pin);      // Blink LED when detecting note
      
      freq_per = sample_freq/period;
      //Serial.println(freq_per);
      cereaf("\n\tperiod : ", period);
      
      cereaf("\tfreq : ", freq_per);
      Serial.print("\t");
      Serial.print(freq_note(freq_per));      
    }
    else
    {
      cereal("\nthresh : ", thresh);
      CLEAR(led_port, led_pin);    // Else Off
    }
    
    count = 0;
    //start_sampl = micros();
  }
  
}    /// ##### #  #  # E o loop



/*    ###################  Functions  ################
 *    #
 *    #
 */
 


void ADC_start_conversion( ){
  
  //ADC_channel_select( Pin );

  // Set Start-Conversion-Bit
  // ADSC,   bit 6
  SET(ADCSRA, 6);
  
}

int ADC_readpin( ){
  
  uint8_t adc;
  
  // If not enabled, >> Enable!
  //if( !(ADCSRA & (1<<7) ) )
  //  ADC_Enable();

  // Start conversion
  ADC_start_conversion( );

  // Check ADIF 
  // Interrupt Flag,  bit 4
  while( ADCSRA &(1<<6) )
    {           }
  
  //Serial.println(ADCH);
  
  adc = ADCH;
  //print_binary(adc);
  
  return adc;
}


int ADC_read( int Pin ){
  
  int adc;
  
  // If not enabled, >> Enable!
  //if( !(ADCSRA & (1<<7) ) )
  //  ADC_Enable();
  
  // Select correct channel through MUX
  ADC_channel_select( Pin );

  // Start conversion
  ADC_start_conversion( );

  // Check ADIF 
  // Interrupt Flag,  bit 4
  while( ADCSRA &(1<<6) )
    {           }
  
  //Serial.println(ADCH);
  
  adc = ADCH;
  //print_binary(adc);
  
  return adc;
}


void ADC_Setup( ){
    /*   AREF :   
    *    ADMUX - Pin 0 to 3
    *   7        6
    *   REFS1    REFS0     ********** 
    *   0        0         AREF - pin
    *   0        1         Vcc  3.3 / 5V
    *   1        1         Internal 1.1V
    */
    // AREF = Vcc   ( 3 / 5 V )
    CLEAR(ADMUX, 6);
    CLEAR(ADMUX, 7);
    
    /*   Left ADJUST
    *     ADLAR = 1
    *   Puts highest 8 Bits into ADCH
    *   lowest two bits into ADCL
    *     ADLAR = 0
    *   puts lowest 8 bits into ADCL
    */
    //ADMUX |= (1<<5);
    SET(ADMUX, 5);

    /*  Auto Trigger
     *  ADATE
     *  Disable with 0
     */
    CLEAR(ADCSRA, 5);

    /*  Interrupt enable
     *  to disable, clear ADIE
     */
    CLEAR(ADCSRA, 3);
    
    /* ADC clock prescalar
     *  ADPS 2  1   0
     *       0  0   0   div by 2
     *       0  0   1   div by 2
     *       0  1   0   div by 4
     *       0  1   1   div by 8
     *    MCU = 16MHz
     *    Target frequ between 100 & 200 kHz
     *    >> i Chose 64 =  1 1 0
     *    >> 16 MHz / 64 = 250 kHz
     */
    // >> ADC at 4 MHz
    SET(ADCSRA, 0);   
    CLEAR(ADCSRA, 1);
    CLEAR(ADCSRA, 2);
    
    //ADC_Enable();
    
} // Eo setup_ADC( )


void ADC_Enable(){
    /*    Enable ADC
     *    set ADEN in reg ADCSRA
     */
    SET(ADCSRA, 7);
}

void ADC_Disable(){
  
    /*    Disable ADC
     *    clear ADEN in reg ADCSRA
     */
    CLEAR(ADCSRA, 7);
}


void ADC_channel_select( int ch){  
   /* MUX Channel selection
    *  ADMUX reg,  bits 0 to 3:   
    *  MUX 3 2 1 0 
    *      0 0 0 0    ADC0
    *      0 0 0 1    ADC1
    *      0 0 1 0    ADC2
    *      ....
    *      1 0 0 0    ADC8
    */

    if(ch & 0x01)
      SET(ADMUX, 0);
    else
      CLEAR(ADMUX, 0);

    if(ch & 0x02)
      SET(ADMUX, 1);
    else
      CLEAR(ADMUX, 1);

    if(ch & 0x04)
      SET(ADMUX, 2);
    else
      CLEAR(ADMUX, 2);

    if(ch & 0x08)
      SET(ADMUX, 3);
    else
      CLEAR(ADMUX, 3);
  
} // Eo ADC channel select


void cereal(String Msg, int Var){
  Serial.print(Msg); 
  Serial.print(Var);
}

void cereaf(String Msg, float Var){
  Serial.print(Msg); 
  Serial.print(Var);
}


void print_binary( int Num){
  for(int i=7; i>=0; i--){
    if( Num & (1<<i) )
      Serial.print('1');
     else
      Serial.print('0');
  }
  
}

String freq_note( float freq){
   String note="";
   if( (freq > B2t) &&  (freq < B4t) )
   {
      if(freq<C3t)
        note="C3";
      else if(freq<Cis3t)
        note="Cis3";
      else if(freq<D3t)
        note="D3";
      else if(freq<Dis3t)
        note="Dis3";
      else if(freq<E3t)
        note="E3";
      else if(freq<F3t)
        note="F3";
      else if(freq<Fis3t)
        note="Fis3";
      else if(freq<G3t)
        note="G3";
      else if(freq<Gis3t)
        note="Gis3";
      else if(freq<A3t)
        note="A3";
      else if(freq<Ais3t)
        note="Ais3";
      else if(freq<B3t)
        note="B3";
      else 
        if(freq<C4t)
        note="C4";
      else if(freq<Cis4t)
        note="Cis4";
      else if(freq<D4t)
        note="D4";
      else if(freq<Dis4t)
        note="Dis4";
      else if(freq<E4t)
        note="E4";
      else if(freq<F4t)
        note="F4";
      else if(freq<Fis4t)
        note="Fis4";
      else if(freq<G4t)
        note="G4";
      else if(freq<Gis4t)
        note="Gis4";
      else if(freq<A4t)
        note="A4";
      else if(freq<Ais4t)
        note="Ais4";
      else if(freq<B4t)
        note="B4";
   }  // if in range
   
   if(note!="\0")
     SET(led_port, led_pin);
   else
     CLEAR(led_port,led_pin);
     
   return note;
}


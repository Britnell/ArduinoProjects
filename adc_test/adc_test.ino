 /*
 * Some example and Test code for running hardware C 
 *   on ATmega328
 */
 
#include "Arduino.h"

#define TRUE     1
#define FALSE    0

#define   SET(port, pin)    port |= (1<<pin)
#define   CLEAR(port, pin)    port &= ~(1<<pin)

#define USE_C    TRUE

#define VREF    5.0

#define led_pin   5
#define adc_pin   4
  
  
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


/*    ***   Setup                */

void setup() {
  //   Vars :
  // led_pin 
  // adc_pin 

  SET(DDRD, led_pin);   //DDRD |= (1<<led_pin);
  
  //   -  ADC 1
  ADC_Setup();
  
  //ADC_channel_select( adc_pin );
  
  //    - ADC 2
  ADC_Enable();

  Serial.begin(115200);
  Serial.println("Setup complete");
}

/*    ***   Setup                */
void loop() {
  int num;
  float ff;
  
  // HW C
  //PORTD |= (1<<led_pin);
  //PORTD &= ~(1<<led_pin);
  //SET(PORTD, led_pin);
  //CLEAR(PORTD, led_pin);
  
  //    -  ADC  3
  for( int a=0; a<6; a++) {
    num = ADC_read(a);
    ff = get_volt(num);

    cereal("\n ADC pin ", a);
    cereal(" , val= ", num);
    cereaf(" ~ Volt ", ff  );
  }
  
  //CLEAR(PORTD, led_pin);
  //delay(500);
} // Eo loop



/*    ###################  Functions  ################
 *    #
 *    #
 */
 



// Read ADC from last selected / used ADC pin / channel
int ADC_readpin( ){
  
  int adc;
  
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

// read ADC channel X
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
    CLEAR(ADCSRA, 0);   
    SET(ADCSRA, 1);
    SET(ADCSRA, 2);
    
    //ADC_Enable();
    
} // Eo setup_ADC( )

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

// Start conversion, for manual or auto-trigger mode
void ADC_start_conversion( ){
  
  //ADC_channel_select( Pin );

  // Set Start-Conversion-Bit
  // ADSC,   bit 6
  SET(ADCSRA, 6);
  
}

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

// One-line Serial function for debugging
//  prints a string plus integer
void cereal(String Msg, int Var){
  Serial.print(Msg); 
  Serial.println(Var);
}


void cereaf(String Msg, float Var){
  Serial.print(Msg); 
  Serial.println(Var);
}


void print_binary( int Num){
  for(int i=7; i>=0; i--){
    if( Num & (1<<i) )
      Serial.print('1');
     else
      Serial.print('0');
  }
  
}



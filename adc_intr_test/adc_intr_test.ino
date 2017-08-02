 /*
 * Adjusted harware C ADC example for itnerrupt with ref:
 *     http://www.avr-tutorials.com/adc/utilizing-avr-adc-interrupt-feature
 */
 
//#include "Arduino.h"

#define TRUE     1
#define FALSE    0

#define   SET(port, pin)    port |= (1<<pin)
#define   CLEAR(port, pin)    port &= ~(1<<pin)

#define VREF    3.3

// Arduino Due has LED on pin 13
#define led_pin   4
// Analog pin to be read
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

void ADC_Enable(int onoff );

void ADC_autotrigger( int Auto);

void cereal(String Msg, int Var);

void cereaf(String Msg, float Var);


/*    *    *       ADC
*/
uint8_t adc_val = 0;
uint8_t adc_vect = 0x15;

// ADC interrupt vector flag
//    22 :  0x015     ADC Conversion Complete

ISR( ADC_vect )
{
  adc_val = ADCH;
  
  // if NOT on auto-trigger :
  // ADC_start_conversion( );  
}

/*    ***   Setup                */

void setup() {
  //
  // led_pin 
  // adc_pin 

  // put your setup code here, to run once:
  SET(DDRD, led_pin);   //DDRD |= (1<<led_pin);
  
  //   -  ADC 1  
  ADC_Setup();
  
  ADC_channel_select( adc_pin );
  //    - ADC 2
  
  Serial.begin(115200);
  Serial.println("Setup complete");
  Serial.flush();
  
  sei();                  // global enable interrupts
  ADC_autotrigger( 1 );     // auto-trigger
  ADC_start_conversion( );  // start-first conversion
}

int math = 0;

/*    ***   Setup                */
void loop() {
  
  math += 1;
  cereal("\nMath is ", math);
  cereal("\n adc is : ",adc_val); 
  
  delay(100);
} // Eo loop



/*    ###################  Functions  ################
 *    #
 *    #
 */
 

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



void ADC_Setup( ){
    
    /* Enable
    */
    ADC_Enable(1);
    
    
    
    /* ADC clock prescalar
     *  ADPS 2  1   0
     *       0  0   0   div by 2
     *       0  0   1   div by 2
     *       0  1   0   div by 4
     *       0  1   1   div by 8
     *    ... etc 16, 32, 
     *       1  1   0    div by 64
     *       1  1   1   div by 128
     *    MCU = 16MHz
     *    Target frequ = 100 & 200 kHz
     *    >> 16 MHz / 64 = 250 kHz
     *    >> 64 =  1 1 0
     */
    // >> ADC at 4 MHz
    SET(ADCSRA, 0);   
    SET(ADCSRA, 1);
    SET(ADCSRA, 2);
    
    
    /*   Reference Voltage :   
    *    ADMUX - Pin 0 to 3
    *   7        6
    *   REFS1    REFS0     ********** 
    *   0        0         AREF - pin
    *   0        1         Vcc  3.3 / 5V
    *   1        1         Internal 1.1V
    */
    // AREF = Vcc   ( 3 / 5 V )
    SET(ADMUX, 6);
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
    
    
    /*  Interrupt enable
     *  to disable, clear ADIE - bit 3
     */
    SET(ADCSRA, 3);
    
    
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

void ADC_start_conversion( ){
  
  //ADC_channel_select( Pin );

  // Set Start-Conversion-Bit
  // ADSC,   bit 6
  SET(ADCSRA, 6);
  
}

void ADC_autotrigger(int Auto){
    /*  Auto Trigger
     *  ADATE - bit 5
     *  Disable with 0
     */
    if( Auto)
      SET(ADCSRA, 5);
    else
      CLEAR(ADCSRA, 5);
}

void ADC_Enable(int onoff){
    /*    Enable ADC
     *    set ADEN in reg ADCSRA
     */
    if(onoff)
      SET(ADCSRA, 7);
    else
      CLEAR(ADCSRA, 7);
}


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



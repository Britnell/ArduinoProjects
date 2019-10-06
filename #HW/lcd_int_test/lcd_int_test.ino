/*******************************************************
 *
 *  LCD demo code with Interrupt driven ADC for buttons
 *
 *
 */

#include <LiquidCrystal.h>

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//   **     *    *    *    ADC

#define   SET(port, pin)    port |= (1<<pin)
#define   CLEAR(port, pin)    port &= ~(1<<pin)

#define adc_pin   0


void ADC_channel_select( int ch);

void ADC_Setup( );

int ADC_read( int Pin );

void ADC_start_conversion(  );

void ADC_Enable(int onoff );

void ADC_autotrigger( int Auto);

void cereal(String Msg, int Var);

void cereaf(String Msg, float Var);


//     *    ******    *    LCD
///    *

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
uint8_t lcd_key = btnNONE;
uint8_t lcd_flag = 0;

int read_LCD_buttons(uint8_t adc_key_in);

void lcd_button_press( );

void clear_lcd();


//    *      *******    *    Program
//    *
//    *


// *  *    ADC interrupt vector flag  -  0x015     ADC Conversion Complete
ISR( ADC_vect )
{
  //adc_val = ADCH;
   
   lcd_key = read_LCD_buttons(ADCH);  // read the buttons
   lcd_flag = 1;
   
   //lcd_button_press();     // Button press code in ISR?!?
   
  // if NOT on auto-trigger :
  // ADC_start_conversion( );  
}


//    *      ***    *    variables
unsigned long Mills, Now;
uint8_t Secs =0;
uint8_t Mins =0;


//    *      *******    *    Setup
//    *
//    *
void setup()
{
 lcd.begin(16, 2);              // start the library
 
 clear_lcd();
 
   lcd.setCursor(0,1);
   lcd.print("Pushbutton: "); // print a simple message
   lcd.setCursor(0,0);            
   lcd.print(Mins);     
   lcd.print(":");
   lcd.print(Secs); 
   
  // ADC Setup - interrupt driven
  ADC_Setup();
  ADC_channel_select( adc_pin );
  sei();                    // global enable interrupts
  ADC_autotrigger( 0 );     // auto-trigger
  
  Mills = millis();
  
  // Do this LAST
  ADC_start_conversion( );  // start-first conversion
}


//    *      *******    *    Loop
//    *
//    *
void loop()
{
   //    without Autotrigger
   if(lcd_flag)
   {
     lcd_button_press();      // gets lcd_key from global
     lcd_flag = 0;            // clear manual flag
     ADC_start_conversion( );  // start next conversion
   }
   
   
   Now = millis();
   if(Now -Mills > 1000)
   {
     Mills += 1000;
     Secs += 1;
     if(Secs >= 60){
       Mins +=1;
       Secs =0;
     }
     lcd.setCursor(0,0);            
     lcd.print(Mins);     
     lcd.print(":");
     lcd.print(Secs);   
   }
     
   
   
  delay(300);
  
}  // Eo Loop

//    *    ******      *    LCD Functions
//    *
//    *


// interpret which button
int read_LCD_buttons(uint8_t adc_key_in)
{
    // interrupt driven, take value from ISR    
    // adc_key_in = analogRead(0); 
    
     //  RIGHT = 0      < 10
     //  UP = 25        < 40 
     //  DOWN = 64      < 80
     //  LEFT = 102     < 127
     //  SELECT = 159    < 200 <
     //  NONE =255        
     
     if (adc_key_in > 200) return btnNONE;
    
     if (adc_key_in < 10)   return btnRIGHT;  
     if (adc_key_in < 40)  return btnUP; 
     if (adc_key_in < 80)  return btnDOWN; 
     if (adc_key_in < 127)  return btnLEFT; 
     if (adc_key_in < 200)  return btnSELECT;  
     
     return btnNONE;  // when all others fail, return this...
}

void lcd_button_press( )
{
  lcd.setCursor(13,1);  
  
  switch (lcd_key)               // depending on which button was pushed, we perform an action
   {
       case btnRIGHT:
         {
         lcd.print("Ri");
         break;
         }
       case btnLEFT:
         {
         lcd.print("Le");
         break;
         }
       case btnUP:
         {
         lcd.print("Up");
         break;
         }
       case btnDOWN:
         {
         lcd.print("Do");
         break;
         }
       case btnSELECT:
         {
         lcd.print("Se");
         break;
         }
         case btnNONE:
         {
         lcd.print("--");
         break;
         }
   }  //Eo case
   
   // Eo void
}  

void clear_lcd(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}


//    *    ********    *    ADC
//    *

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


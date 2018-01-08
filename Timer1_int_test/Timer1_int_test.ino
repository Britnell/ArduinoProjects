//   https://www.instructables.com/id/Arduino-Timer-Interrupts/

#define   SET(port, pin)    port |= (1<<pin)
#define   CLEAR(port, pin)    port &= ~(1<<pin)

void timer_clock_sel();

void waveform_gen_mode();

void timer_setup(){
  
 /* ## 16.11.1  TCCR1A - Control Reg A    pg.131
 *   COM1x1:0 output mode x=A/B
 *   :1   :0
 *    0    0    disconnected
 *    0    1    toggle on match
 *    1    0    Clear on match
 *    1    1    Set on Match
 */
 // For signal Gen, for timer SET ALL
 TCCR1A = 0;
 
 /* ## 16.11.2   TCCR1B - Control Reg B
 *  noise canceler, edge select, WGM,...
 *  bits 2:0 for Clock select
 */
 TCCR1B = 0; 
 timer_clock_sel(); 
 
 /*  ## WGM 13:10   - 
 *      
 */
 waveform_gen_mode( );
 
 // 16-bit Timer counter Reg
 //    initial value
 TCNT1H = 0;
 TCNT1L = 0;
 
 /*  ##  Interrupts
 *
 *
 */
 // Enable interrupts
 SET(TIMSK1, 5);    // bit5:  ICIE - input capture interr. enable
 CLEAR(TIMSK1, 2);    // bit2:  OCIE1B - enable channel B
 CLEAR(TIMSK1, 1);    // bit1:  OCIE1A - enable channel A
 CLEAR(TIMSK1, 0);      // bit0:  TOIE1 - Overflow interrupt enable
 
 /*  ##  Interrupt flag Reg
 *      TIFR1
 */
 
}

void timer_clock_sel(){
  /*   2     1     0    bits
   *  CS12  CS11  CS10
   *   0     0     1    /1
   *   0     1     0    /8
   *   0     1     1    /64
   *   1     0     0    /256
   *   1     0     1    / 1024
   *   1     1     0    - External clock
  */ 
  SET(TCCR1B,2);     //CS12
  CLEAR(TCCR1B,1);     //CS11
  SET(TCCR1B,0);     //SCS10
}

void waveform_gen_mode(){
   
   CLEAR(TCCR1B, 4);    // WGM13
     SET( TCCR1B, 3);    // WGM12  = CTC bit
       CLEAR( TCCR1A, 1);      // WGM11
         CLEAR( TCCR1A, 0);    // WGM10

}

void setup(){

cli();//stop interrupts

//set timer0 interrupt at 2kHz
  TCCR0A = 0;  // set entire TCCR0A register to 0
  TCCR0B = 0;  // same for TCCR0B
  TCNT0  = 0;  //initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

//set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);


sei();//allow interrupts

}//end setup

void loop()
{

  
}


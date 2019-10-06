//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

/*******************************************************
This timer uses LCD Panel with built-in buttons that are connected to ADC 0.

It is a work timer 

1. it starts in ready mode, press select or down to start work interval
2.  Debug functions still active, press UP to skip seconds to :59
      press RIGHT to add 1 minute
      press LEFT to add 3 minutes
3. work interval is 30 minutes, then automatically will switch to break mode.
4. break is 5 minutes, time skipping functions still the same
5. after break it will go back into ready mode. push button to start new work interval.

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);



#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


int read_LCD_buttons();
void lcd_button_press( int lcd_key );

void clear_lcd(uint8_t ligne);
void ready_screen();
char timer_state;

// variables
unsigned long start_time;
int timer_work_interval = 30;
int timer_break_interval = 5;

int Minutes, Seconds;

uint8_t pause = 0;


void setup()
{
  Serial.begin(9600);
  
  lcd.begin(16, 2);              // start the library
  ready_screen();
}
 
 
void loop()
{
  unsigned long Now;
  float rest;
  
  lcd_button_press( read_LCD_buttons() );
  
  if(!pause)
  switch( timer_state){
    case 'w':    // work timer
      Now = millis();
      if( Now - start_time > 1000)
      {
        start_time = millis();
        
        Seconds += 1;
        if(Seconds >= 60){
          Minutes -= 1;
          Seconds -= 60;   
          // progress bar
          rest =  1.0 * Minutes / timer_work_interval;
          draw_bar_work(rest);
          
          if(rest <= 0){
            // reached 0 
            clear_lcd(3);
            start_overtime();
            //start_break();
          }  // Eo rest==0
          else
            print_time(0,0);
         // Eo 60 Seconds
        }
        else
          print_time(0,0);
        
        
      }
      break; 
      
    case 'b':  // break timer
      Now = millis();
      if( Now - start_time > 1000)
      {
        start_time += 1000;
        Seconds += 1;
        if(Seconds >= 60){
          Minutes -= 1;
          Seconds -= 60;   
          // progress bar
          rest = 1.0 * Minutes / timer_break_interval;
          draw_bar_break( rest );
          
          
          if(rest <= 0){
            // reached 0 
            timer_state = 'r';
            ready_screen();
             // Eo rest == 0
          } 
          else
           print_time(0,0); 
          
          // Eo 60 Seconds
        }
        else
          print_time(0,0);
        
        
        
      }
      break;
    
  }  // Eo switch
  
  
  delay(250);    // 250 ms
  // E o Loop
}


//    #    #    #        Screens
//    #

void lcd_button_press( int lcd_key )
{
  lcd.setCursor(13,1);  
  
  switch (lcd_key)               // depending on which button was pushed, we perform an action
   {
       case btnRIGHT:
         Minutes -= 1;
         break;
       case btnLEFT:
         Minutes -= 3;
         break;
       case btnUP:
         Seconds = 58;
         break;
       case btnDOWN:
       if(timer_state=='r')
         start_timer();
       else
       {
         if(pause)
         {
           pause=0;
         }
         else
         {
           lcd.setCursor(0,1);
           lcd.print("#Pause ");
           pause=1;
         }
       }
         break;
       case btnSELECT:
         if(timer_state=='r'){
            // start the timer! 
            start_timer();
         }
         break;
       case btnNONE:
         //lcd.print("--");
         break;
   }  //Eo case
   
   // Eo void
}  

void start_overtime(){
  //
  clear_lcd(3);
  
  lcd.setCursor(10,1);
  lcd.print(timer_break_interval);
  lcd.print("mins");
  
  Minutes = 0;
  Seconds = 0;
  
  draw_bar_over( 1.0);
  
  start_time = millis();
  
  timer_state = 'o';
}

void start_timer(){
  
  clear_lcd(3);
  lcd.setCursor(10,1);
  lcd.print(timer_work_interval);
  lcd.print("mins");

  Minutes = timer_work_interval;
  Seconds = 0;
  
  draw_bar_work(1.0);
  
  start_time = millis();
  
  timer_state = 'w';
}

void start_break(){
  
  clear_lcd(3);
  lcd.setCursor(10,1);
  lcd.print(timer_break_interval);
  lcd.print("mins");
  
  Minutes = timer_break_interval;
  Seconds = 0;
  
  draw_bar_break( 1.0);
  
  start_time = millis();
  
  timer_state = 'b';
}


void ready_screen(){
  clear_lcd(3);
  lcd.setCursor(0,0);
  lcd.print("#Ready  ");
  lcd.setCursor(1,1);
  lcd.print("Press to begin");
  timer_state = 'r';
}



//    #    #    #####    LCD functions
//    #    #    #    #    #    #    #    #    #    #    #    #    #    #    #    #
//    #

void draw_bar_over(float percent){
    draw_bar(6, 0, percent, '+');
}


void draw_bar_work(float percent){
  draw_bar(6, 0, percent, '+');
}

void draw_bar_break( float percent){
    draw_bar_5(6, 0, percent, 'O'); 
}


void draw_bar( int pos, int row, float perc, char symb ){
  lcd.setCursor(pos, row);
  int percy;
  percy = int( 100.0 * perc);
  
  //cereaf("\nperc:",perc);
  //cereal("\tint()", percy);

  if(percy %10 !=0)
    percy += 1;
    
  percy /= 10;
  //cereal("\tfinal",percy);
  
  lcd.print("|");
  
  // draws X=val+1 elements
  for( int i=0; i< percy; i++)
  {    lcd.print(symb);    }
  
  for( int i=percy; i<10; i++)
  {    lcd.print(".");    }
  
  // Eo draw bar
}


void draw_bar_5( int pos, int row, float perc, char symb ){
  lcd.setCursor(pos, row);
  int percy;
  percy = int( 100.0 * perc);
  
  //cereaf("\nperc:",perc);
  //cereal("\tint()", percy);

  if(percy %10 !=0)
    percy += 1;
    
  percy /= 20;
  //cereal("\tfinal",percy);
  
  lcd.print("|");
  
  // draws X=val+1 elements
  for( int i=0; i< percy; i++)
  {    lcd.print(symb);    }
  
  for( int i=percy; i<5; i++)
  {    lcd.print(".");    }
  
  // Eo draw bar
}

void draw_bar_over( int pos, int row, int bits, char symb ){
  
  lcd.setCursor(pos, row);
  
  //bits
  
  // draws X=val+1 elements
  for( int i=0; i< 10-bits; i++)
  {    lcd.print(" ");    }
  
  for( int i=10-bits; i<10; i++)
  {    lcd.print(symb);    }
  
  lcd.print("|");
  // Eo draw bar
}

void print_time( int pos, int row){
  
  lcd.setCursor(pos, row);
  
  if( Minutes < 10){ 
    lcd.print("0");
    lcd.print(Minutes);  }
  else{
    lcd.print(Minutes);  }

  lcd.print(":");
  
  if( Seconds < 10){ 
    lcd.print("0");
    lcd.print(Seconds);  }
  else{
    lcd.print(Seconds);  }
  
  if(timer_state=='w')
  {
    lcd.setCursor(0,1);
    lcd.print("#Work ");
  }
  if(timer_state=='b'){
    lcd.setCursor(0,1);
    lcd.print("#Coffee");
  }
  // Eo print_time
}


void clear_lcd(uint8_t ligne){
  if(ligne == 0 || ligne == 1){
    lcd.setCursor(0,ligne);
    lcd.print("                ");
  }else{
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
}


int read_LCD_buttons()
{
 int adc_key_in = analogRead(0);
 
 //lcd.setCursor(0,0);
 //lcd.print(adc_key_in);
 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result

 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   

 return btnNONE;  // when all others fail, return this...
}



void cereal(String Msg, int Var){
  Serial.print(Msg); 
  Serial.print(Var);
}

void cereaf(String Msg, float Var){
  Serial.print(Msg); 
  Serial.print(Var);
}

//  ########################################

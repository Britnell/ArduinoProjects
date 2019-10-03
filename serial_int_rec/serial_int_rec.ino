// Serial 114 1420

#include <ADCTouch.h>

#define NUM   12
#define HALF  6

#define SET(Reg, Bit)   Reg |= (0x01 << Bit)
#define CLR(Reg, Bit)   Reg &= ~(0x01 << Bit)

// Arduino Nano : INT0 = PD2

#define THRESH    5
#define PEAK_TH   20

int ref[NUM];
int val[NUM];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400); //115200
  delay(200);
  Serial.setTimeout(100);
  
  for( int x=0; x<HALF; x++) {
    ref[x] = ADCTouch.read(x +14, 1000);
  }
  
  
  //
}

char line [40];
uint8_t l = 0;

float sum, interpol;

int peak;
uint8_t peakID;

long T1 = 0;
long T2 = 0;

void loop() 
{
  
  //Serial.readBytes("#", line, 30);
  //Serial.println( Serial.available() );
//    Serial.print(",");
//    if( Serial.available() > 11){
//      Serial.print("E: ");
//      Serial.readBytes(line, Serial.available() );
//      break;
//    }
 
  
  if( Serial.readBytesUntil('#', line, 36) != 0 )
  {
      Serial.print("read line : ");
      Serial.println(line);

      char cmd = line[0];
      if( cmd == 'c')
      {
            //Serial.print("#frame: ");
            //Serial.println(line);
            
            //      Serial.print("Time: ");
            //      Serial.println(millis()-T2);
            //      T2 = millis();
            
            for( l=0; l<HALF; l++) {
              val[l] = ADCTouch.read(l +14, 20) - ref[l];
            }
            
            charay_to_array( line);
            
            // ** print Vals
            Serial.print('#');
            for( int l=0; l<12; l++) {
              Serial.print(val[l]);   Serial.print(",");
            }
            Serial.println();
      
            // ** get peak
            peak = 0;
            peakID = 0;
            
            for(l=0; l<12; l++)
            {
              // normal calc
              if( val[l] > peak )
              {
                peak = val[l];
                peakID = l;
                // Eo if THRES
              }
              // Eo for
            }
      
            // ** if peak relevant
            //
            if( peak > PEAK_TH)
            {
              // peak = relevant
              uint8_t low, hi, hi_adj, peak_adj;
              
              // only use direct neighbours, thus for calc only check those 2
              //   catch the two edge cases manually in if
              
              if(peakID == 0)
              {
                low = 11;
                peak_adj = 12;
                hi = 1;
                hi_adj = 13;
              }
              else if(peakID== 11) 
              {
                low = 10;
                peak_adj = peakID;
                hi = 0;
                hi_adj = 12;}
              else 
              {
                low = peakID -1;
                peak_adj = peakID;
                hi = peakID +1;
                hi_adj = hi;
              }
              
      
              // begin interpolation
              sum = val[peakID];
              interpol = peak_adj * val[peakID];
              
              if( val[low] > THRESH) {
                sum += val[low];
                interpol += low * val[low];
              }
              if( val[hi] > THRESH) {
                sum += val[hi];
                interpol += hi_adj * val[hi];
              }
              
              interpol /= sum;
      
              if(interpol > 12)
                interpol -= 12;
              
              // Eo if PEAK_TH
      //        Serial.print(':');
      //        Serial.println(interpol);
      
              int angela = int( interpol * 2);
              Serial.print(':a:');
              Serial.println(angela);
              
              //
            }

      }
      else if( cmd == 's')
      {
          // get state int
          Serial.println("\t PC serial State change. feed to LEDs");
      }

      // Eo if '#'
  }
  // 
  
  //delay(500);
}

void charay_to_array( char *line)
{
//  int i=0;
//  int comma = next_comma(line, i);
  int i = next_comma(line, 0);
  int comma = next_comma(line, i+1);
  
  //String sub;
  
  // returns -1 if not found
  int index = 6;
  
  //while( comma >= 0 )
  for(int a=0; a<6; a++)
  {
    if( comma >= 0)
    {
//      Serial.print("i ");
//      Serial.print(i);
//      Serial.print("to comma ");
//      Serial.println(comma);
      
      // get substring
      //sub = line.substring(i, comma);
      //Serial.print("converting sub: ");
      //Serial.println(sub);
      //val[index] = str_to_int(sub);
      
      val[index] = chs_to_int(line, i, comma);
  
      //Serial.print("as int : ");
      //Serial.println(val[index]);
      index++;
      
      i = comma+1;
      comma = next_comma(line, i);
    }
  }
  
  // Eo func
}


int chs_to_int( char * line, uint8_t sta, uint8_t fin)
{
  int res = 0;
  
  // use L-1 for base 10 calc, run loop to =L
  uint8_t L = fin -sta -1;
  
  uint8_t neg = 0;
  
  
  if(line[sta] == '-'){
    // negative number
    // start at 1, and 1 shorter
    for( uint8_t i=1; i<= L; i++)
    {
      res += ch_to_int( line[sta +i] ) * decimal( L-i);
      // Eo for
    }
    res *= -1;
    
  }
  else
  {
    // -- positive number
    
//    Serial.print("converting ");
    for( uint8_t i=0; i<= L; i++)
    {
//      Serial.print( line[sta +i]);  Serial.print( ", ");      
      res += ch_to_int( line[sta +i] ) * decimal( L-i);
      /*Serial.print("chr ");
      Serial.print(line[i] );
      Serial.print(" to ");      Serial.print(ch_to_int( line[i] ) );
      Serial.print(" = dec ");
      Serial.println( res); */
    }
//    Serial.print( "\n to ");  Serial.println( res);      
  }
  /*Serial.print("num ");
  Serial.print(line);
  Serial.print(" to int ");
  Serial.println(res);  */

  return res;
}

int next_comma( char *ray, uint8_t start)
{
  uint8_t i=start;
  while( ray[i] != '#' || ray[i] != '\0' ) {
   if( ray[i] == ',')
     return i;
   else
     i++;
     // Eo while
  }
  return -1;
}

void line_to_array( String line)
{
  int i=0;
  int comma = line.indexOf(",");
  String sub;
  
  // returns -1 if not found
  int index = 6;
  
  while( comma >= 0 )
  {
    // get substring
    sub = line.substring(i, comma);
    //Serial.print("converting sub: ");
    //Serial.println(sub);
    val[index] = str_to_int(sub);
    index++;
    
    i = comma+1;
    comma = line.indexOf(",", i);
  }
  
  // Eo func
}


int str_to_int( String line)
{
  int res = 0;
  
  // use Length-1 for Decimal place , run loop to <Dec;
  uint8_t dec = line.length() -1;
  
  uint8_t neg = 0;
  
  if(line[0] == '-'){
    // negative number
    // start at 1, and 1 shorter
    
    for( uint8_t i=1; i<= dec; i++)
    {
      res += ch_to_int( line[i] ) * decimal( dec-i);
      // Eo for
    }
    res *= -1;
    
  }
  else
  {
    // -- positive number
    
    for( uint8_t i=0; i<= dec; i++)
    {
      res += ch_to_int( line[i] ) * decimal( dec-i);
      /*Serial.print("chr ");
      Serial.print(line[i] );
      Serial.print(" to ");      Serial.print(ch_to_int( line[i] ) );
      Serial.print(" = dec ");
      Serial.println( res); */
    }
    
  }
  /*Serial.print("num ");
  Serial.print(line);
  Serial.print(" to int ");
  Serial.println(res);  */

  return res;
}


int ch_to_int( char C )
{
  int res = 0;
  switch(C)
  {
    case '0':
      res = 0;
      break;
    case '1':
      res = 1;
      break;
    case '2':
      res =  2;
      break;
    case '3':
      res =  3;
      break;
    case '4':
      res =  4;
      break;
    case '5':
      res =  5;
      break;
    case '6':
      res = 6;
      break;
    case '7':
      res =  7;
      break;
    case '8':
      res =  8;
      break;
    case '9':
      res =  9;
      break;
    default:
      res =  0;
      break;
  }
  return res;
  // Eo switch
}

int decimal( int x)
{
  int res = 1;
  for(int i=0; i<x; i++) {
    res *= 10;
  }
  return res;
}

//    ##

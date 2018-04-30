/* 
 *  Author: Thomas Britnell
 *    
 *    Demo sketch for sending different variables over Serial.
 *     * Send a signed integer 16-bit with 'i' + int , e.g. 'i123456'
 *     * Send a hex with 'x' + [hex values] , e.g. 'x7f'
 *     * Send an array of integers
 */

void setup() {
  //
  Serial.begin(115200);

  // Eo setup
}

String str;
int X;
char cmd;
char buff[30];


void loop() {
  // 
  int *data;
  
  // check serial
  
  if( Serial.available() > 0 ){
    str = Serial.readStringUntil('\n');
    int L = str.length();
    
    Serial.print("received string :");
    Serial.println(str);
    
    cmd = str[0];
    str.remove(0,1);
    
    Serial.print(" [ cmd :");
    Serial.print(cmd);
    Serial.print("\tLength :");
    Serial.println(L);


    
//    Serial.println(str);
    
    if( cmd == 'i' ){
      X = str_to_int(str);
      Serial.println(" Read int [BIN / DEC] : ");
      Serial.println(X, BIN);
//      Serial.println(X, HEX);
      Serial.println(X);
      
    }
    else if( cmd == 'x') {
      X = str_to_hex(str);
      Serial.println("Read hex [Bin, hex, dec ] ");
      Serial.print(X, BIN);  Serial.print(" ,\t");
      Serial.print(X, HEX);  Serial.print(" ,\t");
      Serial.println(X);       
    }
    else if(cmd=='c') {
      str.toCharArray(buff, L);
//      Serial.print("received line :");
//      Serial.println(buff);
      data = charay_to_array( buff, 3);
      Serial.print(" --> Array : [ ");
      for(int x=0; x<3; x++){
        Serial.print(data[x]);    Serial.print(", ");
      }
      Serial.print(" ]\n");
      // cmd== 'c'
    }
    

    
    // Eo serial.available
  }
  // Eo loop
}


                                                            //  ***    Hex

int str_to_hex( String line){
  int res = 0;
  uint8_t L = line.length();
  uint8_t i = 0;
  
  for( uint8_t x=0; x<L; x++){
    res = (res << 4);
    res += ch_to_hex( line[x] );  
    Serial.println(res, HEX);
  }
  
  return res;
}

                                                          //   **** Array


int * charay_to_array( char *line, uint8_t len )
{
//  int i=0;
//  int comma = next_comma(line, i);
  int val[len];
  
  //int i = next_comma(line, 0);
  int i = 0;
  int comma = next_comma(line, i+1);
  
  //String sub;
  
  // returns -1 if not found
  
  //while( comma >= 0 )
  for(int a=0; a<len; a++)
  {
    if( comma >= 0)
    {
        //Serial.print("i ");
        //Serial.print(i);
        //Serial.print("to comma ");
        //Serial.println(comma);
      
        // get substring
      //sub = line.substring(i, comma);
      //Serial.print("converting sub: ");
      //Serial.println(sub);
      //val[index] = str_to_int(sub);
      
      val[a] = chs_to_int(line, i, comma);
//      Serial.print(a);
//      Serial.print(" as int : ");
//      Serial.println(val[a]);
      
      i = comma+1;
      comma = next_comma(line, i);
    }
    else {
      val[a] = 0;
      //
    }
  }

  return val;
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
  int val[10];

  
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



                                                      // **  Int
int str_to_int( String line)
{
  int res = 0;
  
  // use L-1 for base 10 calc, run loop to =L
  uint8_t L = line.length() -1;
  
  uint8_t neg = 0;
  
  if(line[0] == '-'){
    // negative number
    // start at 1, and 1 shorter
    
    for( uint8_t i=1; i<= L; i++)
    {
      res += ch_to_int( line[i] ) * decimal( L-i);
      // Eo for
    }
    res *= -1;
    
  }
  else
  {
    // -- positive number
    
    for( uint8_t i=0; i<= L; i++)
    {
      res += ch_to_int( line[i] ) * decimal( L-i);
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

int ch_to_hex(char C)
{
  int res = 0;
  switch(C){
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
    case 'A':
      res =  10;
      break;
    case 'a':
      res =  10;
      break;
    case 'B':
      res =  11;
      break;
    case 'b':
      res =  11;
      break;
    case 'C':
      res =  12;
      break;
    case 'c':
      res =  12;
      break;
    case 'D':
      res =  13;
      break;
    case 'd':
      res =  13;
      break;
    case 'E':
      res =  14;
      break;
    case 'e':
      res =  14;
      break;
    case 'F':
      res =  15;
      break;
    case 'f':
      res =  15;
      break;
    default:
      res =  0;
      break;
    // Eo swithc
  }
  
  return res;
  // Eo func
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

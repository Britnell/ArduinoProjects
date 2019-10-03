#define ADC   A0
#define N_AVRG  10

#define pt(x)   Serial.print(x)
#define pl(x)   Serial.println(x)

void setup() {
  // put your setup code here, to run once:

}

uint8_t blow = 0;
uint8_t av = 0;
int avrg[N_AVRG];
long last_average = 0;
long blow_time = 0;

void loop() {

  // * Read
  
  int vai = analogRead(ADC);
  avrg[av] = vai;
  av++;   if(av>=N_AVRG)  av = 0;

  // * Diff and check
  
  int diff = vai-last_average; //
  
//  if(diff>100){
//    pt(vai); pt("\t");
//    pt(last_average); pt("\t");
//    pl(diff);
//  }
  
  if(diff>200)
  {
//    pl(average);
//    pl(diff);
    if(!blow)
    if(millis()-blow_time>400)
    {
      Serial.println("BLOW");
      blow = 1;
    }    
  }
  else{
    if(diff<50)
    if(blow){
      blow = 0;
      blow_time = millis();
      Serial.println("__");
    }
  }

  // Get average
  
  long average = 0;
  for(uint8_t x=0;x<N_AVRG;x++){
    average += avrg[x];
  }
  average /= N_AVRG;
  last_average = average;
//  delay(10);
   
}

//

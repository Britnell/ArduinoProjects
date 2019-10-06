
#define ADC   A0
#define N_AVRG  10

#define pt(x)   Serial.print(x)
#define pl(x)   Serial.println(x)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}


uint8_t av = 0;
int avrg[N_AVRG];

void loop() {

  // * Read
  
  int vai = analogRead(ADC);
  // ADjustable gain : 
  if(abs(vai-512)> 50)
    pl(vai);
   
}

//

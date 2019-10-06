/*
 * Trying to receive data on RF module. it seems to work but is noisey with random peaks and constant 0,1,0,01,1,101,
 * 
 */
#define RXpin   7

long last=0;

void RX_ISR(){
  int d = (millis() -last);
  if( d>3){
    if(digitalRead(RXpin))
      Serial.print("H-");
    else
      Serial.print("L-");
    Serial.println(d);
  }
  last=millis();
//  Serial.println(digitalRead(RXpin) );
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(RXpin, INPUT  );  //    INPUT_PULLUP
  attachInterrupt(digitalPinToInterrupt(RXpin), RX_ISR, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

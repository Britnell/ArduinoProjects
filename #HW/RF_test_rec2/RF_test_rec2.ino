/*
 * Trying to receive data on RF module. it seems to work but is noisey with random peaks and constant 0,1,0,01,1,101,
 * 
 */
#define RXpin   A0
#define thr_h   70
#define thr_l   50

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int rfx = analogRead(RXpin);

    if( rfx>thr_h){
      Serial.print("H-");
      Serial.println(rfx);
    }
    else if( rfx<thr_l){
      Serial.print("L-");
      Serial.println(rfx);
    }
    // Eo loop
}

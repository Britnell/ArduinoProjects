uint8_t pwm_val = 0;
bool dir = 1;

void setup() {
    Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite( 2, pwm_val);
  analogWrite( 3, pwm_val);
  analogWrite( 4, pwm_val);
  
  Serial.println( pwm_val);

  if(dir) { 
    // up
    pwm_val++;
    if( pwm_val >= 255){
      dir = 0;
      pwm_val = 254;
    }
  }
  else
  {
    // down
    pwm_val--;
    if(pwm_val <= 0) {
      pwm_val = 0;  
      dir = 1;
    }
  }
  
  delay(10);
}

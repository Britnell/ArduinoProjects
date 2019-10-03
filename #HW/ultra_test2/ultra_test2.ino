/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/
// defines pins numbers
const int trigPin = 3;
const int echoPin = 2;

// defines variables
long duration;
int distance;
void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(115200); // Starts the serial communication
}

uint8_t bounce = 0;
int dist_lower = 20;
int dist_upper = 40;


void loop() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    
//    Serial.print("Distance: ");
//    Serial.println(distance);

    if(distance< dist_lower){
      if(!bounce){
        bounce = 1;
//        Serial.print(distance);
        Serial.println("\tHI");
      }
      
    }
    else if(distance >dist_upper) {
      if(bounce){
        bounce=0;
//        Serial.print(distance);
        Serial.println("\tBYE");
      }
  
    }
    
    // Eo loop
}



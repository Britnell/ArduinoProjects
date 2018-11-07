//Pinbelegung des Sensors am Arduino
#define trigger 3 // Arduino Pin an HC-SR04 Trig
#define echo 2    // Arduino Pin an HC-SR04 Echo

void
// Entfernung in cm über gewöhnlichen Ultraschallsensor mit Echo und Trigger messen
int getDist()
{
  long entfernung=0;
  long zeit=0;

  digitalWrite(trigger, LOW);
  delayMicroseconds(3);
  noInterrupts();
  digitalWrite(trigger, HIGH); //Trigger Impuls 10 us
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  zeit = pulseIn(echo, HIGH); // Echo-Zeit messen
  interrupts();
  zeit = (zeit/2); // Zeit halbieren
  entfernung = zeit / 29.1; // Zeit in Zentimeter umrechnen
  return(entfernung);
}




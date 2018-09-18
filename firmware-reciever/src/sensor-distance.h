int trigPin = D6; // Trigger: yellow Jumper
int echoPin = D5; // Echo: white Jumper
long duration;
long distance=0;
long lastReadingDistance = 0;

void setupDistanceSensor(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long measureDistance() {
  /*if (millis () - lastReadingDistance >= 300)
  {
  }*/
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  
  /* Serial.print(duration);
  Serial.print("duration, ");
  Serial.print(distance);
  Serial.print("cm");
  Serial.println(); */
  return distance;
}
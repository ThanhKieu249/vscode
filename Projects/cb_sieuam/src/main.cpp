#include <Arduino.h>

int echoPin = 18;
int trigPin = 5;
int duration; 
float d_cm;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  d_cm = duration * 0.0343 / 2.0;
  
  Serial.print("Distance: ");
  Serial.print(d_cm);
  Serial.println(" cm");
  
  delay(2000);
}

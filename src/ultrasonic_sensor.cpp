#include "config.h"
#include "app.h"
#include <Arduino.h>

//define sound speed in cm/uS

long duration;
float distanceCm;
float distanceInch;

void ultrasonic_setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

float ultrasonic_get_distance() {    // Return distance in cm
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * soundSpeed/2;
  return distanceCm; 
  // // Prints the distance in the Serial Monitor
  // Serial.print("Distance (cm): ");
  // Serial.println(distanceCm);
}

/*Citation
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-hc-sr04-ultrasonic-arduino/
*/
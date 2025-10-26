/*
 * @file MinimalUltrasonic.cpp
 * @brief Library for Ultrasonic Ranging Module in a minimalist way
 * @date 25 Oct 2025
 * @author fermeridamagni (Magni Development)
 */

#include <Arduino.h>
#include "MinimalUltrasonic.h"

MinimalUltrasonic::MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut)
{
  trig = trigPin;
  echo = echoPin;
  threePins = trig == echo ? true : false;
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  timeout = timeOut;
}

unsigned int MinimalUltrasonic::timing()
{
  if (threePins)
    pinMode(trig, OUTPUT);

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  delay(30);

  if (threePins)
    pinMode(trig, INPUT);

  previousMicros = micros();
  while (!digitalRead(echo) && (micros() - previousMicros) <= timeout)
    ; // wait for the echo pin HIGH or timeout
  previousMicros = micros();
  while (digitalRead(echo) && (micros() - previousMicros) <= timeout)
    ; // wait for the echo pin LOW or timeout

  return micros() - previousMicros; // duration
}

/*
 * If the unit of measure is not passed as a parameter,
 * sby default, it will return the distance in centimeters.
 * To change the default, replace CM by INC.
 */
unsigned int MinimalUltrasonic::read(uint8_t und)
{
  return timing() / und / 2; // distance by divisor
}

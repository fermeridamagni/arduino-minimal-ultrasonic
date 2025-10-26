/*
 * @file MinimalUltrasonic.cpp
 * @brief Implementation of MinimalUltrasonic library for ultrasonic distance sensors
 * @version 2.0.0
 * @date 25 Oct 2025
 * @author fermeridamagni (Magni Development)
 * 
 * @details This implementation provides accurate distance measurements using
 *          ultrasonic time-of-flight principles. The speed of sound is assumed
 *          to be 343 m/s (at 20°C, sea level).
 * 
 * @license MIT License
 */

#include "MinimalUltrasonic.h"

// ===========================
// Physical Constants
// ===========================

/**
 * @brief Speed of sound in microseconds per centimeter (one-way)
 * Speed of sound: 343 m/s = 34300 cm/s = 0.0343 cm/µs
 * Time for round trip: 1 / (2 * 0.0343) ≈ 29.15 µs/cm
 * Using 29.1 for balance of accuracy and integer arithmetic
 */
static const float MICROSECONDS_PER_CM = 29.1;

// ===========================
// Constructors
// ===========================

MinimalUltrasonic::MinimalUltrasonic(uint8_t sigPin)
  : MinimalUltrasonic(sigPin, sigPin, 20000UL)
{
  // Delegated to main constructor with same pin for trigger and echo
}

MinimalUltrasonic::MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut)
  : _trigPin(trigPin),
    _echoPin(echoPin),
    _isThreePin(trigPin == echoPin),
    _timeout(timeOut),
    _defaultUnit(CM)
{
  // Initialize pins
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
  
  // Ensure trigger starts LOW
  digitalWrite(_trigPin, LOW);
}

// ===========================
// Public Methods
// ===========================

float MinimalUltrasonic::read(Unit unit) const
{
  unsigned long duration = timing();
  
  // If timeout occurred, return 0
  if (duration == 0) {
    return 0.0;
  }
  
  return convertToUnit(duration, unit);
}

void MinimalUltrasonic::setTimeout(unsigned long timeOut)
{
  _timeout = timeOut;
}

void MinimalUltrasonic::setMaxDistance(unsigned int distance)
{
  // Calculate timeout based on distance in cm
  // Time = Distance * 2 (round trip) * microseconds per cm
  _timeout = distance * 2 * MICROSECONDS_PER_CM;
}

unsigned long MinimalUltrasonic::getTimeout() const
{
  return _timeout;
}

MinimalUltrasonic::Unit MinimalUltrasonic::getUnit() const
{
  return _defaultUnit;
}

void MinimalUltrasonic::setUnit(Unit unit)
{
  _defaultUnit = unit;
}

// ===========================
// Private Methods
// ===========================

unsigned long MinimalUltrasonic::timing() const
{
  // For 3-pin sensors, we need to switch the pin mode
  if (_isThreePin) {
    pinMode(_trigPin, OUTPUT);
  }

  // Send trigger pulse
  // Ensure trigger is LOW for a clean pulse
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);
  
  // Send 10µs HIGH pulse to trigger
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  // For 3-pin sensors, switch to INPUT mode to receive echo
  if (_isThreePin) {
    pinMode(_trigPin, INPUT);
  }

  // Wait for echo pin to go HIGH (start of pulse)
  unsigned long startWait = micros();
  while (!digitalRead(_echoPin)) {
    if ((micros() - startWait) > _timeout) {
      return 0; // Timeout - no echo received
    }
  }

  // Measure how long the echo pin stays HIGH
  unsigned long pulseStart = micros();
  while (digitalRead(_echoPin)) {
    if ((micros() - pulseStart) > _timeout) {
      return 0; // Timeout - echo too long
    }
  }
  unsigned long pulseEnd = micros();

  // Return the duration of the echo pulse
  return pulseEnd - pulseStart;
}

float MinimalUltrasonic::convertToUnit(unsigned long microseconds, Unit unit) const
{
  // First, calculate distance in centimeters
  // Distance = (Time / 2) / microseconds_per_cm
  // Division by 2 because sound travels to object and back
  float distanceCm = microseconds / MICROSECONDS_PER_CM / 2.0;

  // Convert to requested unit
  switch (unit) {
    case CM:
      return distanceCm;
    
    case METERS:
      return distanceCm / 100.0;
    
    case MM:
      return distanceCm * 10.0;
    
    case INCHES:
      // 1 inch = 2.54 cm
      return distanceCm / 2.54;
    
    case YARDS:
      // 1 yard = 91.44 cm
      return distanceCm / 91.44;
    
    case MILES:
      // 1 mile = 160934.4 cm
      return distanceCm / 160934.4;
    
    default:
      // Default to centimeters if unknown unit
      return distanceCm;
  }
}

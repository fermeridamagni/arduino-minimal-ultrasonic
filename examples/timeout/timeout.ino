/**
 * Timeout Configuration Example
 *
 * Demonstrates different timeout settings
 * and their effect on detection range.
 */

#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup()
{
  Serial.begin(9600);

  Serial.println("Timeout Configuration Example");
  Serial.println("=============================");
  Serial.println();

  // Short range, fast readings
  sensor.setTimeout(6000UL); // ~1 meter max

  Serial.print("Timeout: ");
  Serial.print(sensor.getTimeout());
  Serial.println(" microseconds");

  float maxRange = sensor.getTimeout() / 58.8235;
  Serial.print("Max range: ~");
  Serial.print(maxRange);
  Serial.println(" cm");
  Serial.println();
}

void loop()
{
  float distance = sensor.read();

  if (distance > 0)
  {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  else
  {
    Serial.println("Out of range or error");
  }

  delay(100);
}
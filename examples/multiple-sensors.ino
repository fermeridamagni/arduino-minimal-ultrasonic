/**
 * Multiple Sensors Example
 *
 * Demonstrates using 4 ultrasonic sensors
 * for complete 360° coverage.
 */

#include <MinimalUltrasonic.h>

// Create four sensor instances
MinimalUltrasonic frontSensor(12, 13);
MinimalUltrasonic backSensor(10, 11);
MinimalUltrasonic leftSensor(8, 9);
MinimalUltrasonic rightSensor(6, 7);

void setup()
{
  Serial.begin(9600);

  Serial.println("Multiple Sensors Example");
  Serial.println("========================");
  Serial.println();
}

void loop()
{
  // Read each sensor with delays to prevent interference
  float front = frontSensor.read();
  delay(50); // Important: prevent crosstalk

  float back = backSensor.read();
  delay(50);

  float left = leftSensor.read();
  delay(50);

  float right = rightSensor.read();
  delay(50);

  // Display readings
  Serial.print("F:");
  Serial.print(front, 1);
  Serial.print(" | B:");
  Serial.print(back, 1);
  Serial.print(" | L:");
  Serial.print(left, 1);
  Serial.print(" | R:");
  Serial.println(right, 1);

  delay(200);
}
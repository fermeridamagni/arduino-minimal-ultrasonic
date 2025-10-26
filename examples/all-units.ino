/**
 * All Units Example - MinimalUltrasonic
 *
 * Demonstrates measurement in all supported units:
 * - Centimeters (CM)
 * - Meters (METERS)
 * - Millimeters (MM)
 * - Inches (INCHES)
 * - Yards (YARDS)
 * - Miles (MILES)
 *
 * Hardware:
 * - HC-SR04 Ultrasonic Sensor
 * - Arduino Uno or compatible
 */

#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup()
{
  Serial.begin(9600);

  Serial.println("MinimalUltrasonic - All Units Example");
  Serial.println("======================================");
  Serial.println();
}

void loop()
{
  Serial.println("Measuring distance in all units:");
  Serial.println("----------------------------------");

  // Centimeters (default)
  sensor.setUnit(MinimalUltrasonic::CM);
  float cm = sensor.read();
  Serial.print("Centimeters:  ");
  Serial.print(cm, 2);
  Serial.println(" cm");
  delay(50);

  // Meters
  sensor.setUnit(MinimalUltrasonic::METERS);
  float meters = sensor.read();
  Serial.print("Meters:       ");
  Serial.print(meters, 4);
  Serial.println(" m");
  delay(50);

  // Millimeters
  sensor.setUnit(MinimalUltrasonic::MM);
  float mm = sensor.read();
  Serial.print("Millimeters:  ");
  Serial.print(mm, 1);
  Serial.println(" mm");
  delay(50);

  // Inches
  sensor.setUnit(MinimalUltrasonic::INCHES);
  float inches = sensor.read();
  Serial.print("Inches:       ");
  Serial.print(inches, 2);
  Serial.println(" in");
  delay(50);

  // Yards
  sensor.setUnit(MinimalUltrasonic::YARDS);
  float yards = sensor.read();
  Serial.print("Yards:        ");
  Serial.print(yards, 4);
  Serial.println(" yd");
  delay(50);

  // Miles
  sensor.setUnit(MinimalUltrasonic::MILES);
  float miles = sensor.read();
  Serial.print("Miles:        ");
  Serial.print(miles, 7);
  Serial.println(" mi");

  Serial.println("==================================");
  Serial.println();

  // Wait before next cycle
  delay(2000);
}
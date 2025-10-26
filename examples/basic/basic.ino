#include <MinimalUltrasonic.h>

// For HC-SR04 (4-pin): specify trigger and echo pins
MinimalUltrasonic sensor(12, 13);

// For Ping/Seeed (3-pin): specify only signal pin
// MinimalUltrasonic sensor(13);

void setup()
{
  Serial.begin(9600);
  Serial.println("Distance Sensor - Basic Example");
  Serial.println("================================");
}

void loop()
{
  // Read distance in centimeters (default)
  float distance = sensor.read();

  // Display result
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Wait before next reading
  delay(1000);
}
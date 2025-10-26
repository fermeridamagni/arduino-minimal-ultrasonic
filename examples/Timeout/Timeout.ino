/*
 * Timeout Example
 * 
 * Demonstrates how to configure the timeout for ultrasonic measurements.
 * The timeout determines the maximum detectable distance.
 * 
 * Timeout Guidelines:
 * - 20000µs (default) ≈ 3.4m max range
 * - 30000µs ≈ 5.1m max range
 * - 40000µs ≈ 6.8m max range
 * 
 * Compatible with:
 * - HC-SR04 (4-pin sensor)
 * - Ping))) (3-pin sensor)
 * - Seeed SEN136B5B (3-pin sensor)
 *
 * Hardware Connections:
 * 
 * For HC-SR04 (4-pin):          For Ping)))/Seeed (3-pin):
 * ---------------------          --------------------
 * | HC-SR04 | Arduino |          | Sensor  | Arduino |
 * ---------------------          --------------------
 * |   VCC   |   5V    |          |   VCC   |   5V    |
 * |   Trig  |   12    |          |   SIG   |   13    |
 * |   Echo  |   13    |          |   GND   |   GND   |
 * |   GND   |   GND   |          --------------------
 * ---------------------
 * 
 * @created 11 Jun 2018 by Erick Simões
 * @modified 25 Oct 2025 by fermeridamagni (Magni Development)
 * 
 * This example code is released into the MIT License.
 */

#include <MinimalUltrasonic.h>

// Method 1: Set timeout in constructor
// MinimalUltrasonic ultrasonic(12, 13, 40000UL);

// Method 2: Set timeout after construction
MinimalUltrasonic ultrasonic(12, 13);

void setup() {
  Serial.begin(9600);
  Serial.println("Ultrasonic Distance Sensor - Timeout Example");
  Serial.println("---------------------------------------------");
  
  // Configure timeout for ~6.8m max range
  ultrasonic.setTimeout(40000UL);
  
  // Alternative: Set maximum distance directly
  // ultrasonic.setMaxDistance(680);  // 680 cm = 6.8m
  
  Serial.print("Timeout set to: ");
  Serial.print(ultrasonic.getTimeout());
  Serial.println(" microseconds");
  Serial.println();
}

void loop() {
  float distance = ultrasonic.read();
  
  if (distance == 0) {
    Serial.println("No object detected (timeout)");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  
  delay(1000);
}

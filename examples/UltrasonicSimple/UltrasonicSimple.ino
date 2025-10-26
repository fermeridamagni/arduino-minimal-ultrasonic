/*
 * Ultrasonic Simple Example
 * 
 * Demonstrates basic distance measurement using an ultrasonic sensor.
 * Prints the distance in centimeters to the Serial Monitor.
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
 * Note: Pin numbers can be changed according to your setup.
 * 
 * @created 3 Apr 2014 by Erick Simões
 * @modified 25 Oct 2025 by fermeridamagni (Magni Development)
 * 
 * This example code is released into the MIT License.
 */

#include <MinimalUltrasonic.h>

// For 4-pin sensor (HC-SR04): specify trigger and echo pins
MinimalUltrasonic ultrasonic(12, 13);

// For 3-pin sensor (Ping, Seeed): specify only the signal pin
// MinimalUltrasonic ultrasonic(13);

void setup() {
  Serial.begin(9600);
  Serial.println("Ultrasonic Distance Sensor - Simple Example");
  Serial.println("-------------------------------------------");
}

void loop() {
  // Read distance (default unit is centimeters)
  float distance = ultrasonic.read();
  
  // Print the result
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Wait 1 second before next measurement
  delay(1000);
}

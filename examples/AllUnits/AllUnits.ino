/*
 * All Units Example
 * 
 * Demonstrates all available units of measurement for ultrasonic distance sensing.
 * Displays the same distance in all supported units.
 * 
 * Supported Units:
 * - CM (Centimeters) - Default
 * - METERS (Meters)
 * - MM (Millimeters)
 * - INCHES (Inches)
 * - YARDS (Yards)
 * - MILES (Miles)
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
 * @created 25 Oct 2025 by fermeridamagni (Magni Development)
 * 
 * This example code is released into the MIT License.
 */

#include <MinimalUltrasonic.h>

// Configure your sensor here
MinimalUltrasonic ultrasonic(12, 13);  // For HC-SR04
// MinimalUltrasonic ultrasonic(13);   // For Ping))) or Seeed

void setup() {
  Serial.begin(9600);
  Serial.println("Ultrasonic Distance Sensor - All Units Example");
  Serial.println("==============================================");
  Serial.println();
  Serial.println("This example shows the same distance in all available units.");
  Serial.println();
}

void loop() {
  Serial.println("--- New Measurement ---");
  
  // Read distance in centimeters first
  float distCm = ultrasonic.read(MinimalUltrasonic::CM);
  
  // Check for timeout (no object detected)
  if (distCm == 0) {
    Serial.println("No object detected (timeout)");
    Serial.println();
    delay(1000);
    return;
  }
  
  // Display in all units
  Serial.print("Centimeters: ");
  Serial.print(distCm, 2);
  Serial.println(" cm");
  
  Serial.print("Meters:      ");
  Serial.print(ultrasonic.read(MinimalUltrasonic::METERS), 4);
  Serial.println(" m");
  
  Serial.print("Millimeters: ");
  Serial.print(ultrasonic.read(MinimalUltrasonic::MM), 1);
  Serial.println(" mm");
  
  Serial.print("Inches:      ");
  Serial.print(ultrasonic.read(MinimalUltrasonic::INCHES), 2);
  Serial.println(" in");
  
  Serial.print("Yards:       ");
  Serial.print(ultrasonic.read(MinimalUltrasonic::YARDS), 4);
  Serial.println(" yd");
  
  Serial.print("Miles:       ");
  Serial.print(ultrasonic.read(MinimalUltrasonic::MILES), 7);
  Serial.println(" mi");
  
  Serial.println();
  
  // Unit Conversion Table
  Serial.println("Quick Reference:");
  Serial.println("  1 inch  = 2.54 cm");
  Serial.println("  1 yard  = 91.44 cm");
  Serial.println("  1 meter = 100 cm");
  Serial.println("  1 mile  = 160934.4 cm");
  Serial.println();
  
  delay(2000);
}

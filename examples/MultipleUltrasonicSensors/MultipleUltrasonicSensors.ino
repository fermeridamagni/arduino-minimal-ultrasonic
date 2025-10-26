/*
 * Multiple Ultrasonic Sensors Example
 * 
 * Demonstrates using multiple ultrasonic sensors simultaneously and
 * reading distances in different units of measurement.
 * 
 * This example shows:
 * - Using multiple sensors at once
 * - Reading in different units (cm, meters, inches, etc.)
 * - Setting default units per sensor
 * 
 * Compatible with:
 * - HC-SR04 (4-pin sensor)
 * - Ping))) (3-pin sensor)
 * - Seeed SEN136B5B (3-pin sensor)
 *
 * Hardware Connections:
 * 
 * Sensor 1 (HC-SR04):           Sensor 2 (Ping):
 * ---------------------          --------------------
 * | HC-SR04 | Arduino |          | Ping    | Arduino |
 * ---------------------          --------------------
 * |   VCC   |   5V    |          |   VCC   |   5V    |
 * |   Trig  |   12    |          |   SIG   |   10    |
 * |   Echo  |   13    |          |   GND   |   GND   |
 * |   GND   |   GND   |          --------------------
 * ---------------------
 * 
 * Sensor 3 (Seeed):
 * --------------------
 * | Seeed   | Arduino |
 * --------------------
 * |   VCC   |   5V    |
 * |   SIG   |   8     |
 * |   GND   |   GND   |
 * --------------------
 * 
 * @created 3 Mar 2017 by Erick Simões
 * @modified 25 Oct 2025 by fermeridamagni (Magni Development)
 * 
 * This example code is released into the MIT License.
 */

#include <MinimalUltrasonic.h>

// Create three different sensors
MinimalUltrasonic sensor1(12, 13);  // HC-SR04 (4-pin)
MinimalUltrasonic sensor2(10);      // Ping))) (3-pin)
MinimalUltrasonic sensor3(8);       // Seeed (3-pin)

void setup() {
  Serial.begin(9600);
  Serial.println("Multiple Ultrasonic Sensors Example");
  Serial.println("===================================");
  Serial.println();
  
  // Optional: Set different default units for each sensor
  sensor1.setUnit(MinimalUltrasonic::CM);      // Centimeters
  sensor2.setUnit(MinimalUltrasonic::METERS);  // Meters
  sensor3.setUnit(MinimalUltrasonic::INCHES);  // Inches
}

void loop() {
  // Method 1: Read using default unit (set by setUnit)
  Serial.println("--- Using Default Units ---");
  Serial.print("Sensor 1: ");
  Serial.print(sensor1.read());
  Serial.println(" cm");

  Serial.print("Sensor 2: ");
  Serial.print(sensor2.read());
  Serial.println(" m");

  Serial.print("Sensor 3: ");
  Serial.print(sensor3.read());
  Serial.println(" inches");
  
  Serial.println();
  
  // Method 2: Specify unit explicitly in read() call
  Serial.println("--- Using Explicit Units ---");
  
  Serial.print("Sensor 1: ");
  Serial.print(sensor1.read(MinimalUltrasonic::CM));
  Serial.println(" cm");
  
  Serial.print("Sensor 1: ");
  Serial.print(sensor1.read(MinimalUltrasonic::MM));
  Serial.println(" mm");
  
  Serial.print("Sensor 2: ");
  Serial.print(sensor2.read(MinimalUltrasonic::INCHES));
  Serial.println(" inches");
  
  Serial.print("Sensor 3: ");
  Serial.print(sensor3.read(MinimalUltrasonic::YARDS));
  Serial.println(" yards");
  
  Serial.println();
  
  // Demonstrate all available units with sensor1
  Serial.println("--- All Available Units (Sensor 1) ---");
  Serial.print("Centimeters: ");
  Serial.println(sensor1.read(MinimalUltrasonic::CM));
  
  Serial.print("Meters:      ");
  Serial.println(sensor1.read(MinimalUltrasonic::METERS), 4);
  
  Serial.print("Millimeters: ");
  Serial.println(sensor1.read(MinimalUltrasonic::MM));
  
  Serial.print("Inches:      ");
  Serial.println(sensor1.read(MinimalUltrasonic::INCHES), 2);
  
  Serial.print("Yards:       ");
  Serial.println(sensor1.read(MinimalUltrasonic::YARDS), 4);
  
  Serial.print("Miles:       ");
  Serial.println(sensor1.read(MinimalUltrasonic::MILES), 6);
  
  Serial.println();
  Serial.println("===========================================");
  Serial.println();
  
  delay(2000);
}

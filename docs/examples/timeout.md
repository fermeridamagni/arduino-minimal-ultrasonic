# Timeout Configuration Example

Learn how to configure timeout for different detection ranges and optimize performance.

## Overview

This example demonstrates how to configure the sensor timeout to:

- Optimize detection range
- Improve reading speed
- Handle different scenarios

## What is Timeout?

**Timeout** is the maximum time the sensor waits for an echo before giving up.

- **Longer timeout** = Greater detection range (but slower error detection)
- **Shorter timeout** = Faster readings (but limited range)

**Formula:**

```txt
max_distance_cm = timeout_microseconds / 58.8235
```

## Hardware Required

- Arduino Uno (or compatible)
- HC-SR04 Ultrasonic Sensor
- Jumper wires

## Circuit

Same as [Basic Example](/examples/basic):

```txt
HC-SR04    Arduino
--------   -------
VCC    ->  5V
GND    ->  GND
TRIG   ->  Pin 12
ECHO   ->  Pin 13
```

## Example 1: Basic Timeout Configuration

```cpp
/**
 * Timeout Configuration Example
 * 
 * Demonstrates different timeout settings
 * and their effect on detection range.
 */

#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  Serial.println("Timeout Configuration Example");
  Serial.println("=============================");
  Serial.println();
  
  // Short range, fast readings
  sensor.setTimeout(6000UL);  // ~1 meter max
  
  Serial.print("Timeout: ");
  Serial.print(sensor.getTimeout());
  Serial.println(" microseconds");
  
  float maxRange = sensor.getTimeout() / 58.8235;
  Serial.print("Max range: ~");
  Serial.print(maxRange);
  Serial.println(" cm");
  Serial.println();
}

void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("Out of range or error");
  }
  
  delay(100);
}
```

## Example 2: Dynamic Range Selection

Choose timeout based on your needs:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

// Predefined ranges
enum Range {
  SHORT,   // ~1 meter
  MEDIUM,  // ~3.4 meters (default)
  LONG     // ~6.8 meters
};

void setRange(Range range) {
  unsigned long timeout;
  
  switch (range) {
    case SHORT:
      timeout = 6000UL;   // 1m range
      Serial.println("Range: SHORT (~1m)");
      break;
    case MEDIUM:
      timeout = 20000UL;  // 3.4m range
      Serial.println("Range: MEDIUM (~3.4m)");
      break;
    case LONG:
      timeout = 40000UL;  // 6.8m range
      Serial.println("Range: LONG (~6.8m)");
      break;
  }
  
  sensor.setTimeout(timeout);
  
  Serial.print("Timeout set to: ");
  Serial.print(timeout);
  Serial.println(" µs");
  
  float maxDist = timeout / 58.8235;
  Serial.print("Max distance: ");
  Serial.print(maxDist);
  Serial.println(" cm");
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Dynamic Range Selection");
  Serial.println("=======================");
  Serial.println();
  
  // Start with medium range
  setRange(MEDIUM);
}

void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("No reading");
  }
  
  delay(200);
  
  // Check for range change commands
  if (Serial.available()) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case 's':
      case 'S':
        setRange(SHORT);
        break;
      case 'm':
      case 'M':
        setRange(MEDIUM);
        break;
      case 'l':
      case 'L':
        setRange(LONG);
        break;
    }
  }
}
```

**Usage:**

- Type 'S' for short range
- Type 'M' for medium range
- Type 'L' for long range

## Example 3: Using setMaxDistance()

Set range by distance instead of timeout:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  Serial.println("setMaxDistance() Example");
  Serial.println("========================");
  Serial.println();
  
  // Set maximum distance in centimeters
  float maxDistance = 200.0;  // 2 meters
  sensor.setMaxDistance(maxDistance);
  
  Serial.print("Max distance set to: ");
  Serial.print(maxDistance);
  Serial.println(" cm");
  
  // Verify the timeout
  unsigned long timeout = sensor.getTimeout();
  Serial.print("Calculated timeout: ");
  Serial.print(timeout);
  Serial.println(" µs");
  Serial.println();
}

void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("Beyond max distance");
  }
  
  delay(100);
}
```

## Example 4: Performance Comparison

Compare reading speeds with different timeouts:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void benchmarkTimeout(unsigned long timeout, const char* label) {
  sensor.setTimeout(timeout);
  
  Serial.print("Testing: ");
  Serial.print(label);
  Serial.print(" (");
  Serial.print(timeout);
  Serial.println(" µs)");
  
  unsigned long startTime = millis();
  int successCount = 0;
  int errorCount = 0;
  
  // Take 20 readings
  for (int i = 0; i < 20; i++) {
    float distance = sensor.read();
    
    if (distance > 0) {
      successCount++;
    } else {
      errorCount++;
    }
  }
  
  unsigned long elapsed = millis() - startTime;
  float avgTime = (float)elapsed / 20.0;
  
  Serial.print("  Average time per reading: ");
  Serial.print(avgTime);
  Serial.println(" ms");
  
  Serial.print("  Success: ");
  Serial.print(successCount);
  Serial.print(", Errors: ");
  Serial.println(errorCount);
  
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Timeout Performance Comparison");
  Serial.println("==============================");
  Serial.println();
  Serial.println("Place object at 50cm from sensor");
  Serial.println("Starting in 3 seconds...");
  delay(3000);
  Serial.println();
  
  // Test different timeouts
  benchmarkTimeout(6000UL, "Short (1m)");
  benchmarkTimeout(12000UL, "Medium-Short (2m)");
  benchmarkTimeout(20000UL, "Medium (3.4m)");
  benchmarkTimeout(30000UL, "Medium-Long (5m)");
  benchmarkTimeout(40000UL, "Long (6.8m)");
  
  Serial.println("Benchmark complete!");
}

void loop() {
  // Empty
}
```

## Timeout Reference Table

| Timeout (µs) | Max Range (cm) | Max Range (m) | Use Case |
|-------------|----------------|---------------|----------|
| 3000 | 51 | 0.5 | Very close range, fastest |
| 6000 | 102 | 1.0 | Short range detection |
| 12000 | 204 | 2.0 | Medium-short range |
| 20000 | 340 | 3.4 | **Default** - general use |
| 30000 | 510 | 5.1 | Long range |
| 40000 | 680 | 6.8 | Maximum practical range |
| 60000 | 1020 | 10.2 | Extended range (unreliable) |

## Timeout Calculation

### Calculate Timeout from Distance

```cpp
unsigned long calculateTimeout(float maxDistanceCm) {
  // Formula: timeout = distance × 58.8235
  // Multiply by 1.2 for 20% safety margin
  unsigned long timeout = (unsigned long)(maxDistanceCm * 58.8235 * 1.2);
  return timeout;
}

void setup() {
  Serial.begin(9600);
  
  // Set max distance to 150cm
  unsigned long timeout = calculateTimeout(150.0);
  sensor.setTimeout(timeout);
  
  Serial.print("Timeout for 150cm: ");
  Serial.print(timeout);
  Serial.println(" µs");
}
```

### Calculate Max Distance from Timeout

```cpp
float calculateMaxDistance(unsigned long timeout) {
  // Formula: distance = timeout / 58.8235
  float maxDistance = timeout / 58.8235;
  return maxDistance;
}

void loop() {
  unsigned long currentTimeout = sensor.getTimeout();
  float maxDist = calculateMaxDistance(currentTimeout);
  
  Serial.print("Current max range: ");
  Serial.print(maxDist);
  Serial.println(" cm");
  
  delay(5000);
}
```

## Application-Specific Configurations

### Robot Obstacle Avoidance

Fast response needed:

```cpp
void setup() {
  Serial.begin(9600);
  
  // Short timeout for fast obstacle detection
  // Robot needs max 50cm detection
  sensor.setTimeout(3500UL);  // ~60cm max, very fast
  
  Serial.println("Robot mode: Fast obstacle detection");
}
```

### Parking Sensor

Medium range needed:

```cpp
void setup() {
  Serial.begin(9600);
  
  // Detect objects up to 2 meters
  sensor.setMaxDistance(200.0);
  
  Serial.println("Parking sensor mode");
}
```

### Room Mapping

Long range needed:

```cpp
void setup() {
  Serial.begin(9600);
  
  // Detect walls up to 5 meters
  sensor.setTimeout(30000UL);
  
  Serial.println("Room mapping mode: Long range");
}
```

### Level Monitoring

Precise short range:

```cpp
void setup() {
  Serial.begin(9600);
  
  // Monitor liquid level up to 1 meter
  sensor.setTimeout(6000UL);
  sensor.setUnit(MinimalUltrasonic::MM);  // Millimeter precision
  
  Serial.println("Level monitoring: Precise short range");
}
```

## Performance Optimization

### Trade-offs

```cpp
// SHORT TIMEOUT (fast, limited range)
sensor.setTimeout(6000UL);
// + Faster error detection
// + Higher reading frequency possible
// + Lower power consumption
// - Limited detection range

// LONG TIMEOUT (slow, extended range)
sensor.setTimeout(40000UL);
// + Extended detection range
// + Better for distant objects
// - Slower error detection
// - Lower reading frequency
// - Higher power consumption per reading
```

### Optimal Configuration

```cpp
void optimizeForApplication() {
  float expectedMaxDistance = 100.0;  // Your use case
  
  // Calculate optimal timeout with 20% margin
  unsigned long optimalTimeout = expectedMaxDistance * 58.8235 * 1.2;
  
  sensor.setTimeout(optimalTimeout);
  
  Serial.print("Optimized for ");
  Serial.print(expectedMaxDistance);
  Serial.println(" cm");
  Serial.print("Timeout: ");
  Serial.print(optimalTimeout);
  Serial.println(" µs");
}
```

## Troubleshooting

### Problem: Always Returns 0

**Cause:** Timeout too short for actual distance

**Solution:**

```cpp
// Increase timeout
sensor.setTimeout(30000UL);  // Try longer timeout
```

### Problem: Slow Readings

**Cause:** Timeout too long for application

**Solution:**

```cpp
// Reduce timeout to match your needs
sensor.setTimeout(6000UL);  // Faster for short range
```

### Problem: Intermittent Readings

**Cause:** Timeout barely sufficient

**Solution:**

```cpp
// Add safety margin
float maxDist = 100.0;
sensor.setMaxDistance(maxDist * 1.3);  // 30% margin
```

## Best Practices

1. **Match timeout to application** - Don't use default if inappropriate
2. **Add safety margin** - 20-30% above expected max distance
3. **Test in actual conditions** - Verify range in deployment environment
4. **Consider trade-offs** - Balance speed vs range
5. **Document your choice** - Comment why you chose specific timeout

## Complete Example

Full-featured timeout configuration:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void printConfiguration() {
  Serial.println("=== Current Configuration ===");
  Serial.print("Timeout: ");
  Serial.print(sensor.getTimeout());
  Serial.println(" µs");
  
  float maxRange = sensor.getTimeout() / 58.8235;
  Serial.print("Max range: ");
  Serial.print(maxRange, 1);
  Serial.println(" cm");
  
  Serial.print("Unit: ");
  switch (sensor.getUnit()) {
    case MinimalUltrasonic::CM: Serial.println("CM"); break;
    case MinimalUltrasonic::METERS: Serial.println("METERS"); break;
    default: Serial.println("Other");
  }
  Serial.println("============================");
}

void setup() {
  Serial.begin(9600);
  
  // Configure for your application
  sensor.setMaxDistance(150.0);  // 1.5 meters
  sensor.setUnit(MinimalUltrasonic::CM);
  
  printConfiguration();
}

void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("Out of range");
  }
  
  delay(100);
}
```

## Next Steps

- **[Multiple Sensors](/examples/multiple-sensors)** - Handle multiple sensors
- **[Advanced Example](/examples/advanced)** - Error handling and filtering
- **[Timeout Guide](/guide/timeout-configuration)** - Detailed timeout guide

## See Also

- [Timeout Configuration Guide](/guide/timeout-configuration)
- [Optimization Guide](/guide/optimization)
- [API Reference - setTimeout()](/api/methods#settimeout)

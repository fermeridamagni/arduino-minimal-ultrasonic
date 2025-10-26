# Troubleshooting

Solutions to common issues when using MinimalUltrasonic sensors.

## Quick Diagnostics

First, run this diagnostic code to check your sensor:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  Serial.println("=== MinimalUltrasonic Diagnostic ===");
  delay(1000);
  
  // Test multiple readings
  Serial.println("Testing 10 readings...");
  int validCount = 0;
  
  for (int i = 0; i < 10; i++) {
    float distance = sensor.read();
    
    Serial.print(i + 1);
    Serial.print(". ");
    
    if (distance > 0) {
      Serial.print("✓ ");
      Serial.print(distance);
      Serial.println(" cm");
      validCount++;
    } else {
      Serial.println("✗ Error (0)");
    }
    
    delay(200);
  }
  
  Serial.println();
  Serial.print("Valid readings: ");
  Serial.print(validCount);
  Serial.println("/10");
  
  if (validCount == 0) {
    Serial.println();
    Serial.println("⚠️ PROBLEM DETECTED");
    Serial.println("Sensor is not returning any valid readings.");
    Serial.println("Check wiring, power, and pin numbers.");
  } else if (validCount < 5) {
    Serial.println();
    Serial.println("⚠️ WARNING");
    Serial.println("Many invalid readings. Check for interference.");
  } else {
    Serial.println();
    Serial.println("✓ Sensor appears to be working normally");
  }
}

void loop() {
  // Empty
}
```

## Problem: Always Returns 0

### Symptom

```cmd
Distance: 0 cm
Distance: 0 cm
Distance: 0 cm
```

### Possible Causes & Solutions

#### 1. Wrong Pin Numbers

**Check:**

```cpp
// Verify pin numbers match your wiring
MinimalUltrasonic sensor(TRIG_PIN, ECHO_PIN);
```

**Solution:**

```cpp
// Common pin configurations:
MinimalUltrasonic sensor(12, 13);  // Uno: D12 trig, D13 echo
MinimalUltrasonic sensor(9, 10);   // Alternative
MinimalUltrasonic sensor(2, 3);    // Alternative

// For 3-pin sensors (Ping))), Seeed):
MinimalUltrasonic sensor(10);      // Single pin for both
```

#### 2. Incorrect Wiring

**Check connections:**

4-Pin HC-SR04:

```txt
Sensor    Arduino
------    -------
VCC   ->  5V
GND   ->  GND
TRIG  ->  D12 (or your TRIG_PIN)
ECHO  ->  D13 (or your ECHO_PIN)
```

3-Pin Ping))) or Seeed:

```txt
Sensor    Arduino
------    -------
VCC   ->  5V
GND   ->  GND
SIG   ->  D10 (or your signal pin)
```

**Test pins:**

```cpp
void testPins() {
  const int TRIG = 12;
  const int ECHO = 13;
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  // Test trigger
  digitalWrite(TRIG, HIGH);
  delay(100);
  Serial.print("Trigger test: ");
  Serial.println(digitalRead(TRIG) == HIGH ? "OK" : "FAIL");
  digitalWrite(TRIG, LOW);
  
  // Test echo
  Serial.print("Echo pin state: ");
  Serial.println(digitalRead(ECHO));
}
```

#### 3. Power Issues

**Symptoms:**

- Intermittent readings
- Works sometimes, not others
- Returns 0 after some time

**Solutions:**

```cpp
// 1. Use external power for multiple sensors
// Connect:
// - Sensor VCC to external 5V supply
// - All GNDs together (Arduino + Supply + Sensors)
// - Signal pins to Arduino

// 2. Add capacitor near sensor
// 100µF capacitor between VCC and GND at sensor

// 3. Check voltage
// Should be 4.5-5.5V at sensor VCC pin
```

#### 4. Sensor Type Mismatch

**Problem:** Using 3-pin constructor for 4-pin sensor

```cpp
// WRONG: 4-pin HC-SR04 with 3-pin constructor
MinimalUltrasonic sensor(12);  // Missing echo pin!

// CORRECT:
MinimalUltrasonic sensor(12, 13);  // Trig and echo
```

**Problem:** Using 4-pin constructor for 3-pin sensor

```cpp
// WRONG: 3-pin Ping))) with separate pins
MinimalUltrasonic sensor(12, 13);  // Ping))) uses same pin!

// CORRECT:
MinimalUltrasonic sensor(12);  // Single pin
```

## Problem: Erratic or Unstable Readings

### Symptom

```txt
Distance: 45.23 cm
Distance: 123.45 cm
Distance: 12.34 cm
Distance: 0 cm
Distance: 89.67 cm
```

### Solutions

#### 1. Add Filtering

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

float getStableReading() {
  float readings[5];
  int validCount = 0;
  
  // Collect valid readings
  for (int i = 0; i < 5; i++) {
    float dist = sensor.read();
    if (dist > 0) {
      readings[validCount++] = dist;
    }
    delay(30);
  }
  
  if (validCount < 3) return 0;
  
  // Sort
  for (int i = 0; i < validCount - 1; i++) {
    for (int j = i + 1; j < validCount; j++) {
      if (readings[i] > readings[j]) {
        float temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }
  
  // Return median
  return readings[validCount / 2];
}

void loop() {
  float distance = getStableReading();
  Serial.println(distance);
  delay(200);
}
```

#### 2. Check for Interference

**Multiple sensors too close:**

```cpp
// Problem: Sensors interfering
sensor1.read();
sensor2.read();  // Might pick up sensor1's echo!

// Solution: Add delays
sensor1.read();
delay(50);
sensor2.read();
```

**Electrical noise:**

```cpp
// Check for:
// - Motors running nearby
// - Relays switching
// - Other ultrasonic sources

// Solutions:
// - Move sensor away from noise sources
// - Add decoupling capacitors
// - Use shielded cables for long wires
```

#### 3. Environmental Factors

**Problem:** Soft or angled surfaces

```cpp
// Soft materials (cloth, foam) absorb ultrasound
// Angled surfaces reflect away from sensor

// Solutions:
// - Target hard, flat surfaces
// - Mount sensor perpendicular to target
// - Test with actual materials in your application
```

## Problem: Reading Too Slow

### Symptom

Long delays between readings, slow response

### Solutions

#### 1. Reduce Timeout

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  // Default timeout: 20000µs (3.4m range)
  // For 1m range: 6000µs is enough
  sensor.setTimeout(6000UL);
  
  // Result: Faster error detection, quicker readings
}
```

#### 2. Remove Delays

```cpp
// SLOW:
void loop() {
  float distance = sensor.read();
  Serial.println(distance);
  delay(500);  // Unnecessary long delay
}

// FAST:
void loop() {
  float distance = sensor.read();
  Serial.println(distance);
  delay(60);  // Minimum recommended
}
```

#### 3. Optimize Serial

```cpp
// SLOW:
Serial.begin(9600);

// FAST:
Serial.begin(115200);
```

## Problem: Works Sometimes, Not Others

### Symptom

Sensor works initially but fails after some time, or works intermittently

### Solutions

#### 1. Check Power Stability

```cpp
// Add diagnostic output
void loop() {
  float distance = sensor.read();
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Free RAM: ");
  Serial.println(freeRam());
  
  delay(100);
}

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
```

#### 2. Look for Memory Issues

```cpp
// Problem: Memory leak or fragmentation
// Check if creating objects in loop

// BAD:
void loop() {
  MinimalUltrasonic sensor(12, 13);  // Don't create here!
  float distance = sensor.read();
}

// GOOD:
MinimalUltrasonic sensor(12, 13);  // Create once, globally

void loop() {
  float distance = sensor.read();
}
```

#### 3. Check for Timing Issues

```cpp
// Problem: Reading too fast
void loop() {
  sensor.read();
  sensor.read();
  sensor.read();  // No delays!
}

// Solution: Add delays
void loop() {
  sensor.read();
  delay(60);  // Give sensor time to recover
}
```

## Problem: Inaccurate Measurements

### Symptom

Readings consistently off by some amount

### Solutions

#### 1. Verify Measurement Setup

```cpp
void calibrationTest() {
  Serial.println("=== Calibration Test ===");
  Serial.println("Place object at known distances:");
  
  float testDistances[] = {10.0, 20.0, 50.0, 100.0};
  
  for (int i = 0; i < 4; i++) {
    Serial.print("Place object at ");
    Serial.print(testDistances[i]);
    Serial.println(" cm");
    Serial.println("Press any key when ready...");
    
    while (!Serial.available()) delay(100);
    Serial.read();
    
    float measured = sensor.read();
    float error = measured - testDistances[i];
    float errorPercent = (error / testDistances[i]) * 100.0;
    
    Serial.print("Measured: ");
    Serial.print(measured);
    Serial.print(" cm, Error: ");
    Serial.print(error);
    Serial.print(" cm (");
    Serial.print(errorPercent);
    Serial.println("%)");
    Serial.println();
  }
}
```

#### 2. Check Unit Setting

```cpp
// Problem: Using wrong unit
sensor.setUnit(MinimalUltrasonic::METERS);  // But expecting CM!

// Solution: Verify unit
MinimalUltrasonic::Unit currentUnit = sensor.getUnit();
Serial.print("Current unit: ");
if (currentUnit == MinimalUltrasonic::CM) {
  Serial.println("Centimeters");
}
```

#### 3. Environmental Compensation

```cpp
// MinimalUltrasonic assumes 20°C (343 m/s)
// For accurate measurements at different temperatures:

float temperatureCompensation(float rawDistance, float tempCelsius) {
  // Speed of sound: v = 331.3 + 0.606 * T (m/s)
  float actualSpeed = 331.3 + (0.606 * tempCelsius);
  float compensated = rawDistance * (actualSpeed / 343.0);
  return compensated;
}

void loop() {
  float raw = sensor.read();
  float compensated = temperatureCompensation(raw, 25.0);  // 25°C
  
  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print(" cm, Compensated: ");
  Serial.print(compensated);
  Serial.println(" cm");
}
```

## Problem: Multiple Sensors Interfering

### Symptom

Erratic readings when multiple sensors are active

### Solutions

#### 1. Time Multiplexing

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor1(12, 13);
MinimalUltrasonic sensor2(10, 11);
MinimalUltrasonic sensor3(8, 9);

void loop() {
  // Read one at a time with delays
  float d1 = sensor1.read();
  delay(50);  // Critical!
  
  float d2 = sensor2.read();
  delay(50);
  
  float d3 = sensor3.read();
  delay(50);
  
  Serial.print("S1: ");
  Serial.print(d1);
  Serial.print(" | S2: ");
  Serial.print(d2);
  Serial.print(" | S3: ");
  Serial.println(d3);
}
```

#### 2. Physical Separation

```cpp
// Mount sensors facing different directions
// Minimum 45° angle between sensors
// Or physical barriers between them

MinimalUltrasonic front(12, 13);   // Forward
MinimalUltrasonic back(10, 11);    // Backward
MinimalUltrasonic left(8, 9);      // Left 90°
MinimalUltrasonic right(6, 7);     // Right 90°
```

## Problem: Unexpected Values

### Symptom

Getting specific strange values repeatedly

### Troubleshooting

#### Value: 0 cm

```cpp
// Meaning: No valid echo received
// Causes:
// - Nothing in detection range
// - Object too far
// - Object didn't reflect signal
// - Wiring issue
// - Sensor malfunction
```

#### Value: Very large (>400 cm)

```cpp
// Possible causes:
// - Timeout too long for actual distance
// - Interference causing false echo
// - Multiple reflections

// Solution:
sensor.setTimeout(30000UL);  // Limit range
if (distance > 400.0) {
  distance = 0;  // Treat as error
}
```

#### Value: Very small (<2 cm)

```cpp
// HC-SR04 minimum is ~2cm
// Readings below this are unreliable

if (distance > 0 && distance < 2.0) {
  Serial.println("Object too close");
  distance = 2.0;  // Clamp to minimum
}
```

## Hardware Troubleshooting

### Check Sensor Health

```cpp
void sensorHealthCheck() {
  Serial.println("=== Sensor Health Check ===");
  
  // 1. Visual inspection
  Serial.println("Visual inspection:");
  Serial.println("- Check for physical damage");
  Serial.println("- Verify transducers are clean");
  Serial.println("- Look for corrosion on pins");
  Serial.println();
  
  // 2. Voltage test
  Serial.println("Connect multimeter to VCC pin");
  Serial.println("Should read 4.5-5.5V");
  delay(5000);
  
  // 3. Functionality test
  Serial.println("Functional test (100 readings):");
  int successCount = 0;
  
  for (int i = 0; i < 100; i++) {
    float dist = sensor.read();
    if (dist > 0) successCount++;
    delay(50);
  }
  
  float successRate = (float)successCount / 100.0 * 100.0;
  Serial.print("Success rate: ");
  Serial.print(successRate);
  Serial.println("%");
  
  if (successRate > 90) {
    Serial.println("✓ Sensor is healthy");
  } else if (successRate > 70) {
    Serial.println("⚠️ Sensor may be degraded");
  } else {
    Serial.println("❌ Sensor likely faulty - consider replacement");
  }
}
```

### Common Hardware Issues

**1. Damaged Sensor**

- Symptoms: Never works, always 0
- Solution: Replace sensor (they're inexpensive)

**2. Poor Connections**

- Symptoms: Works when touched, intermittent
- Solution: Resolder connections, use dupont connectors

**3. Wrong Voltage**

- Symptoms: Weak or no readings
- Solution: Ensure 5V supply, check with multimeter

**4. ESD Damage**

- Symptoms: Erratic behavior, partial functionality
- Solution: Use ESD precautions, replace if damaged

## Debugging Tools

### Serial Monitor Helper

```cpp
void printDebugInfo() {
  float distance = sensor.read();
  unsigned long timeout = sensor.getTimeout();
  MinimalUltrasonic::Unit unit = sensor.getUnit();
  
  Serial.println("=== Debug Info ===");
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" ");
  
  switch (unit) {
    case MinimalUltrasonic::CM: Serial.println("cm"); break;
    case MinimalUltrasonic::METERS: Serial.println("m"); break;
    case MinimalUltrasonic::MM: Serial.println("mm"); break;
    case MinimalUltrasonic::INCHES: Serial.println("in"); break;
    case MinimalUltrasonic::YARDS: Serial.println("yd"); break;
    case MinimalUltrasonic::MILES: Serial.println("mi"); break;
  }
  
  Serial.print("Timeout: ");
  Serial.print(timeout);
  Serial.println(" µs");
  
  float maxRange = (timeout * 0.343) / 20.0;  // Convert to cm
  Serial.print("Max range: ");
  Serial.print(maxRange);
  Serial.println(" cm");
  
  Serial.println("==================");
}
```

## Getting Help

If problems persist:

1. **Check examples** in the library
2. **Review documentation** at [docs](/guide/getting-started)
3. **Test with simple code** (diagnostic above)
4. **Check hardware** connections and voltage
5. **Try different sensor** to rule out hardware failure

## Troubleshooting Checklist

✅ **Wiring**

- [ ] Correct pin numbers in code
- [ ] Secure physical connections
- [ ] Proper voltage (5V)
- [ ] Correct sensor type (3-pin vs 4-pin)

✅ **Code**

- [ ] Delays between readings (60ms+)
- [ ] Error checking implemented
- [ ] Appropriate timeout set
- [ ] Correct unit selected

✅ **Environment**

- [ ] Hard, flat target surface
- [ ] Perpendicular sensor mounting
- [ ] No interference sources
- [ ] Appropriate range (2-400cm)

✅ **Sensor Health**

- [ ] Visual inspection passed
- [ ] Voltage correct
- [ ] Success rate > 90%
- [ ] No physical damage

## Next Steps

- [Best Practices](/guide/best-practices) - Prevent problems
- [Error Handling](/guide/error-handling) - Handle errors gracefully
- [Getting Started](/guide/getting-started) - Review basics

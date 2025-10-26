# Best Practices

Guidelines and recommendations for using MinimalUltrasonic effectively in your projects.

## General Guidelines

### 1. Sensor Placement

Position sensors correctly for optimal performance:

```cpp
// ✓ GOOD: Clear line of sight
// Sensor facing perpendicular to target
// No obstacles in front
// Mounted securely

// ✗ BAD: Angled mounting
// Vibrating surface
// Blocked by obstacles
```

**Optimal Mounting:**

- Perpendicular to target surface (90° angle)
- Stable, vibration-free mounting
- No obstacles within 2 cm in front
- Protected from direct sunlight and rain

**Distance Guidelines:**

- Minimum: 2 cm from sensor
- Maximum: Depends on timeout setting
- Recommended working range: 5-300 cm

### 2. Power Supply

Ensure stable power delivery:

```cpp
// For multiple sensors, use external power
// Connect:
// - Sensor VCC to 5V external supply
// - Sensor GND to Arduino GND and supply GND
// - Signal pins to Arduino
```

**Power Requirements:**

- Operating voltage: 5V DC
- Current: ~15 mA per sensor during burst
- Use capacitors (100µF) near sensors for stability
- External supply for 3+ sensors

### 3. Timing and Delays

Space out measurements appropriately:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void loop() {
  float distance = sensor.read();
  Serial.println(distance);
  
  delay(100);  // Minimum 60ms recommended
}
```

**Timing Guidelines:**

- Minimum 60ms between readings (single sensor)
- Add 30-50ms delays between multiple sensors
- Avoid rapid continuous reading
- Allow sensor to "rest" between measurements

### 4. Error Handling

Always validate readings:

```cpp
void loop() {
  float distance = sensor.read();
  
  // Always check for errors
  if (distance > 0) {
    // Valid reading
    processDistance(distance);
  } else {
    // Handle error
    handleError();
  }
}
```

See [Error Handling](/guide/error-handling) guide for detailed strategies.

## Code Organization

### Initialize in Setup

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  // Configure sensor once
  sensor.setUnit(MinimalUltrasonic::CM);
  sensor.setTimeout(30000UL);
  
  // Initial test
  float test = sensor.read();
  if (test > 0) {
    Serial.println("✓ Sensor initialized");
  } else {
    Serial.println("⚠️ Sensor check failed");
  }
}

void loop() {
  // Simple reading code
  float distance = sensor.read();
  processDistance(distance);
}
```

### Use Constants

```cpp
// Define constants at top
const uint8_t TRIG_PIN = 12;
const uint8_t ECHO_PIN = 13;
const float THRESHOLD_DISTANCE = 30.0;
const unsigned long TIMEOUT = 20000UL;

MinimalUltrasonic sensor(TRIG_PIN, ECHO_PIN);

void setup() {
  sensor.setTimeout(TIMEOUT);
}

void loop() {
  float distance = sensor.read();
  
  if (distance > 0 && distance < THRESHOLD_DISTANCE) {
    // Object detected within threshold
    triggerAlert();
  }
}
```

### Create Helper Functions

```cpp
bool isObjectNear(float threshold = 30.0) {
  float distance = sensor.read();
  return (distance > 0 && distance < threshold);
}

float getAverageDistance(int samples = 5) {
  float sum = 0;
  int valid = 0;
  
  for (int i = 0; i < samples; i++) {
    float dist = sensor.read();
    if (dist > 0) {
      sum += dist;
      valid++;
    }
    delay(30);
  }
  
  return (valid > 0) ? (sum / valid) : 0;
}

void loop() {
  if (isObjectNear(50.0)) {
    Serial.println("Object nearby");
  }
  
  float avg = getAverageDistance(10);
  Serial.print("Average: ");
  Serial.println(avg);
  
  delay(500);
}
```

## Performance Optimization

### 1. Choose Appropriate Timeout

Match timeout to your needs:

```cpp
// Short-range application (< 1m)
sensor.setTimeout(6000UL);  // ~1m range, faster readings

// Medium-range (< 3m)  
sensor.setTimeout(20000UL);  // ~3.4m range (default)

// Long-range (< 7m)
sensor.setTimeout(40000UL);  // ~6.8m range, slower readings
```

**Timeout Selection:**

- Shorter timeout = faster error detection
- Longer timeout = greater range
- Match timeout to your actual detection needs
- Don't use unnecessarily long timeouts

### 2. Optimize Reading Frequency

```cpp
// Real-time obstacle avoidance
delay(60);  // ~16 Hz update rate

// Parking sensor
delay(100); // ~10 Hz update rate

// Room occupancy detection
delay(500); // ~2 Hz update rate

// Slow monitoring
delay(1000); // 1 Hz update rate
```

### 3. Use Appropriate Unit

```cpp
// Short distances: use CM or MM
sensor.setUnit(MinimalUltrasonic::CM);

// Medium distances: use CM or METERS
sensor.setUnit(MinimalUltrasonic::CM);

// Long distances: use METERS
sensor.setUnit(MinimalUltrasonic::METERS);

// Avoid MILES for short distances (unnecessary precision loss)
```

### 4. Filter Strategically

```cpp
// For stable, slow-moving objects: moving average
float getMovingAverage() {
  static float buffer[5];
  static int index = 0;
  
  buffer[index] = sensor.read();
  index = (index + 1) % 5;
  
  float sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += buffer[i];
  }
  return sum / 5;
}

// For fast-moving objects: median filter
float getMedian() {
  float readings[5];
  for (int i = 0; i < 5; i++) {
    readings[i] = sensor.read();
    delay(20);
  }
  // Sort and return median
  return readings[2];
}

// For critical applications: no filter, just retry
float getReliable() {
  for (int i = 0; i < 3; i++) {
    float dist = sensor.read();
    if (dist > 0) return dist;
    delay(50);
  }
  return 0;
}
```

## Reliability Improvements

### 1. Implement Watchdog

```cpp
unsigned long lastValidReading = 0;
const unsigned long WATCHDOG_TIMEOUT = 10000;  // 10 seconds

void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    lastValidReading = millis();
    // Process distance
  } else {
    // Check watchdog
    if (millis() - lastValidReading > WATCHDOG_TIMEOUT) {
      Serial.println("⚠️ Sensor watchdog timeout!");
      // Reset or alert
    }
  }
  
  delay(100);
}
```

### 2. Health Monitoring

```cpp
struct SensorHealth {
  unsigned long totalReadings;
  unsigned long validReadings;
  unsigned long errorReadings;
  float successRate;
};

SensorHealth health = {0};

void updateHealth(bool valid) {
  health.totalReadings++;
  if (valid) {
    health.validReadings++;
  } else {
    health.errorReadings++;
  }
  health.successRate = 
    (float)health.validReadings / health.totalReadings * 100.0;
}

void loop() {
  float distance = sensor.read();
  updateHealth(distance > 0);
  
  // Print health stats every 100 readings
  if (health.totalReadings % 100 == 0) {
    Serial.print("Success rate: ");
    Serial.print(health.successRate);
    Serial.println("%");
  }
  
  delay(100);
}
```

### 3. Calibration Check

```cpp
void calibrationCheck() {
  Serial.println("=== Calibration Check ===");
  Serial.println("Place object at 10cm and wait...");
  delay(3000);
  
  float measured = sensor.read();
  float expected = 10.0;
  float error = abs(measured - expected);
  float errorPercent = (error / expected) * 100.0;
  
  Serial.print("Measured: ");
  Serial.print(measured);
  Serial.println(" cm");
  Serial.print("Error: ");
  Serial.print(error);
  Serial.print(" cm (");
  Serial.print(errorPercent);
  Serial.println("%)");
  
  if (errorPercent < 5.0) {
    Serial.println("✓ Calibration OK");
  } else {
    Serial.println("⚠️ Calibration may be off");
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  calibrationCheck();
}
```

## Multiple Sensors Best Practices

### 1. Prevent Crosstalk

```cpp
// Method 1: Time multiplexing
void readMultipleSensors() {
  float d1 = sensor1.read();
  delay(50);  // Critical: wait between sensors
  
  float d2 = sensor2.read();
  delay(50);
  
  float d3 = sensor3.read();
}

// Method 2: Direction separation
// Mount sensors facing different directions
MinimalUltrasonic front(12, 13);   // Forward
MinimalUltrasonic back(10, 11);    // Backward
MinimalUltrasonic left(8, 9);      // Left
MinimalUltrasonic right(6, 7);     // Right
```

### 2. Manage Resources

```cpp
// Use array for many sensors
const int SENSOR_COUNT = 5;
MinimalUltrasonic sensors[SENSOR_COUNT] = {
  MinimalUltrasonic(12, 13),
  MinimalUltrasonic(10, 11),
  MinimalUltrasonic(8, 9),
  MinimalUltrasonic(6, 7),
  MinimalUltrasonic(4, 5)
};

void loop() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    float distance = sensors[i].read();
    Serial.print("S");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(distance);
    Serial.print("  ");
    delay(30);  // Prevent interference
  }
  Serial.println();
  delay(100);
}
```

See [Multiple Sensors](/guide/multiple-sensors) guide for more details.

## Environmental Considerations

### 1. Temperature Compensation

```cpp
// MinimalUltrasonic uses fixed speed of sound (343 m/s at 20°C)
// For extreme accuracy, you might need temperature compensation

// This library doesn't support it, but you can:
// 1. Use the library's CM reading
// 2. Apply temperature correction factor externally

float temperatureCorrection(float rawDistance, float tempCelsius) {
  // Speed of sound varies with temperature
  // v = 331.3 + (0.606 * T) m/s
  float speedAtTemp = 331.3 + (0.606 * tempCelsius);
  float correctionFactor = speedAtTemp / 343.0;
  return rawDistance * correctionFactor;
}
```

### 2. Surface Considerations

```cpp
// Different surfaces behave differently:

// ✓ GOOD surfaces (strong reflections):
// - Flat hard surfaces (walls, floors)
// - Perpendicular orientation
// - Smooth materials (wood, metal, plastic)

// ⚠️ PROBLEMATIC surfaces:
// - Soft materials (cloth, foam, carpet)
// - Angled surfaces (sound bounces away)
// - Irregular shapes (scatter sound)
// - Very small objects (may not reflect enough)

// Solution: Test with actual materials
// Adjust timeouts and thresholds accordingly
```

### 3. Environmental Factors

```cpp
// Be aware of:
// - Humidity: affects speed of sound slightly
// - Air pressure: minimal effect
// - Wind: can affect outdoor readings
// - Ultrasonic noise: other sensors, animals
// - Electrical noise: motors, relays nearby

// Mitigation:
// - Use shielded cables for long runs
// - Add filtering for noisy environments
// - Physical barriers between sensors
// - Test in actual deployment environment
```

## Documentation and Maintenance

### 1. Comment Your Code

```cpp
// Bad: No context
MinimalUltrasonic s(12, 13);
float d = s.read();

// Good: Clear purpose
// Front-facing sensor for obstacle detection
// Range: 5-200cm
MinimalUltrasonic frontSensor(12, 13);
const float OBSTACLE_THRESHOLD = 30.0;  // 30cm warning distance

void loop() {
  float distance = frontSensor.read();
  
  // Check for obstacles within threshold
  if (distance > 0 && distance < OBSTACLE_THRESHOLD) {
    activateWarning();
  }
}
```

### 2. Keep Records

```txt
Sensor Configuration Log:
- Date: 2024-01-15
- Sensor: HC-SR04
- Trig Pin: D12
- Echo Pin: D13
- Unit: Centimeters
- Timeout: 20000µs (3.4m range)
- Mounting: Front bumper, 5cm from ground
- Purpose: Collision avoidance
- Test Results: 95% accuracy at 10-100cm
```

### 3. Version Control

```cpp
// MinimalUltrasonic Sensor Module
// Version: 1.0
// Last Modified: 2024-01-15
// Author: Your Name
// Purpose: Distance monitoring for robot navigation

#define MODULE_VERSION "1.0"
#define TRIG_PIN 12
#define ECHO_PIN 13

MinimalUltrasonic sensor(TRIG_PIN, ECHO_PIN);
```

## Testing Checklist

Before deploying your project:

✅ **Basic Functionality**

- [ ] Sensor returns valid readings
- [ ] Error handling works correctly
- [ ] Readings are within expected range

✅ **Reliability**

- [ ] Tested for at least 1000 readings
- [ ] Error rate is acceptable (< 5%)
- [ ] No false positives/negatives

✅ **Performance**

- [ ] Reading frequency meets requirements
- [ ] Response time is acceptable
- [ ] No interference with other sensors

✅ **Environmental**

- [ ] Tested with target materials
- [ ] Works at expected distances
- [ ] Functions in deployment environment

✅ **Code Quality**

- [ ] Well commented
- [ ] Error handling implemented
- [ ] Configuration documented

✅ **Long-term**

- [ ] Run for extended period (hours)
- [ ] Monitor for degradation
- [ ] Check stability over time

## Common Pitfalls to Avoid

### ❌ Don't: Read Too Frequently

```cpp
// BAD: No delay between readings
void loop() {
  float d = sensor.read();  // Too fast!
  Serial.println(d);
}
```

### ✅ Do: Add Appropriate Delays

```cpp
// GOOD: Proper timing
void loop() {
  float distance = sensor.read();
  Serial.println(distance);
  delay(100);  // Allow time between readings
}
```

### ❌ Don't: Ignore Errors

```cpp
// BAD: No error checking
void loop() {
  float d = sensor.read();
  activateMotor(d);  // Might use invalid 0!
}
```

### ✅ Do: Validate Readings

```cpp
// GOOD: Check for valid data
void loop() {
  float distance = sensor.read();
  if (distance > 0) {
    activateMotor(distance);
  }
}
```

### ❌ Don't: Use Wrong Timeout

```cpp
// BAD: Timeout doesn't match use case
sensor.setTimeout(60000UL);  // 10m range for 50cm application!
```

### ✅ Do: Match Timeout to Range

```cpp
// GOOD: Appropriate timeout
sensor.setTimeout(6000UL);  // ~1m range for 50cm application
```

## Summary

**Key Takeaways:**

1. **Always validate** sensor readings
2. **Add delays** between measurements (60ms+)
3. **Use appropriate** timeouts for your range
4. **Handle errors** gracefully
5. **Test thoroughly** in actual conditions
6. **Document** your configuration
7. **Monitor** sensor health
8. **Prevent** crosstalk with multiple sensors

## Next Steps

- [Error Handling](/guide/error-handling) - Handle errors effectively
- [Troubleshooting](/guide/troubleshooting) - Solve common problems
- [Multiple Sensors](/guide/multiple-sensors) - Work with multiple sensors
- [Optimization](/guide/optimization) - Performance tips

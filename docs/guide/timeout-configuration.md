# Timeout Configuration

Configure the maximum wait time for ultrasonic echo responses to control the maximum detection range.

## Understanding Timeout

### What is Timeout?

The timeout determines how long the sensor waits for an echo response before giving up. This directly controls the maximum detectable distance.

**Formula:**

```txt
Max Distance (cm) = (Timeout in µs / 29.1) / 2
```

### Why Configure Timeout?

- **Adjust maximum range** - Detect objects further away
- **Improve response time** - Faster readings when no object present
- **Match your application** - Indoor vs outdoor needs
- **Optimize performance** - Balance speed vs range

## Default Timeout

By default, the timeout is **20,000 microseconds (20ms)**:

```cpp
MinimalUltrasonic sensor(12, 13);  // Default: 20000µs
```

This provides approximately **3.4 meters** maximum range.

## Setting Timeout

### Method 1: In Constructor

Set timeout when creating the sensor:

```cpp
// 30ms timeout (~5.1m range)
MinimalUltrasonic sensor(12, 13, 30000UL);

// 40ms timeout (~6.8m range)
MinimalUltrasonic sensor(12, 13, 40000UL);
```

### Method 2: Using setTimeout()

Change timeout after creation:

```cpp
MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  // Set to 40ms
  sensor.setTimeout(40000UL);
}
```

### Method 3: Using setMaxDistance()

Set by desired distance (easier!):

```cpp
MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  // Set max distance to 500 cm (5 meters)
  sensor.setMaxDistance(500);
}
```

This automatically calculates the appropriate timeout.

## Timeout Guidelines

### Common Timeout Values

| Timeout (µs) | Max Range | Use Case |
|--------------|-----------|----------|
| 10,000 | ~1.7m | Close-range indoor |
| 20,000 | ~3.4m | Default, general purpose |
| 30,000 | ~5.1m | Medium-range applications |
| 40,000 | ~6.8m | Long-range outdoor |
| 50,000 | ~8.6m | Maximum practical range |

### Choosing the Right Timeout

**Short Timeout (< 20ms)**

- ✅ Faster readings
- ✅ Better responsiveness
- ✅ Good for close-range
- ❌ Limited detection range

**Medium Timeout (20-30ms)**

- ✅ Balanced performance
- ✅ Good range (3-5m)
- ✅ Reasonable speed
- ✅ Recommended for most applications

**Long Timeout (> 30ms)**

- ✅ Maximum detection range
- ✅ Outdoor applications
- ❌ Slower when no object detected
- ❌ May capture false echoes

## Complete Examples

### Example 1: Indoor Robot

```cpp
#include <MinimalUltrasonic.h>

// Short timeout for fast response
MinimalUltrasonic sensor(12, 13, 15000UL);

void setup() {
  Serial.begin(9600);
  Serial.println("Indoor Navigation - 2.5m range");
}

void loop() {
  float distance = sensor.read();
  
  if (distance > 0 && distance < 30) {
    Serial.println("OBSTACLE DETECTED!");
  } else {
    Serial.print("Clear: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  
  delay(50);  // Fast loop
}
```

### Example 2: Parking Sensor

```cpp
#include <MinimalUltrasonic.h>

// Medium timeout for car parking
MinimalUltrasonic sensor(12, 13, 25000UL);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  float distance = sensor.read();
  
  if (distance == 0) {
    // No car detected
    digitalWrite(LED_BUILTIN, LOW);
  } else if (distance < 50) {
    // Too close!
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("⚠️ STOP!");
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  
  delay(200);
}
```

### Example 3: Outdoor Distance Monitor

```cpp
#include <MinimalUltrasonic.h>

// Long timeout for outdoor use
MinimalUltrasonic sensor(12, 13, 50000UL);

void setup() {
  Serial.begin(9600);
  Serial.println("Outdoor Monitor - 8.6m range");
  
  Serial.print("Timeout: ");
  Serial.print(sensor.getTimeout());
  Serial.println(" µs");
}

void loop() {
  float meters = sensor.read(MinimalUltrasonic::METERS);
  
  if (meters == 0) {
    Serial.println("Out of range (>8.6m)");
  } else {
    Serial.print("Distance: ");
    Serial.print(meters, 2);
    Serial.println(" m");
  }
  
  delay(1000);
}
```

### Example 4: Dynamic Timeout

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  Serial.println("Dynamic Timeout Demo");
  Serial.println("Commands: 1=Short, 2=Medium, 3=Long");
}

void loop() {
  // Check for serial commands
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    switch(cmd) {
      case '1':
        sensor.setTimeout(15000UL);
        Serial.println("→ Short range (2.5m)");
        break;
      case '2':
        sensor.setTimeout(30000UL);
        Serial.println("→ Medium range (5.1m)");
        break;
      case '3':
        sensor.setTimeout(50000UL);
        Serial.println("→ Long range (8.6m)");
        break;
    }
  }
  
  float distance = sensor.read();
  
  if (distance > 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("Out of range");
  }
  
  delay(500);
}
```

## Query Timeout

Check current timeout setting:

```cpp
unsigned long currentTimeout = sensor.getTimeout();

Serial.print("Current timeout: ");
Serial.print(currentTimeout);
Serial.println(" microseconds");

// Calculate max range
float maxRange = (currentTimeout / 29.1) / 2;
Serial.print("Max range: ~");
Serial.print(maxRange);
Serial.println(" cm");
```

## Performance Considerations

### Timeout vs Speed

When no object is detected, the sensor waits for the full timeout:

```cpp
// Short timeout = faster response when nothing detected
sensor.setTimeout(10000UL);  // ~58ms total per reading

// Long timeout = slower response when nothing detected  
sensor.setTimeout(50000UL);  // ~50ms wait time
```

### Optimization Tips

**For Fast Response:**

```cpp
// Use shortest timeout needed
sensor.setMaxDistance(200);  // Only 2m range

void loop() {
  sensor.read();
  delay(30);  // Can read ~33 times per second
}
```

**For Maximum Range:**

```cpp
// Use longer timeout
sensor.setMaxDistance(800);  // 8m range

void loop() {
  sensor.read();
  delay(100);  // Slower loop when range is priority
}
```

## Common Issues

### Always Returns 0

**Problem:** Distance always returns 0

**Solution:** Increase timeout

```cpp
sensor.setTimeout(40000UL);  // Increase range
```

### Slow Response

**Problem:** Readings are slow

**Solution:** Reduce timeout

```cpp
sensor.setTimeout(15000UL);  // Faster response
```

### False Readings

**Problem:** Getting unexpected distances

**Solution:** Use appropriate timeout for environment

```cpp
// Indoor: shorter timeout
sensor.setTimeout(20000UL);

// Outdoor: longer timeout ok
sensor.setTimeout(40000UL);
```

## Best Practices

### 1. Match Timeout to Application

```cpp
// Indoor robot: short timeout
MinimalUltrasonic indoor(12, 13, 15000UL);

// Outdoor monitoring: long timeout
MinimalUltrasonic outdoor(12, 13, 50000UL);
```

### 2. Use setMaxDistance() for Clarity

```cpp
// More readable
sensor.setMaxDistance(300);  // 3 meters

// vs
sensor.setTimeout(17460UL);  // What distance is this?
```

### 3. Document Your Timeout Choice

```cpp
// Set timeout for indoor use (max 2.5m range)
sensor.setTimeout(15000UL);
```

### 4. Test in Your Environment

```cpp
void calibrateTimeout() {
  for (int timeout = 20000; timeout <= 50000; timeout += 10000) {
    sensor.setTimeout(timeout);
    Serial.print("Timeout: ");
    Serial.print(timeout);
    Serial.print("µs -> ");
    Serial.print(sensor.read());
    Serial.println(" cm");
    delay(1000);
  }
}
```

## Advanced: Calculate Timeout

If you need a specific maximum distance:

```cpp
// Calculate timeout for desired max distance
unsigned long calculateTimeout(float maxDistanceCm) {
  return maxDistanceCm * 2 * 29.1;
}

// Use it
sensor.setTimeout(calculateTimeout(600));  // 6m max
```

## Next Steps

- [Error Handling](/guide/error-handling) - Handle timeout errors
- [Multiple Sensors](/guide/multiple-sensors) - Configure multiple sensors
- [Optimization](/guide/optimization) - Performance tuning
- [Example](/examples/timeout) - Complete timeout example

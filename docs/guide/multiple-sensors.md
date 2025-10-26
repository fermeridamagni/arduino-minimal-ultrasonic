# Multiple Sensors

Use multiple ultrasonic sensors simultaneously in your Arduino project.

## Overview

MinimalUltrasonic supports using multiple sensors at the same time. Each sensor operates independently with its own configuration.

## Basic Setup

### Creating Multiple Sensors

Simply create multiple sensor objects:

```cpp
#include <MinimalUltrasonic.h>

// Front sensor
MinimalUltrasonic front(12, 13);

// Left sensor
MinimalUltrasonic left(10, 11);

// Right sensor
MinimalUltrasonic right(8, 9);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float distFront = front.read();
  float distLeft = left.read();
  float distRight = right.read();
  
  Serial.print("Front: "); Serial.print(distFront); Serial.print(" cm | ");
  Serial.print("Left: "); Serial.print(distLeft); Serial.print(" cm | ");
  Serial.print("Right: "); Serial.println(distRight); Serial.println(" cm");
  
  delay(100);
}
```

## Independent Configuration

Each sensor can have different settings:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic front(12, 13);
MinimalUltrasonic back(10, 11);

void setup() {
  Serial.begin(9600);
  
  // Different timeouts
  front.setTimeout(20000UL);  // 3.4m range
  back.setTimeout(40000UL);   // 6.8m range
  
  // Different units
  front.setUnit(MinimalUltrasonic::CM);
  back.setUnit(MinimalUltrasonic::METERS);
}

void loop() {
  Serial.print("Front: ");
  Serial.print(front.read());
  Serial.print(" cm, Back: ");
  Serial.print(back.read());
  Serial.println(" m");
  
  delay(500);
}
```

## Practical Examples

### Example 1: Robot Navigation

```cpp
#include <MinimalUltrasonic.h>

// Four sensors for complete coverage
MinimalUltrasonic front(12, 13);
MinimalUltrasonic back(10, 11);
MinimalUltrasonic left(8, 9);
MinimalUltrasonic right(6, 7);

const float SAFE_DISTANCE = 30.0;  // 30 cm

void setup() {
  Serial.begin(9600);
  Serial.println("Robot Navigation System");
}

void loop() {
  float f = front.read();
  float b = back.read();
  float l = left.read();
  float r = right.read();
  
  // Decision making
  if (f < SAFE_DISTANCE && f > 0) {
    Serial.println("⚠️ Obstacle ahead! STOP");
  } else if (l < SAFE_DISTANCE && l > 0) {
    Serial.println("← Obstacle on left");
  } else if (r < SAFE_DISTANCE && r > 0) {
    Serial.println("→ Obstacle on right");
  } else {
    Serial.println("✓ Path clear");
  }
  
  delay(200);
}
```

### Example 2: Smart Parking System

```cpp
#include <MinimalUltrasonic.h>

// Sensors for each parking spot
MinimalUltrasonic spot1(12, 13);
MinimalUltrasonic spot2(10, 11);
MinimalUltrasonic spot3(8, 9);

const float CAR_PRESENT = 100.0;  // Less than 100cm = car present

void setup() {
  Serial.begin(9600);
  Serial.println("Parking System Monitor");
  
  // Set appropriate timeout
  spot1.setTimeout(30000UL);
  spot2.setTimeout(30000UL);
  spot3.setTimeout(30000UL);
}

void loop() {
  // Check each spot
  bool occupied1 = (spot1.read() < CAR_PRESENT && spot1.read() > 0);
  bool occupied2 = (spot2.read() < CAR_PRESENT && spot2.read() > 0);
  bool occupied3 = (spot3.read() < CAR_PRESENT && spot3.read() > 0);
  
  // Display status
  Serial.println("Parking Status:");
  Serial.print("Spot 1: "); Serial.println(occupied1 ? "🚗 Occupied" : "✓ Free");
  Serial.print("Spot 2: "); Serial.println(occupied2 ? "🚗 Occupied" : "✓ Free");
  Serial.print("Spot 3: "); Serial.println(occupied3 ? "🚗 Occupied" : "✓ Free");
  Serial.println();
  
  delay(2000);
}
```

### Example 3: Security Perimeter

```cpp
#include <MinimalUltrasonic.h>

// Sensors around perimeter
MinimalUltrasonic north(12, 13);
MinimalUltrasonic south(10, 11);
MinimalUltrasonic east(8, 9);
MinimalUltrasonic west(6, 7);

const float PERIMETER_DISTANCE = 200.0;  // 2 meters

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Security Perimeter System");
}

void loop() {
  // Check all sensors
  float n = north.read();
  float s = south.read();
  float e = east.read();
  float w = west.read();
  
  // Detect breach
  bool breach = false;
  
  if (n > 0 && n < PERIMETER_DISTANCE) {
    Serial.println("⚠️ BREACH: North side");
    breach = true;
  }
  if (s > 0 && s < PERIMETER_DISTANCE) {
    Serial.println("⚠️ BREACH: South side");
    breach = true;
  }
  if (e > 0 && e < PERIMETER_DISTANCE) {
    Serial.println("⚠️ BREACH: East side");
    breach = true;
  }
  if (w > 0 && w < PERIMETER_DISTANCE) {
    Serial.println("⚠️ BREACH: West side");
    breach = true;
  }
  
  // Alert
  digitalWrite(LED_BUILTIN, breach ? HIGH : LOW);
  
  if (!breach) {
    Serial.println("✓ Perimeter secure");
  }
  
  delay(500);
}
```

## Important Considerations

### 1. Crosstalk Prevention

Ultrasonic sensors can interfere with each other. To prevent crosstalk:

**Method 1: Time Multiplexing**

```cpp
void loop() {
  // Read sensors one at a time with delays
  float dist1 = sensor1.read();
  delay(50);  // Wait between sensors
  
  float dist2 = sensor2.read();
  delay(50);
  
  float dist3 = sensor3.read();
  delay(50);
}
```

**Method 2: Spatial Separation**

```cpp
// Mount sensors far apart or facing different directions
MinimalUltrasonic front(12, 13);   // Facing forward
MinimalUltrasonic back(10, 11);    // Facing backward (opposite)
```

**Method 3: Sequential Reading**

```cpp
MinimalUltrasonic sensors[] = {
  MinimalUltrasonic(12, 13),
  MinimalUltrasonic(10, 11),
  MinimalUltrasonic(8, 9)
};

void loop() {
  for (int i = 0; i < 3; i++) {
    float dist = sensors[i].read();
    Serial.print("Sensor "); 
    Serial.print(i);
    Serial.print(": ");
    Serial.println(dist);
    delay(30);  // Delay between reads
  }
}
```

### 2. Pin Management

Ensure each sensor uses unique pins:

```cpp
// ✓ Good - unique pins
MinimalUltrasonic s1(12, 13);
MinimalUltrasonic s2(10, 11);
MinimalUltrasonic s3(8, 9);

// ✗ Bad - shared pins will conflict
MinimalUltrasonic s1(12, 13);
MinimalUltrasonic s2(12, 13);  // Conflict!
```

### 3. Memory Considerations

Each sensor uses 8 bytes of RAM:

```cpp
// 5 sensors = 40 bytes
MinimalUltrasonic s1(12, 13);
MinimalUltrasonic s2(10, 11);
MinimalUltrasonic s3(8, 9);
MinimalUltrasonic s4(6, 7);
MinimalUltrasonic s5(4, 5);

// Total RAM: 5 × 8 = 40 bytes (minimal!)
```

### 4. Reading Strategy

**Simultaneous (Fast but may interfere):**

```cpp
void loop() {
  float d1 = sensor1.read();
  float d2 = sensor2.read();
  float d3 = sensor3.read();
  // Fast but sensors may interfere
}
```

**Sequential (Slower but reliable):**

```cpp
void loop() {
  float d1 = sensor1.read();
  delay(30);
  float d2 = sensor2.read();
  delay(30);
  float d3 = sensor3.read();
  delay(30);
  // Slower but no interference
}
```

## Advanced Patterns

### Sensor Array

```cpp
#include <MinimalUltrasonic.h>

const int SENSOR_COUNT = 4;

struct SensorConfig {
  uint8_t trigPin;
  uint8_t echoPin;
  const char* name;
};

SensorConfig configs[SENSOR_COUNT] = {
  {12, 13, "Front"},
  {10, 11, "Back"},
  {8, 9, "Left"},
  {6, 7, "Right"}
};

MinimalUltrasonic* sensors[SENSOR_COUNT];

void setup() {
  Serial.begin(9600);
  
  // Initialize sensors
  for (int i = 0; i < SENSOR_COUNT; i++) {
    sensors[i] = new MinimalUltrasonic(
      configs[i].trigPin, 
      configs[i].echoPin
    );
  }
}

void loop() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    float distance = sensors[i]->read();
    
    Serial.print(configs[i].name);
    Serial.print(": ");
    Serial.print(distance);
    Serial.print(" cm  ");
    
    delay(30);  // Prevent interference
  }
  Serial.println();
  delay(500);
}
```

### Closest Object Detection

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensors[] = {
  MinimalUltrasonic(12, 13),
  MinimalUltrasonic(10, 11),
  MinimalUltrasonic(8, 9)
};

const int SENSOR_COUNT = 3;

void loop() {
  float minDistance = 999999;
  int closestSensor = -1;
  
  // Find closest object
  for (int i = 0; i < SENSOR_COUNT; i++) {
    float dist = sensors[i].read();
    
    if (dist > 0 && dist < minDistance) {
      minDistance = dist;
      closestSensor = i;
    }
    
    delay(30);
  }
  
  if (closestSensor >= 0) {
    Serial.print("Closest object: Sensor ");
    Serial.print(closestSensor);
    Serial.print(" at ");
    Serial.print(minDistance);
    Serial.println(" cm");
  } else {
    Serial.println("No objects detected");
  }
  
  delay(500);
}
```

## Mixing 3-Pin and 4-Pin Sensors

You can use different sensor types together:

```cpp
#include <MinimalUltrasonic.h>

// 4-pin HC-SR04
MinimalUltrasonic hcsr04(12, 13);

// 3-pin Ping
MinimalUltrasonic ping(10);

// 3-pin Seeed
MinimalUltrasonic seeed(8);

void loop() {
  Serial.print("HC-SR04: "); Serial.println(hcsr04.read());
  delay(30);
  Serial.print("Ping: "); Serial.println(ping.read());
  delay(30);
  Serial.print("Seeed: "); Serial.println(seeed.read());
  delay(30);
  Serial.println();
  delay(500);
}
```

## Troubleshooting Multiple Sensors

### Problem: Erratic Readings

**Cause:** Sensors interfering with each other

**Solution:** Add delays between readings

```cpp
sensor1.read();
delay(50);  // Increase delay
sensor2.read();
```

### Problem: All Return 0

**Cause:** Insufficient power

**Solution:** Use external power supply

```cpp
// Power sensors from external 5V supply
// Connect Arduino GND to external GND
```

### Problem: Some Sensors Not Working

**Cause:** Pin conflicts or wiring

**Solution:** Check unique pins and connections

```cpp
// Verify each sensor independently
void testSensor(MinimalUltrasonic& sensor, const char* name) {
  Serial.print(name); Serial.print(": ");
  Serial.println(sensor.read());
}
```

## Best Practices

1. **Add Delays** - 30-50ms between sensor readings
2. **Separate Physically** - Mount sensors facing different directions
3. **Test Individually** - Verify each sensor works alone first
4. **Use Sequential Reading** - Read one at a time
5. **Stable Power** - Ensure adequate power supply
6. **Document Pin Usage** - Keep track of which pins are used

## Next Steps

- [Best Practices](/guide/best-practices) - General best practices
- [Troubleshooting](/guide/troubleshooting) - Solve common issues
- [Example](/examples/multiple-sensors) - Complete working example

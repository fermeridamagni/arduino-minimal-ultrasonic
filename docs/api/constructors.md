# Constructors

Documentation for `MinimalUltrasonic` constructors.

## Overview

MinimalUltrasonic provides two constructors to support different sensor types:

1. **4-pin constructor** for HC-SR04 and similar sensors
2. **3-pin constructor** for Ping))) and Seeed sensors

## 4-Pin Constructor

### Signature

```cpp
MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin)
```

### Description

Creates a sensor instance for 4-pin ultrasonic sensors with separate trigger and echo pins.

### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `trigPin` | `uint8_t` | Digital pin number for trigger signal |
| `echoPin` | `uint8_t` | Digital pin number for echo signal |

### Example

```cpp
#include <MinimalUltrasonic.h>

// Create sensor on pins 12 (trigger) and 13 (echo)
MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
}

void loop() {
    float distance = sensor.read();
    Serial.println(distance);
    delay(100);
}
```

### Wiring

```txt
HC-SR04    Arduino
-------    -------
VCC    ->  5V
GND    ->  GND
TRIG   ->  D12 (trigPin)
ECHO   ->  D13 (echoPin)
```

### Supported Sensors

- HC-SR04
- US-015
- US-100 (in GPIO mode)
- SRF05
- Other 4-pin ultrasonic sensors

## 3-Pin Constructor

### Signature

```cpp
MinimalUltrasonic(uint8_t pin)
```

### Description

Creates a sensor instance for 3-pin ultrasonic sensors that use a single pin for both trigger and echo.

### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pin` | `uint8_t` | Digital pin number for combined signal |

### Example

```cpp
#include <MinimalUltrasonic.h>

// Create sensor on pin 10 (single pin for both)
MinimalUltrasonic sensor(10);

void setup() {
    Serial.begin(9600);
}

void loop() {
    float distance = sensor.read();
    Serial.println(distance);
    delay(100);
}
```

### Wiring

```
Ping)))    Arduino
--------   -------
VCC    ->  5V
GND    ->  GND
SIG    ->  D10 (pin)
```

### Supported Sensors

- Parallax PING)))
- Seeed Studio Ultrasonic Ranger
- DFRobot URM37 (in GPIO mode)
- Other 3-pin ultrasonic sensors

## Constructor Delegation

Internally, the 3-pin constructor delegates to the 4-pin constructor:

```cpp
// From implementation
MinimalUltrasonic::MinimalUltrasonic(uint8_t pin)
    : MinimalUltrasonic(pin, pin) {
    _singlePin = true;
}
```

This design:

- Reduces code duplication
- Maintains consistent behavior
- Simplifies maintenance

## Initialization Details

### What Happens During Construction

1. Pin numbers are stored
2. Unit is set to `CM` (default)
3. Timeout is set to `20000µs` (default, ~3.4m range)
4. Single-pin mode is determined
5. **No** pin modes are set (done lazily on first read)

### Default Configuration

```cpp
MinimalUltrasonic sensor(12, 13);

// After construction:
// - Unit: CM
// - Timeout: 20000µs
// - Max range: ~340cm
// - Pin modes: Not yet configured
```

## Choosing the Right Constructor

### Use 4-Pin Constructor When

✅ You have an HC-SR04 sensor  
✅ Sensor has separate TRIG and ECHO pins  
✅ You want maximum flexibility in pin assignment  
✅ You have plenty of available digital pins  

### Use 3-Pin Constructor When

✅ You have a Parallax PING))) sensor  
✅ You have a Seeed Studio sensor  
✅ Sensor has a single SIG pin  
✅ You want to save a pin  

## Common Patterns

### Pattern 1: Multiple 4-Pin Sensors

```cpp
MinimalUltrasonic front(12, 13);
MinimalUltrasonic back(10, 11);
MinimalUltrasonic left(8, 9);
MinimalUltrasonic right(6, 7);

void loop() {
    Serial.print("F:"); Serial.print(front.read());
    Serial.print(" B:"); Serial.print(back.read());
    Serial.print(" L:"); Serial.print(left.read());
    Serial.print(" R:"); Serial.println(right.read());
    delay(200);
}
```

### Pattern 2: Mixed Sensor Types

```cpp
// 4-pin HC-SR04
MinimalUltrasonic hcsr04(12, 13);

// 3-pin Ping)))
MinimalUltrasonic ping(10);

void loop() {
    Serial.print("HC-SR04: "); Serial.println(hcsr04.read());
    Serial.print("Ping: "); Serial.println(ping.read());
    delay(200);
}
```

### Pattern 3: Array of Sensors

```cpp
const int SENSOR_COUNT = 4;
MinimalUltrasonic sensors[SENSOR_COUNT] = {
    MinimalUltrasonic(12, 13),
    MinimalUltrasonic(10, 11),
    MinimalUltrasonic(8, 9),
    MinimalUltrasonic(6, 7)
};

void loop() {
    for (int i = 0; i < SENSOR_COUNT; i++) {
        Serial.print("Sensor "); 
        Serial.print(i);
        Serial.print(": ");
        Serial.println(sensors[i].read());
        delay(50);
    }
}
```

### Pattern 4: Dynamic Allocation

```cpp
MinimalUltrasonic* sensor;

void setup() {
    Serial.begin(9600);
    
    // Choose sensor type at runtime
    bool use4Pin = true;  // Could be from config
    
    if (use4Pin) {
        sensor = new MinimalUltrasonic(12, 13);
    } else {
        sensor = new MinimalUltrasonic(10);
    }
}

void loop() {
    float distance = sensor->read();
    Serial.println(distance);
    delay(100);
}
```

## Pin Requirements

### 4-Pin Constructor

**Minimum:** 2 digital pins
**Recommendations:**

- Use any digital pins (D2-D13 on Uno)
- Trigger and echo can be on any pins
- Can use same trigger for multiple sensors (not recommended)
- Keep wires short (<30cm)

```cpp
// Valid pin combinations
MinimalUltrasonic s1(12, 13);  // Adjacent pins
MinimalUltrasonic s2(2, 8);    // Non-adjacent pins
MinimalUltrasonic s3(10, 10);  // Same pin (acts like 3-pin)
```

### 3-Pin Constructor

**Minimum:** 1 digital pin
**Recommendations:**

- Use any digital pin
- Avoid pins with special functions if possible
- Keep wire short (<30cm)

```cpp
// Valid pin assignments
MinimalUltrasonic s1(10);   // Standard digital pin
MinimalUltrasonic s2(2);    // Interrupt-capable pin (works but no advantage)
MinimalUltrasonic s3(A0);   // Analog pin used as digital
```

## Pin Conflicts

### What to Avoid

```cpp
// ❌ BAD: Same pins for different sensors
MinimalUltrasonic sensor1(12, 13);
MinimalUltrasonic sensor2(12, 13);  // CONFLICT!

// ❌ BAD: Overlapping pins
MinimalUltrasonic sensor1(12, 13);
MinimalUltrasonic sensor2(13, 14);  // Pin 13 shared!

// ✅ GOOD: Unique pins
MinimalUltrasonic sensor1(12, 13);
MinimalUltrasonic sensor2(10, 11);  // No conflicts
```

## Validation

The constructors do **not** validate pin numbers. Invalid pins will cause undefined behavior.

```cpp
// No validation performed:
MinimalUltrasonic sensor(99, 100);  // Invalid pins! Will not work

// Your responsibility:
const uint8_t TRIG_PIN = 12;
const uint8_t ECHO_PIN = 13;

// Verify pins are valid for your board
if (TRIG_PIN < NUM_DIGITAL_PINS && ECHO_PIN < NUM_DIGITAL_PINS) {
    MinimalUltrasonic sensor(TRIG_PIN, ECHO_PIN);
}
```

## Memory and Performance

### Memory Usage

```cpp
MinimalUltrasonic sensor1(12, 13);  // 8 bytes
MinimalUltrasonic sensor2(10);      // 8 bytes
```

Both constructors create objects of **exactly 8 bytes**.

### Construction Performance

- **Time:** O(1) - Constant time
- **Overhead:** Negligible (<1µs on AVR)
- **No I/O:** Pin modes not set until first read

## Troubleshooting

### Constructor Not Found

```cpp
// Error: no matching function for call to 'MinimalUltrasonic::MinimalUltrasonic()'
MinimalUltrasonic sensor;  // ❌ No default constructor

// Solution: Provide pins
MinimalUltrasonic sensor(12, 13);  // ✅ OK
```

### Wrong Sensor Type

```cpp
// Problem: Using 4-pin constructor for Ping))) sensor
MinimalUltrasonic ping(10, 11);  // Will not work correctly

// Solution: Use 3-pin constructor
MinimalUltrasonic ping(10);  // Correct
```

### Pin Conflicts

```cpp
// Problem: Serial uses pins 0 and 1
MinimalUltrasonic sensor(0, 1);  // Conflicts with Serial!

// Solution: Use different pins
MinimalUltrasonic sensor(12, 13);  // Safe pins
```

## Examples

### Example 1: Basic HC-SR04

```cpp
#include <MinimalUltrasonic.h>

// HC-SR04 on pins 12 and 13
MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    Serial.println("HC-SR04 Ready");
}

void loop() {
    float cm = sensor.read();
    
    if (cm > 0) {
        Serial.print(cm);
        Serial.println(" cm");
    }
    
    delay(100);
}
```

### Example 2: Basic Ping)))

```cpp
#include <MinimalUltrasonic.h>

// Ping))) on pin 10
MinimalUltrasonic sensor(10);

void setup() {
    Serial.begin(9600);
    Serial.println("Ping))) Ready");
}

void loop() {
    float cm = sensor.read();
    
    if (cm > 0) {
        Serial.print(cm);
        Serial.println(" cm");
    }
    
    delay(100);
}
```

### Example 3: Configuration After Construction

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    
    // Configure after construction
    sensor.setUnit(MinimalUltrasonic::INCHES);
    sensor.setTimeout(30000UL);
    
    Serial.println("Configured for inches, 5m range");
}

void loop() {
    float inches = sensor.read();
    
    if (inches > 0) {
        Serial.print(inches);
        Serial.println(" inches");
    }
    
    delay(100);
}
```

## See Also

- [MinimalUltrasonic Class](/api/class) - Class overview
- [Methods](/api/methods) - Available methods
- [Basic Usage](/guide/basic-usage) - Getting started
- [Examples](/examples/basic) - Complete examples

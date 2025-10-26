# API Reference

Complete reference for the MinimalUltrasonic library API.

## Overview

MinimalUltrasonic provides a simple, type-safe API for ultrasonic distance measurement. The library consists of one main class and supporting enums/constants.

## Quick Reference

```cpp
// Include
#include <MinimalUltrasonic.h>

// Constructors
MinimalUltrasonic(uint8_t sigPin)
MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut = 20000UL)

// Main Methods
float read(Unit unit = CM) const
void setTimeout(unsigned long timeOut)
void setMaxDistance(unsigned int distance)
void setUnit(Unit unit)

// Query Methods
unsigned long getTimeout() const
Unit getUnit() const

// Units Enum
enum Unit { CM, METERS, MM, INCHES, YARDS, MILES }
```

## Documentation Sections

<div class="api-grid">

### [MinimalUltrasonic Class](/api/class)

Core class for sensor management

- Object lifecycle
- Member variables
- Design decisions

### [Constructors](/api/constructors)

Creating sensor objects

- 3-pin sensors
- 4-pin sensors  
- Timeout configuration

### [Methods](/api/methods)

All available methods

- Reading distance
- Configuration
- Queries

### [Unit Enum](/api/units)

Measurement units

- Available units
- Conversion factors
- Usage examples

### [Constants](/api/constants)

Legacy compatibility

- CM constant
- INC constant
- Migration guide

</div>

## Type Reference

### Return Types

| Method | Return Type | Description |
|--------|-------------|-------------|
| `read()` | `float` | Distance in specified unit |
| `setTimeout()` | `void` | No return value |
| `setMaxDistance()` | `void` | No return value |
| `getTimeout()` | `unsigned long` | Timeout in microseconds |
| `setUnit()` | `void` | No return value |
| `getUnit()` | `Unit` | Current default unit |

### Parameter Types

| Parameter | Type | Description |
|-----------|------|-------------|
| `sigPin` | `uint8_t` | Pin number (0-255) |
| `trigPin` | `uint8_t` | Pin number (0-255) |
| `echoPin` | `uint8_t` | Pin number (0-255) |
| `timeOut` | `unsigned long` | Microseconds |
| `distance` | `unsigned int` | Centimeters |
| `unit` | `Unit` | Enum value |

## Usage Patterns

### Basic Pattern

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float distance = sensor.read();
  Serial.println(distance);
  delay(500);
}
```

### Advanced Pattern

```cpp
#include <MinimalUltrasonic.h>

class DistanceSensor {
private:
  MinimalUltrasonic sensor;
  float lastReading;
  
public:
  DistanceSensor(uint8_t trig, uint8_t echo) 
    : sensor(trig, echo), lastReading(0) {
    sensor.setUnit(MinimalUltrasonic::METERS);
    sensor.setTimeout(30000UL);
  }
  
  float getDistance() {
    float current = sensor.read();
    if (current > 0) {
      lastReading = current;
    }
    return lastReading;
  }
};
```

## Error Handling

### Return Value Checking

```cpp
float distance = sensor.read();

if (distance == 0) {
  // Error: timeout or no object
  handleError();
} else if (distance < 2) {
  // Warning: too close
  handleTooClose();
} else {
  // Valid reading
  processDistance(distance);
}
```

### Range Validation

```cpp
const float MIN_DIST = 2.0;
const float MAX_DIST = 400.0;

bool isValidDistance(float dist) {
  return (dist > MIN_DIST && dist < MAX_DIST);
}

float distance = sensor.read();
if (isValidDistance(distance)) {
  // Use distance
}
```

## Performance Characteristics

### Timing

| Operation | Time | Notes |
|-----------|------|-------|
| `read()` with object | ~30-200µs | Depends on distance |
| `read()` timeout | timeout value | No object detected |
| `setTimeout()` | <1µs | Instant |
| `setUnit()` | <1µs | Instant |

### Memory Usage

| Item | Size | Notes |
|------|------|-------|
| Class instance | 8 bytes | Per sensor |
| Method calls | 0 bytes | No heap allocation |
| String operations | Variable | User's responsibility |

## Thread Safety

MinimalUltrasonic is **not thread-safe**. Do not call methods from multiple threads or interrupts simultaneously.

For multi-threaded environments:

- Use separate sensor objects per thread
- Or implement mutex locking
- Avoid concurrent `read()` calls

## Compatibility Notes

### Arduino Compatibility

Works with standard Arduino API. Requires:

- `digitalWrite()`
- `digitalRead()`
- `pinMode()`
- `micros()`
- `delayMicroseconds()`

### Platform Support

Tested on:

- AVR (Uno, Mega, Nano)
- ESP32
- ESP8266
- ARM (Due, Zero)
- Teensy

## See Also

- **[Class Documentation](/api/class)** - Detailed class reference
- **[Examples](/examples/basic)** - Working code examples
- **[Guide](/guide/basic-usage)** - Usage tutorials

<style>
.api-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 1rem;
  margin: 2rem 0;
}

.api-grid > div {
  border: 1px solid var(--vp-c-divider);
  border-radius: 8px;
  padding: 1.5rem;
}

.api-grid h3 {
  margin-top: 0;
  border: none;
}

.api-grid ul {
  list-style: none;
  padding-left: 0;
  margin: 0.5rem 0 0 0;
}

.api-grid li {
  color: var(--vp-c-text-2);
  font-size: 0.9em;
}
</style>

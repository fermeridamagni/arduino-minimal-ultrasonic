# MinimalUltrasonic API Reference

Complete API documentation for the MinimalUltrasonic Arduino library v2.0.0.

## Table of Contents

- [Class: MinimalUltrasonic](#class-minimalultrasonic)
- [Enum: Unit](#enum-unit)
- [Constructors](#constructors)
- [Public Methods](#public-methods)
- [Constants](#constants)
- [Return Values](#return-values)
- [Code Examples](#code-examples)

---

## Class: MinimalUltrasonic

The main class for interfacing with ultrasonic distance sensors.

### Include

```cpp
#include <MinimalUltrasonic.h>
```

---

## Enum: Unit

Enumeration of available measurement units.

### Definition

```cpp
enum Unit : uint8_t {
    CM = 0,      // Centimeters (default)
    METERS = 1,  // Meters
    MM = 2,      // Millimeters
    INCHES = 3,  // Inches
    YARDS = 4,   // Yards
    MILES = 5    // Miles
};
```

### Usage

```cpp
float distance = sensor.read(MinimalUltrasonic::CM);
float meters = sensor.read(MinimalUltrasonic::METERS);
```

---

## Constructors

### MinimalUltrasonic(uint8_t sigPin)

Constructor for 3-pin ultrasonic sensors (Ping, Seeed SEN136B5B).

**Parameters:**

- `sigPin` - Digital pin number for signal (combined trigger/echo)

**Example:**

```cpp
MinimalUltrasonic sensor(13);  // 3-pin sensor on pin 13
```

**Use Case:**

- Ping))) ultrasonic sensor
- Seeed SEN136B5B sensor
- Any 3-pin ultrasonic sensor with combined signal pin

---

### MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut = 20000UL)

Constructor for 4-pin ultrasonic sensors (HC-SR04).

**Parameters:**

- `trigPin` - Digital pin number for trigger output
- `echoPin` - Digital pin number for echo input
- `timeOut` - *(optional)* Maximum wait time in microseconds (default: 20000µs)

**Example:**

```cpp
MinimalUltrasonic sensor(12, 13);           // Default timeout (20000µs)
MinimalUltrasonic sensor(12, 13, 40000UL);  // Custom timeout (40000µs)
```

**Use Case:**

- HC-SR04 ultrasonic sensor
- HC-SR04+ sensor
- Any 4-pin ultrasonic sensor with separate trigger and echo pins

---

## Public Methods

### read()

Read distance from the sensor in specified unit.

**Signature:**

```cpp
float read(Unit unit = CM) const
```

**Parameters:**

- `unit` - *(optional)* Unit of measurement (default: CM)

**Returns:**

- `float` - Distance in specified unit
- `0` - If timeout occurred or no object detected

**Example:**

```cpp
float distCm = sensor.read();                          // Centimeters (default)
float distM = sensor.read(MinimalUltrasonic::METERS);  // Meters
float distIn = sensor.read(MinimalUltrasonic::INCHES); // Inches
float distMm = sensor.read(MinimalUltrasonic::MM);     // Millimeters
float distYd = sensor.read(MinimalUltrasonic::YARDS);  // Yards
float distMi = sensor.read(MinimalUltrasonic::MILES);  // Miles
```

**Notes:**

- Returns `0` if no echo received within timeout
- Minimum detectable distance: ~2 cm
- Maximum distance depends on timeout setting

---

### setTimeout()

Set the timeout for echo response.

**Signature:**

```cpp
void setTimeout(unsigned long timeOut)
```

**Parameters:**

- `timeOut` - Maximum wait time in microseconds

**Returns:** None

**Example:**

```cpp
sensor.setTimeout(30000UL);  // Set to 30ms (~5.1m range)
sensor.setTimeout(40000UL);  // Set to 40ms (~6.8m range)
```

**Timeout Guidelines:**

| Timeout (µs) | Max Range |
|--------------|-----------|
| 10,000 | ~1.7m |
| 20,000 | ~3.4m |
| 30,000 | ~5.1m |
| 40,000 | ~6.8m |

---

### setMaxDistance()

Set maximum detection distance (automatically calculates timeout).

**Signature:**

```cpp
void setMaxDistance(unsigned int distance)
```

**Parameters:**

- `distance` - Maximum distance in centimeters

**Returns:** None

**Example:**

```cpp
sensor.setMaxDistance(300);  // 3 meters max range
sensor.setMaxDistance(500);  // 5 meters max range
```

**Notes:**

- This is a convenience method that calculates timeout from distance
- Equivalent to: `setTimeout(distance * 2 * 29.1)`

---

### getTimeout()

Get the current timeout value.

**Signature:**

```cpp
unsigned long getTimeout() const
```

**Parameters:** None

**Returns:**

- `unsigned long` - Current timeout in microseconds

**Example:**

```cpp
unsigned long timeout = sensor.getTimeout();
Serial.print("Current timeout: ");
Serial.print(timeout);
Serial.println(" microseconds");
```

---

### setUnit()

Set the default unit of measurement for future read() calls.

**Signature:**

```cpp
void setUnit(Unit unit)
```

**Parameters:**

- `unit` - Default unit to use

**Returns:** None

**Example:**

```cpp
sensor.setUnit(MinimalUltrasonic::METERS);
float dist = sensor.read();  // Now returns meters by default

sensor.setUnit(MinimalUltrasonic::INCHES);
float dist2 = sensor.read(); // Now returns inches by default
```

---

### getUnit()

Get the current default unit of measurement.

**Signature:**

```cpp
Unit getUnit() const
```

**Parameters:** None

**Returns:**

- `Unit` - Current default unit

**Example:**

```cpp
MinimalUltrasonic::Unit currentUnit = sensor.getUnit();

if (currentUnit == MinimalUltrasonic::CM) {
    Serial.println("Using centimeters");
} else if (currentUnit == MinimalUltrasonic::METERS) {
    Serial.println("Using meters");
}
```

---

## Constants

### Legacy Compatibility Constants

For backward compatibility with v1.x:

```cpp
#define CM MinimalUltrasonic::CM
#define INC MinimalUltrasonic::INCHES
```

**Example (v1.x style - still works):**

```cpp
float cm = sensor.read(CM);
float inches = sensor.read(INC);
```

**Example (v2.x style - preferred):**

```cpp
float cm = sensor.read(MinimalUltrasonic::CM);
float inches = sensor.read(MinimalUltrasonic::INCHES);
```

---

## Return Values

### Distance Measurements

All distance measurements return `float` type:

**Valid Range:**

- Minimum: ~2 cm (sensor dependent)
- Maximum: Determined by timeout setting

**Special Values:**

- `0` - Timeout occurred, no echo received, or no object detected
- `< 2` - Object may be too close, reading unreliable

**Error Checking:**

```cpp
float distance = sensor.read();

if (distance == 0) {
    Serial.println("Error: No object detected or timeout");
} else if (distance < 2) {
    Serial.println("Warning: Object too close, reading may be unreliable");
} else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}
```

---

## Code Examples

### Example 1: Basic Usage

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
}

void loop() {
    float distance = sensor.read();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(1000);
}
```

### Example 2: Multiple Units

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println("Distance in different units:");
    Serial.print("  CM: "); 
    Serial.println(sensor.read(MinimalUltrasonic::CM));
    
    Serial.print("  Meters: "); 
    Serial.println(sensor.read(MinimalUltrasonic::METERS), 4);
    
    Serial.print("  Inches: "); 
    Serial.println(sensor.read(MinimalUltrasonic::INCHES), 2);
    
    Serial.println();
    delay(2000);
}
```

### Example 3: Custom Timeout

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13, 40000UL);  // 6.8m max range

void setup() {
    Serial.begin(9600);
    
    // Or set timeout after construction:
    // sensor.setTimeout(40000UL);
    
    // Or set by distance:
    // sensor.setMaxDistance(680);  // 6.8m in cm
    
    Serial.print("Timeout: ");
    Serial.print(sensor.getTimeout());
    Serial.println(" microseconds");
}

void loop() {
    float distance = sensor.read();
    
    if (distance == 0) {
        Serial.println("No object detected (timeout)");
    } else {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
    }
    
    delay(1000);
}
```

### Example 4: Multiple Sensors

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic front(12, 13);
MinimalUltrasonic left(10, 11);
MinimalUltrasonic right(8, 9);

void setup() {
    Serial.begin(9600);
    
    // Set different default units for each
    front.setUnit(MinimalUltrasonic::CM);
    left.setUnit(MinimalUltrasonic::INCHES);
    right.setUnit(MinimalUltrasonic::METERS);
}

void loop() {
    Serial.print("Front: ");
    Serial.print(front.read());
    Serial.println(" cm");
    
    Serial.print("Left: ");
    Serial.print(left.read());
    Serial.println(" inches");
    
    Serial.print("Right: ");
    Serial.print(right.read(), 3);
    Serial.println(" meters");
    
    Serial.println();
    delay(1000);
}
```

### Example 5: Error Handling

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    sensor.setTimeout(30000UL);
}

void loop() {
    float distance = sensor.read();
    
    // Comprehensive error checking
    if (distance == 0) {
        Serial.println("ERROR: No echo received");
        Serial.println("  - Check wiring");
        Serial.println("  - Verify power supply");
        Serial.println("  - Ensure object in range");
    } else if (distance < 2) {
        Serial.println("WARNING: Object too close");
        Serial.print("  Distance: ");
        Serial.print(distance);
        Serial.println(" cm (unreliable)");
    } else if (distance > 400) {
        Serial.println("WARNING: Distance suspicious");
        Serial.print("  Distance: ");
        Serial.print(distance);
        Serial.println(" cm (may be echo bounce)");
    } else {
        // Valid reading
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
    }
    
    delay(1000);
}
```

### Example 6: Dynamic Unit Selection

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    Serial.println("Commands:");
    Serial.println("  'c' - Centimeters");
    Serial.println("  'm' - Meters");
    Serial.println("  'i' - Inches");
}

void loop() {
    // Check for serial input
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        
        switch(cmd) {
            case 'c':
                sensor.setUnit(MinimalUltrasonic::CM);
                Serial.println("Unit set to: Centimeters");
                break;
            case 'm':
                sensor.setUnit(MinimalUltrasonic::METERS);
                Serial.println("Unit set to: Meters");
                break;
            case 'i':
                sensor.setUnit(MinimalUltrasonic::INCHES);
                Serial.println("Unit set to: Inches");
                break;
        }
    }
    
    float distance = sensor.read();
    Serial.print("Distance: ");
    Serial.print(distance);
    
    // Show current unit
    MinimalUltrasonic::Unit unit = sensor.getUnit();
    if (unit == MinimalUltrasonic::CM) Serial.println(" cm");
    else if (unit == MinimalUltrasonic::METERS) Serial.println(" m");
    else if (unit == MinimalUltrasonic::INCHES) Serial.println(" in");
    
    delay(500);
}
```

---

## Type Definitions

### Summary

| Type | Description |
|------|-------------|
| `MinimalUltrasonic` | Main sensor class |
| `MinimalUltrasonic::Unit` | Enum for measurement units |
| `uint8_t` | 8-bit unsigned integer (pin numbers) |
| `unsigned long` | Timeout values (microseconds) |
| `float` | Distance measurements (all units) |

---

## Version Information

- **API Version**: 2.0.0
- **Release Date**: October 25, 2025
- **Compatibility**: Arduino IDE 1.6.0+
- **Author**: fermeridamagni (Magni Development)

---

## See Also

- [README.md](README.md) - Quick start guide
- [TECHNICAL.md](TECHNICAL.md) - Technical documentation
- [CHANGELOG.md](CHANGELOG.md) - Version history
- [Examples](examples/) - Working code examples

---

**Last Updated**: October 25, 2025  
**Document Version**: 2.0.0

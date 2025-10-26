# MinimalUltrasonic Class

Complete reference for the `MinimalUltrasonic` class.

## Class Overview

```cpp
class MinimalUltrasonic {
public:
    enum Unit {
        CM,
        METERS,
        MM,
        INCHES,
        YARDS,
        MILES
    };

    // Constructors
    MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin);
    MinimalUltrasonic(uint8_t pin);

    // Main methods
    float read();
    
    // Configuration
    void setUnit(Unit unit);
    Unit getUnit() const;
    void setTimeout(unsigned long timeout);
    void setMaxDistance(float maxDistance);
    unsigned long getTimeout() const;

private:
    uint8_t _trigPin;
    uint8_t _echoPin;
    Unit _unit;
    unsigned long _timeout;
    bool _singlePin;
    
    unsigned long timing();
    float convertUnit(float cm);
};
```

## Class Members

### Public Members

#### Unit Enumx

Enumeration for measurement units:

| Enum Value | Description | Conversion Factor |
|------------|-------------|-------------------|
| `CM` | Centimeters (default) | 1.0 |
| `METERS` | Meters | 0.01 |
| `MM` | Millimeters | 10.0 |
| `INCHES` | Inches | 0.393701 |
| `YARDS` | Yards | 0.0109361 |
| `MILES` | Miles | 6.21371e-6 |

See [Unit Enum](/api/units) for details.

#### Constructors

**4-Pin Constructor (HC-SR04)**

```cpp
MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin)
```

**3-Pin Constructor (Ping))), Seeed)**

```cpp
MinimalUltrasonic(uint8_t pin)
```

See [Constructors](/api/constructors) for details.

#### Methods

**Reading Methods**

- `float read()` - Get distance measurement
- `unsigned long timing()` - Get raw microsecond timing

**Configuration Methods**

- `void setUnit(Unit unit)` - Set measurement unit
- `Unit getUnit() const` - Get current unit
- `void setTimeout(unsigned long timeout)` - Set timeout in microseconds
- `void setMaxDistance(float maxDistance)` - Set max distance in centimeters
- `unsigned long getTimeout() const` - Get current timeout

See [Methods](/api/methods) for details.

### Private Members

#### Private Attributes

```cpp
uint8_t _trigPin;           // Trigger pin number
uint8_t _echoPin;           // Echo pin number
Unit _unit;                 // Current measurement unit
unsigned long _timeout;     // Timeout in microseconds
bool _singlePin;            // True for 3-pin sensors
```

#### Private Methods

```cpp
unsigned long timing();     // Get raw pulse duration
float convertUnit(float cm); // Convert from cm to current unit
```

## Constants

The class uses the following physical constants:

```cpp
// Speed of sound at 20°C
const float SOUND_SPEED = 343.0;  // meters/second

// Time divisor for CM calculation
// (Speed in cm/µs × 2 for round trip)
const float TIME_DIVISOR_CM = 58.8235;

// Default timeout (20000µs ≈ 3.4m range)
const unsigned long DEFAULT_TIMEOUT = 20000UL;
```

See [Constants](/api/constants) for details.

## Usage Example

### Complete Class Usage

```cpp
#include <MinimalUltrasonic.h>

// Create sensor instance
MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    
    // Configure sensor
    sensor.setUnit(MinimalUltrasonic::CM);
    sensor.setTimeout(20000UL);
    
    Serial.println("Sensor initialized");
}

void loop() {
    // Read distance
    float distance = sensor.read();
    
    // Check for valid reading
    if (distance > 0) {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.print(" ");
        
        // Print unit
        switch (sensor.getUnit()) {
            case MinimalUltrasonic::CM:
                Serial.println("cm");
                break;
            case MinimalUltrasonic::METERS:
                Serial.println("m");
                break;
            case MinimalUltrasonic::MM:
                Serial.println("mm");
                break;
            case MinimalUltrasonic::INCHES:
                Serial.println("in");
                break;
            case MinimalUltrasonic::YARDS:
                Serial.println("yd");
                break;
            case MinimalUltrasonic::MILES:
                Serial.println("mi");
                break;
        }
    } else {
        Serial.println("Error: No reading");
    }
    
    delay(100);
}
```

## Class Design

### Design Principles

1. **Minimal Memory Footprint** - Only 8 bytes per instance
2. **Simple Interface** - Easy to use, hard to misuse
3. **Efficient** - Optimized for embedded systems
4. **Flexible** - Support multiple sensor types and units

### Memory Layout

```cpp
sizeof(MinimalUltrasonic) = 8 bytes

Memory breakdown:
- _trigPin:    1 byte (uint8_t)
- _echoPin:    1 byte (uint8_t)
- _unit:       1 byte (enum stored as uint8_t)
- _timeout:    4 bytes (unsigned long)
- _singlePin:  1 byte (bool)
Total:         8 bytes
```

### Performance Characteristics

| Operation | Time Complexity | Notes |
|-----------|----------------|-------|
| Constructor | O(1) | Constant time initialization |
| read() | O(1) | Depends on timeout, max 40ms |
| setUnit() | O(1) | Simple assignment |
| setTimeout() | O(1) | Simple assignment |
| getUnit() | O(1) | Simple return |
| getTimeout() | O(1) | Simple return |

## Thread Safety

The class is **not thread-safe**. Each instance should only be accessed from a single execution context.

For multiple sensors, use separate instances:

```cpp
MinimalUltrasonic sensor1(12, 13);
MinimalUltrasonic sensor2(10, 11);

// Each can be used independently
void loop() {
    float d1 = sensor1.read();
    delay(50);
    float d2 = sensor2.read();
}
```

## Inheritance

The class is **not designed for inheritance**. It's a final, concrete implementation.

Do not inherit from `MinimalUltrasonic`:

```cpp
// ❌ Not recommended
class MySensor : public MinimalUltrasonic {
    // ...
};

// ✅ Use composition instead
class MySensor {
    MinimalUltrasonic sensor;
public:
    MySensor(uint8_t trig, uint8_t echo) 
        : sensor(trig, echo) {}
};
```

## Comparison with Other Libraries

### NewPing Library

```cpp
// NewPing: 20-40 bytes per instance
NewPing sonar(12, 13, 400);
unsigned int distance = sonar.ping_cm();

// MinimalUltrasonic: 8 bytes per instance
MinimalUltrasonic sensor(12, 13);
float distance = sensor.read();
```

### Ultrasonic Library

```cpp
// Ultrasonic: No unit support
Ultrasonic sensor(12, 13);
long distance = sensor.read();  // Always CM

// MinimalUltrasonic: Multiple units
MinimalUltrasonic sensor(12, 13);
sensor.setUnit(MinimalUltrasonic::METERS);
float distance = sensor.read();  // In meters
```

## Best Practices

### 1. Const Correctness

```cpp
// Use const for read-only methods
const MinimalUltrasonic sensor(12, 13);
Unit unit = sensor.getUnit();      // ✓ OK
unsigned long timeout = sensor.getTimeout();  // ✓ OK
// sensor.setUnit(MinimalUltrasonic::METERS);  // ✗ Error (const)
```

### 2. Error Handling

```cpp
void loop() {
    float distance = sensor.read();
    
    // Always check for errors
    if (distance == 0) {
        handleError();
        return;
    }
    
    processDistance(distance);
}
```

### 3. Configuration in Setup

```cpp
void setup() {
    // Configure once
    sensor.setUnit(MinimalUltrasonic::CM);
    sensor.setTimeout(20000UL);
}

void loop() {
    // Just read
    float distance = sensor.read();
}
```

## Platform Compatibility

The class is compatible with:

- ✅ Arduino Uno / Nano (ATmega328P)
- ✅ Arduino Mega (ATmega2560)
- ✅ Arduino Due (SAM3X8E)
- ✅ ESP8266
- ✅ ESP32
- ✅ STM32 (with Arduino core)
- ✅ Teensy (2.0, 3.x, 4.x)

See [Compatibility](/technical/compatibility) for details.

## Related Pages

- [Constructors](/api/constructors) - Constructor details
- [Methods](/api/methods) - Method documentation
- [Unit Enum](/api/units) - Unit enumeration
- [Constants](/api/constants) - Physical constants
- [Architecture](/technical/architecture) - Internal design

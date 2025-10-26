# Methods

Complete reference for all `MinimalUltrasonic` methods.

## Overview

MinimalUltrasonic provides a simple yet powerful API with the following methods:

**Reading Methods:**

- [`read()`](#read) - Get distance measurement in current unit
- [`timing()`](#timing) - Get raw microsecond timing (advanced)

**Configuration Methods:**

- [`setUnit()`](#setunit) - Set measurement unit
- [`getUnit()`](#getunit) - Get current unit
- [`setTimeout()`](#settimeout) - Set timeout in microseconds
- [`setMaxDistance()`](#setmaxdistance) - Set maximum distance in centimeters
- [`getTimeout()`](#gettimeout) - Get current timeout value

## Reading Methods

### read()

Get distance measurement in the currently configured unit.

#### Signature

```cpp
float read()
```

#### Parameters

None

#### Return Value

| Type | Description |
|------|-------------|
| `float` | Distance in current unit, or `0` if no valid reading |

#### Description

Performs a complete distance measurement cycle:

1. Sends 10µs trigger pulse
2. Waits for echo pulse
3. Measures pulse duration
4. Converts to current unit
5. Returns result

Returns `0` if:

- No echo received within timeout
- Object is too close (< 2cm)
- Object is beyond range
- Measurement error occurred

#### Example

```cpp
MinimalUltrasonic sensor(12, 13);

void loop() {
    float distance = sensor.read();
    
    if (distance > 0) {
        Serial.print("Distance: ");
        Serial.println(distance);
    } else {
        Serial.println("No reading");
    }
    
    delay(100);
}
```

#### Performance

- **Typical execution time:** 20-40ms (depends on distance and timeout)
- **Minimum delay between calls:** 60ms recommended
- **Blocking:** Yes, waits for echo or timeout

#### See Also

- [Error Handling](/guide/error-handling) - Handle invalid readings
- [Basic Usage](/guide/basic-usage) - Usage examples

---

### timing()

Get raw echo pulse duration in microseconds (advanced use).

#### Signature

```cpp
unsigned long timing()
```

#### Parameters

None

#### Return Value

| Type | Description |
|------|-------------|
| `unsigned long` | Echo pulse duration in microseconds, or `0` if timeout |

#### Description

Low-level method that returns the raw pulse duration. Useful for:

- Custom distance calculations
- Debugging
- Temperature compensation
- Non-standard calculations

Most users should use `read()` instead.

#### Example

```cpp
MinimalUltrasonic sensor(12, 13);

void loop() {
    unsigned long microseconds = sensor.timing();
    
    if (microseconds > 0) {
        // Manual calculation
        float cm = microseconds / 58.8235;
        Serial.print("Distance: ");
        Serial.print(cm);
        Serial.println(" cm");
    }
    
    delay(100);
}
```

#### Calculation

Convert microseconds to distance:

```cpp
// Formula: distance = (time × speed_of_sound) / 2
// Where speed_of_sound = 343 m/s = 0.0343 cm/µs

float cm = microseconds / 58.8235;
float inches = microseconds / 148.0;
float meters = microseconds / 5882.35;
```

#### Performance

- **Same as read()** but without unit conversion
- **Returns raw timing** - marginally faster than read()

---

## Configuration Methods

### setUnit()

Set the measurement unit for `read()` method.

#### Signature

```cpp
void setUnit(Unit unit)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `unit` | `Unit` | Desired measurement unit (enum value) |

#### Return Value

None (void)

#### Description

Changes the unit for subsequent `read()` calls. Does not affect `timing()`.

Available units:

- `MinimalUltrasonic::CM` - Centimeters (default)
- `MinimalUltrasonic::METERS` - Meters
- `MinimalUltrasonic::MM` - Millimeters
- `MinimalUltrasonic::INCHES` - Inches
- `MinimalUltrasonic::YARDS` - Yards
- `MinimalUltrasonic::MILES` - Miles

#### Example

```cpp
MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    
    // Set to meters
    sensor.setUnit(MinimalUltrasonic::METERS);
}

void loop() {
    float distance = sensor.read();  // Now in meters
    Serial.print(distance);
    Serial.println(" m");
    delay(100);
}
```

#### Dynamic Unit Switching

```cpp
void loop() {
    // Switch units dynamically
    sensor.setUnit(MinimalUltrasonic::CM);
    float cm = sensor.read();
    
    sensor.setUnit(MinimalUltrasonic::INCHES);
    float inches = sensor.read();
    
    Serial.print(cm);
    Serial.print(" cm = ");
    Serial.print(inches);
    Serial.println(" inches");
    
    delay(500);
}
```

#### Performance

- **Time:** O(1) - instant
- **No overhead:** Unit conversion happens in `read()`

#### See Also

- [Multiple Units Guide](/guide/multiple-units) - Working with units
- [Unit Enum](/api/units) - Unit details

---

### getUnit()

Get the currently configured measurement unit.

#### Signature

```cpp
Unit getUnit() const
```

#### Parameters

None

#### Return Value

| Type | Description |
|------|-------------|
| `Unit` | Current measurement unit (enum value) |

#### Description

Returns the current unit setting. Useful for:

- Displaying current unit
- Conditional logic
- Debugging
- Status reporting

#### Example

```cpp
MinimalUltrasonic sensor(12, 13);

void printWithUnit() {
    float distance = sensor.read();
    Serial.print(distance);
    Serial.print(" ");
    
    Unit currentUnit = sensor.getUnit();
    switch (currentUnit) {
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
}
```

#### Performance

- **Time:** O(1) - instant
- **Const method:** Can be called on const objects

---

### setTimeout()

Set the maximum time to wait for echo pulse.

#### Signature

```cpp
void setTimeout(unsigned long timeout)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `timeout` | `unsigned long` | Maximum wait time in microseconds |

#### Return Value

None (void)

#### Description

Sets how long to wait for an echo before giving up. Determines the maximum detectable range.

**Default:** 20000µs (~3.4m range)

**Range calculation:**

```
max_distance_cm = (timeout × 343) / (2 × 10000)
max_distance_cm = timeout / 58.8235
```

#### Example

```cpp
MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    
    // For 1 meter range (fast readings)
    sensor.setTimeout(6000UL);
    
    // For 5 meter range (slower readings)
    // sensor.setTimeout(30000UL);
}
```

#### Common Timeout Values

| Timeout (µs) | Max Range (cm) | Max Range (m) | Use Case |
|-------------|----------------|---------------|----------|
| 3000 | ~51 | 0.5 | Very short range, fast response |
| 6000 | ~102 | 1.0 | Short range detection |
| 12000 | ~204 | 2.0 | Medium range |
| 20000 | ~340 | 3.4 | **Default** - general use |
| 30000 | ~510 | 5.1 | Long range |
| 40000 | ~680 | 6.8 | Maximum practical range |

#### Performance Impact

- **Shorter timeout:** Faster error detection, but limited range
- **Longer timeout:** Greater range, but slower when no object

#### See Also

- [Timeout Configuration](/guide/timeout-configuration) - Detailed guide
- [Optimization](/guide/optimization) - Performance tuning

---

### setMaxDistance()

Set maximum detection distance in centimeters (convenience method).

#### Signature

```cpp
void setMaxDistance(float maxDistance)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `maxDistance` | `float` | Maximum distance in centimeters |

#### Return Value

None (void)

#### Description

Convenience method that calculates and sets the appropriate timeout for a desired maximum range in centimeters.

Internally converts to timeout:

```cpp
timeout = maxDistance × 58.8235
```

#### Example

```cpp
MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    
    // Set max range to 200cm
    sensor.setMaxDistance(200.0);
    
    // Equivalent to:
    // sensor.setTimeout(200.0 * 58.8235);  // 11764µs
}
```

#### Comparison with setTimeout()

```cpp
// These are equivalent:
sensor.setMaxDistance(100.0);  // 100cm
sensor.setTimeout(5882UL);      // 5882µs

// setMaxDistance is more intuitive
// setTimeout gives finer control
```

#### Range Limits

```cpp
// Practical limits:
sensor.setMaxDistance(2.0);    // Minimum: 2cm
sensor.setMaxDistance(400.0);  // Maximum: 4m (HC-SR04 limit)

// Beyond sensor limits:
sensor.setMaxDistance(1000.0); // May not work reliably
```

---

### getTimeout()

Get the current timeout value in microseconds.

#### Signature

```cpp
unsigned long getTimeout() const
```

#### Parameters

None

#### Return Value

| Type | Description |
|------|-------------|
| `unsigned long` | Current timeout in microseconds |

#### Description

Returns the currently configured timeout value. Useful for:

- Checking current configuration
- Calculating maximum range
- Debugging
- Logging

#### Example

```cpp
MinimalUltrasonic sensor(12, 13);

void printConfiguration() {
    unsigned long timeout = sensor.getTimeout();
    float maxRange = timeout / 58.8235;
    
    Serial.print("Timeout: ");
    Serial.print(timeout);
    Serial.println(" µs");
    
    Serial.print("Max range: ~");
    Serial.print(maxRange);
    Serial.println(" cm");
}
```

#### Calculate Max Range

```cpp
void loop() {
    unsigned long timeout = sensor.getTimeout();
    
    // Calculate theoretical max range
    float maxRangeCm = timeout / 58.8235;
    float maxRangeMeters = timeout / 5882.35;
    
    Serial.print("Max range: ");
    Serial.print(maxRangeCm);
    Serial.print(" cm (");
    Serial.print(maxRangeMeters);
    Serial.println(" m)");
    
    delay(5000);
}
```

#### Performance

- **Time:** O(1) - instant
- **Const method:** Can be called on const objects

---

## Method Chaining

Methods that return `void` can be used sequentially:

```cpp
MinimalUltrasonic sensor(12, 13);

void setup() {
    // Sequential configuration
    sensor.setUnit(MinimalUltrasonic::CM);
    sensor.setTimeout(20000UL);
}
```

Not chainable (methods return void):

```cpp
// This does NOT work:
// sensor.setUnit(MinimalUltrasonic::CM).setTimeout(20000UL);  // ❌
```

## Performance Summary

| Method | Time | Notes |
|--------|------|-------|
| `read()` | 20-40ms | Depends on distance/timeout |
| `timing()` | 20-40ms | Same as read() |
| `setUnit()` | <1µs | Instant |
| `getUnit()` | <1µs | Instant |
| `setTimeout()` | <1µs | Instant |
| `setMaxDistance()` | <1µs | Calculation + assignment |
| `getTimeout()` | <1µs | Instant |

## Thread Safety

Methods are **not thread-safe**. Do not call from interrupts or multiple threads simultaneously.

## Const Correctness

These methods are `const` and can be called on const objects:

- `getUnit()`
- `getTimeout()`

These methods modify state and cannot be called on const objects:

- `read()`
- `timing()`
- `setUnit()`
- `setTimeout()`
- `setMaxDistance()`

## See Also

- [MinimalUltrasonic Class](/api/class) - Class overview
- [Constructors](/api/constructors) - Creating instances
- [Examples](/examples/basic) - Usage examples
- [Basic Usage Guide](/guide/basic-usage) - Getting started

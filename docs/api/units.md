# Unit Enum

Documentation for the `Unit` enumeration in MinimalUltrasonic.

## Overview

The `Unit` enum defines all supported measurement units for distance readings.

```cpp
enum Unit {
    CM,
    METERS,
    MM,
    INCHES,
    YARDS,
    MILES
};
```

## Enum Values

### CM

**Centimeters** - Default unit, most commonly used.

**Properties:**

- Symbol: `cm`
- Conversion factor: `1.0` (base unit)
- Precision: 2 decimal places typically
- Range: 2-400 cm (HC-SR04 typical)

**Example:**

```cpp
sensor.setUnit(MinimalUltrasonic::CM);
float distance = sensor.read();  // e.g., 45.23 cm
```

**Best for:**

- General purpose measurements
- Short to medium distances
- Maximum precision
- Most Arduino projects

---

### METERS

**Meters** - SI unit for longer distances.

**Properties:**

- Symbol: `m`
- Conversion factor: `0.01`
- Precision: 4 decimal places typically
- Range: 0.02-4 m (HC-SR04 typical)

**Example:**

```cpp
sensor.setUnit(MinimalUltrasonic::METERS);
float distance = sensor.read();  // e.g., 0.4523 m
```

**Best for:**

- Long range measurements
- Scientific applications
- SI unit compatibility
- International projects

---

### MM

**Millimeters** - High precision unit.

**Properties:**

- Symbol: `mm`
- Conversion factor: `10.0`
- Precision: 1 decimal place typically
- Range: 20-4000 mm (HC-SR04 typical)

**Example:**

```cpp
sensor.setUnit(MinimalUltrasonic::MM);
float distance = sensor.read();  // e.g., 452.3 mm
```

**Best for:**

- High precision measurements
- Small distances
- Engineering applications
- Fine positioning

---

### INCHES

**Inches** - Imperial unit commonly used in US.

**Properties:**

- Symbol: `in` or `"`
- Conversion factor: `0.393701`
- Precision: 2 decimal places typically
- Range: 0.79-157 inches (HC-SR04 typical)

**Example:**

```cpp
sensor.setUnit(MinimalUltrasonic::INCHES);
float distance = sensor.read();  // e.g., 17.81 inches
```

**Best for:**

- US market products
- Imperial measurements
- Consumer electronics
- Regional compatibility

---

### YARDS

**Yards** - Imperial unit for medium distances.

**Properties:**

- Symbol: `yd`
- Conversion factor: `0.0109361`
- Precision: 4 decimal places typically
- Range: 0.0219-4.37 yards (HC-SR04 typical)

**Example:**

```cpp
sensor.setUnit(MinimalUltrasonic::YARDS);
float distance = sensor.read();  // e.g., 0.4945 yards
```

**Best for:**

- Sports applications
- US field measurements
- Medium range distances
- Specific regional needs

---

### MILES

**Miles** - Imperial unit for very long distances.

**Properties:**

- Symbol: `mi`
- Conversion factor: `6.21371e-6`
- Precision: Many decimal places
- Range: 0.0000124-0.00248 miles (HC-SR04 typical)

**Example:**

```cpp
sensor.setUnit(MinimalUltrasonic::MILES);
float distance = sensor.read();  // e.g., 0.000281 miles
```

**Best for:**

- Extremely long range (theoretical)
- Demonstration purposes
- **Not recommended** for typical ultrasonic ranges

---

## Conversion Table

### From Centimeters

| Unit | Formula | Example (100 cm) |
|------|---------|------------------|
| CM | value × 1.0 | 100.00 cm |
| METERS | value × 0.01 | 1.00 m |
| MM | value × 10.0 | 1000.0 mm |
| INCHES | value × 0.393701 | 39.37 inches |
| YARDS | value × 0.0109361 | 1.0936 yards |
| MILES | value × 0.0000062137 | 0.00062137 miles |

### Common Distance Equivalents

| CM | Meters | MM | Inches | Yards | Miles |
|----|--------|-----|--------|-------|-------|
| 1 | 0.01 | 10 | 0.39 | 0.011 | 6.21×10⁻⁶ |
| 10 | 0.1 | 100 | 3.94 | 0.109 | 6.21×10⁻⁵ |
| 50 | 0.5 | 500 | 19.69 | 0.547 | 3.11×10⁻⁴ |
| 100 | 1.0 | 1000 | 39.37 | 1.094 | 6.21×10⁻⁴ |
| 200 | 2.0 | 2000 | 78.74 | 2.187 | 1.24×10⁻³ |

## Usage Patterns

### Switching Units Dynamically

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void loop() {
    // Take one measurement
    float rawDistance = sensor.read();
    
    // Display in multiple units
    sensor.setUnit(MinimalUltrasonic::CM);
    Serial.print(sensor.read());
    Serial.print(" cm = ");
    
    sensor.setUnit(MinimalUltrasonic::INCHES);
    Serial.print(sensor.read());
    Serial.print(" in = ");
    
    sensor.setUnit(MinimalUltrasonic::METERS);
    Serial.print(sensor.read());
    Serial.println(" m");
    
    delay(1000);
}
```

### Unit Selection Based on Range

```cpp
void autoSelectUnit(MinimalUltrasonic& sensor) {
    // Get distance in CM first
    sensor.setUnit(MinimalUltrasonic::CM);
    float cm = sensor.read();
    
    // Select appropriate unit based on distance
    if (cm < 10) {
        sensor.setUnit(MinimalUltrasonic::MM);
        Serial.print(sensor.read());
        Serial.println(" mm");
    } else if (cm < 100) {
        sensor.setUnit(MinimalUltrasonic::CM);
        Serial.print(cm);
        Serial.println(" cm");
    } else {
        sensor.setUnit(MinimalUltrasonic::METERS);
        Serial.print(sensor.read());
        Serial.println(" m");
    }
}
```

### User-Configurable Units

```cpp
MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    Serial.println("Select unit:");
    Serial.println("1 - CM");
    Serial.println("2 - METERS");
    Serial.println("3 - INCHES");
    
    while (!Serial.available());
    int choice = Serial.read() - '0';
    
    switch (choice) {
        case 1:
            sensor.setUnit(MinimalUltrasonic::CM);
            break;
        case 2:
            sensor.setUnit(MinimalUltrasonic::METERS);
            break;
        case 3:
            sensor.setUnit(MinimalUltrasonic::INCHES);
            break;
    }
}
```

## Unit String Conversion

Helper function to get unit string:

```cpp
const char* getUnitString(MinimalUltrasonic::Unit unit) {
    switch (unit) {
        case MinimalUltrasonic::CM:
            return "cm";
        case MinimalUltrasonic::METERS:
            return "m";
        case MinimalUltrasonic::MM:
            return "mm";
        case MinimalUltrasonic::INCHES:
            return "in";
        case MinimalUltrasonic::YARDS:
            return "yd";
        case MinimalUltrasonic::MILES:
            return "mi";
        default:
            return "?";
    }
}

void loop() {
    float distance = sensor.read();
    Serial.print(distance);
    Serial.print(" ");
    Serial.println(getUnitString(sensor.getUnit()));
    delay(100);
}
```

## Implementation Details

### Conversion Factor Storage

Conversion factors are stored as constants in the implementation:

```cpp
// From implementation
float MinimalUltrasonic::convertUnit(float cm) {
    switch (_unit) {
        case CM: return cm;
        case METERS: return cm * 0.01;
        case MM: return cm * 10.0;
        case INCHES: return cm * 0.393701;
        case YARDS: return cm * 0.0109361;
        case MILES: return cm * 0.000006213712;
        default: return cm;
    }
}
```

### Performance

- **Storage:** 1 byte per sensor instance
- **Conversion time:** <1µs on AVR
- **Memory overhead:** None (inline conversion)

### Precision Considerations

Floating-point precision limits:

```cpp
// CM: ~0.01 cm precision (float)
float cm = 123.45;  // ✓ Accurate

// MILES: Limited precision for small values
float miles = 0.0001234567;  // May lose precision
```

## Best Practices

### 1. Choose Appropriate Unit

```cpp
// ✓ GOOD: CM for typical ranges
sensor.setUnit(MinimalUltrasonic::CM);  // 2-400 cm

// ✓ GOOD: METERS for longer ranges
sensor.setUnit(MinimalUltrasonic::METERS);  // 0.02-4 m

// ⚠️ QUESTIONABLE: MILES for ultrasonic
sensor.setUnit(MinimalUltrasonic::MILES);  // Very small numbers
```

### 2. Set Unit Once

```cpp
// ✓ GOOD: Set in setup
void setup() {
    sensor.setUnit(MinimalUltrasonic::CM);
}

void loop() {
    float distance = sensor.read();  // Always in CM
}

// ⚠️ OK but slower: Change each loop
void loop() {
    sensor.setUnit(MinimalUltrasonic::CM);
    float distance = sensor.read();  // Slight overhead
}
```

### 3. Document Units

```cpp
// ✓ GOOD: Clear what unit is used
sensor.setUnit(MinimalUltrasonic::CM);
float distanceCm = sensor.read();  // Variable name indicates unit

// ⚠️ UNCLEAR: What unit?
float distance = sensor.read();  // Ambiguous
```

## Unit Selection Guide

| Distance Range | Recommended Unit | Alternative |
|----------------|------------------|-------------|
| 0-10 cm | MM | CM |
| 10-100 cm | CM | INCHES |
| 100-400 cm | CM or METERS | INCHES |
| > 400 cm | METERS | - |

| Application | Recommended Unit |
|-------------|------------------|
| Robot navigation | CM |
| Parking sensor | CM or INCHES |
| Distance measurement | METERS |
| Level monitoring | CM or MM |
| Gesture control | CM or MM |
| Security perimeter | METERS |

## See Also

- [Multiple Units Guide](/guide/multiple-units) - Working with different units
- [Methods](/api/methods) - setUnit() and getUnit()
- [Technical Conversions](/technical/conversions) - Conversion mathematics
- [All Units Example](/examples/all-units) - Complete example

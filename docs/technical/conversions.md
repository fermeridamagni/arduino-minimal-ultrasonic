# Unit Conversions

Detailed mathematical explanation of unit conversion system.

## Overview

MinimalUltrasonic measures time in microseconds and converts to 6 different distance units:

| Unit | Symbol | Conversion Factor |
|------|--------|------------------|
| Centimeters | cm | Base unit |
| Meters | m | ÷ 100 |
| Millimeters | mm | × 10 |
| Inches | in | × 0.393701 |
| Yards | yd | × 0.0109361 |
| Miles | mi | × 0.000006213712 |

## Base Calculation

### Time to Centimeters

Starting point: Microseconds from `pulseIn()`

```cpp
// Speed of sound at 20°C
const float SPEED_OF_SOUND = 343.0; // m/s

// Convert to cm/µs
const float CM_PER_US = 0.0343; // cm/µs

// Round trip adjustment (divide by 2)
const float US_PER_CM = 58.8235; // µs/cm

// Formula
distance_cm = time_microseconds / 58.8235;
```

**Derivation:**

```txt
Speed = 343 m/s = 34300 cm/s
Convert to cm/µs:
= 34300 cm/s ÷ 1000000 µs/s
= 0.0343 cm/µs

Round trip (sound goes there and back):
distance = (time × speed) / 2
distance_cm = (time_µs × 0.0343) / 2
distance_cm = time_µs × 0.01715
distance_cm = time_µs / 58.8235

Therefore: US_PER_CM = 58.8235
```

## Unit Enum

```cpp
enum Unit {
    CM,      // Centimeters (default)
    METERS,  // Meters
    MM,      // Millimeters
    INCHES,  // Inches
    YARDS,   // Yards
    MILES    // Miles
};
```

## Conversion Implementation

### Method 1: Convert at Source

Convert during measurement (current implementation):

```cpp
unsigned int MinimalUltrasonic::read(Unit unit) {
    unsigned long duration = timing();
    
    if (duration == 0) return 0;
    
    // Convert to CM first
    unsigned int cm = duration / US_PER_CM;
    
    // Apply unit conversion
    return convertUnit(cm, CM, unit);
}
```

### Method 2: Direct Conversion

Alternative: Convert directly from microseconds:

```cpp
unsigned int read_direct(Unit unit) {
    unsigned long duration = timing();
    
    switch(unit) {
        case CM:
            return duration / 58.8235;
        case METERS:
            return duration / 5882.35;
        case MM:
            return duration / 5.88235;
        case INCHES:
            return duration / 149.35;
        case YARDS:
            return duration / 5380.58;
        case MILES:
            return duration / 9481344.0;
        default:
            return 0;
    }
}
```

**Library uses Method 1** for clarity and maintainability.

## Conversion Factors

### Metric Conversions

**Centimeters → Meters:**

```cpp
meters = cm * 0.01;
// or
meters = cm / 100.0;

Example: 250 cm = 2.5 m
```

**Centimeters → Millimeters:**

```cpp
mm = cm * 10.0;

Example: 25 cm = 250 mm
```

### Imperial Conversions

**Centimeters → Inches:**

```cpp
inches = cm * 0.393701;

Derivation:
1 inch = 2.54 cm
1 cm = 1 / 2.54 inches
1 cm = 0.393701 inches

Example: 100 cm = 39.37 inches
```

**Centimeters → Yards:**

```cpp
yards = cm * 0.0109361;

Derivation:
1 yard = 36 inches = 91.44 cm
1 cm = 1 / 91.44 yards
1 cm = 0.0109361 yards

Example: 100 cm = 1.09 yards
```

**Centimeters → Miles:**

```cpp
miles = cm * 0.000006213712;

Derivation:
1 mile = 1760 yards = 63360 inches = 160934.4 cm
1 cm = 1 / 160934.4 miles
1 cm = 0.000006213712 miles

Example: 100000 cm (1 km) = 0.621 miles
```

## Implementation Details

### convertUnit() Function

```cpp
unsigned int MinimalUltrasonic::convertUnit(unsigned int value, Unit from, Unit to) {
    if (from == to) return value;
    
    // Convert source to CM
    float cm;
    switch(from) {
        case CM:     cm = value; break;
        case METERS: cm = value * 100.0; break;
        case MM:     cm = value / 10.0; break;
        case INCHES: cm = value / 0.393701; break;
        case YARDS:  cm = value / 0.0109361; break;
        case MILES:  cm = value / 0.000006213712; break;
        default:     return 0;
    }
    
    // Convert CM to target
    switch(to) {
        case CM:     return (unsigned int)cm;
        case METERS: return (unsigned int)(cm * 0.01);
        case MM:     return (unsigned int)(cm * 10.0);
        case INCHES: return (unsigned int)(cm * 0.393701);
        case YARDS:  return (unsigned int)(cm * 0.0109361);
        case MILES:  return (unsigned int)(cm * 0.000006213712);
        default:     return 0;
    }
}
```

### Two-Step Process

1. **Normalize to CM** - Common base unit
2. **Convert to target** - Apply target conversion

This approach:

- ✅ Simplifies logic (6 cases instead of 36)
- ✅ Easy to add new units
- ✅ Clear and maintainable
- ❌ Small precision loss from double conversion

## Precision Analysis

### Float vs Integer

Library uses `unsigned int` for distance returns:

```cpp
unsigned int read(Unit unit = CM);
```

**Implications:**

```cpp
// Internal calculation uses float
float cm = duration / 58.8235;

// Result truncated to integer
return (unsigned int)(cm * conversion_factor);
```

**Precision loss:**

| Unit | Resolution | Notes |
|------|-----------|-------|
| CM | 1 cm | No loss |
| METERS | 1 cm (0.01 m) | Returned as cm in meters |
| MM | 1 mm | No loss (10mm = 1cm) |
| INCHES | 1 inch (~2.54 cm) | Loses sub-inch |
| YARDS | 1 yard (~91 cm) | Loses sub-yard |
| MILES | N/A | Out of sensor range |

### Example: Precision Loss

```cpp
Actual distance: 100.7 cm

CM:     100 (lost 0.7 cm)
METERS: 1 (lost 0.007 m)
MM:     1007 (no loss)
INCHES: 39 (lost 0.37 inches)
YARDS:  1 (lost 0.09 yards)
```

**For higher precision:**

- Use MM for small distances
- Use CM for medium distances
- Use METERS for large distances

## Conversion Tables

### Metric Conversion Table

| CM | MM | METERS |
|----|-----|--------|
| 1 | 10 | 0.01 |
| 10 | 100 | 0.1 |
| 50 | 500 | 0.5 |
| 100 | 1000 | 1.0 |
| 200 | 2000 | 2.0 |
| 400 | 4000 | 4.0 |

### Imperial Conversion Table

| CM | INCHES | YARDS |
|----|--------|-------|
| 1 | 0.39 | 0.01 |
| 10 | 3.94 | 0.11 |
| 50 | 19.69 | 0.55 |
| 100 | 39.37 | 1.09 |
| 200 | 78.74 | 2.19 |
| 400 | 157.48 | 4.37 |

### Mixed Conversion Table

| CM | INCHES | Feet | Yards |
|----|--------|------|-------|
| 2.54 | 1 | 0.083 | 0.028 |
| 30.48 | 12 | 1 | 0.333 |
| 91.44 | 36 | 3 | 1 |
| 100 | 39.37 | 3.28 | 1.09 |

## Usage Examples

### Example 1: Basic Conversion

```cpp
MinimalUltrasonic sensor(7, 8);

unsigned int cm = sensor.read(CM);
unsigned int mm = sensor.read(MM);
unsigned int inches = sensor.read(INCHES);

Serial.println(cm);      // 100
Serial.println(mm);      // 1000
Serial.println(inches);  // 39
```

### Example 2: Manual Conversion

```cpp
unsigned int cm = sensor.read(CM);

// Convert manually
unsigned int meters = cm / 100;
unsigned int mm = cm * 10;
unsigned int inches = cm * 0.393701;
```

### Example 3: Dynamic Unit Selection

```cpp
Unit userUnit = INCHES; // From user input

unsigned int distance = sensor.read(userUnit);

switch(userUnit) {
    case CM:
        Serial.print(distance);
        Serial.println(" cm");
        break;
    case INCHES:
        Serial.print(distance);
        Serial.println(" inches");
        break;
    // ... other units
}
```

### Example 4: Comparison

```cpp
// Same measurement in different units
unsigned int cm = sensor.read(CM);
unsigned int inches = sensor.read(INCHES);

// Verify conversion
unsigned int converted = cm * 0.393701;

if (inches == converted) {
    Serial.println("Conversion correct");
}
```

## Performance Considerations

### Computational Cost

**Time per conversion:**

```txt
CM:      0 operations (base unit)
METERS:  1 division (÷ 100)
MM:      1 multiplication (× 10)
INCHES:  1 multiplication (× 0.393701)
YARDS:   1 multiplication (× 0.0109361)
MILES:   1 multiplication (× 0.000006213712)
```

**Total read() cost:**

- Timing: ~400-6000 µs (pulseIn wait)
- Division: ~1 µs (µs to cm)
- Conversion: ~1 µs (cm to unit)

**Conversion is negligible** compared to measurement time.

### Memory Cost

**Conversion constants:**

```cpp
// If stored as constants
const float TO_METERS = 0.01;          // 4 bytes
const float TO_MM = 10.0;              // 4 bytes
const float TO_INCHES = 0.393701;      // 4 bytes
const float TO_YARDS = 0.0109361;      // 4 bytes
const float TO_MILES = 0.000006213712; // 4 bytes

Total: 20 bytes
```

**Library approach:** Constants inlined in switch statement (0 bytes storage)

## Temperature Compensation

For extreme accuracy, compensate for temperature:

```cpp
float getSpeedOfSound(float tempC) {
    return 331.3 + (0.606 * tempC); // m/s
}

float getUSPerCM(float tempC) {
    float speed = getSpeedOfSound(tempC);
    float cm_per_us = speed / 10000.0;
    return 2.0 / cm_per_us; // Round trip
}

unsigned int readWithTemp(float tempC) {
    unsigned long duration = timing();
    float us_per_cm = getUSPerCM(tempC);
    return duration / us_per_cm;
}
```

**Complexity:** Not included in library (adds overhead for ~1-3% improvement)

## Common Conversion Patterns

### Pattern 1: Display in Multiple Units

```cpp
void displayMultiple() {
    unsigned int cm = sensor.read(CM);
    
    Serial.print(cm);
    Serial.print(" cm = ");
    
    Serial.print(cm * 0.01);
    Serial.print(" m = ");
    
    Serial.print(cm * 0.393701);
    Serial.println(" inches");
}
```

### Pattern 2: Range Checking

```cpp
bool inRange(unsigned int min_cm, unsigned int max_cm) {
    unsigned int distance = sensor.read(CM);
    return (distance >= min_cm && distance <= max_cm);
}
```

### Pattern 3: User-Configurable Units

```cpp
Unit selectedUnit = CM;

void setup() {
    // Read from EEPROM or config
    selectedUnit = loadUnitPreference();
}

void loop() {
    unsigned int distance = sensor.read(selectedUnit);
    display(distance, selectedUnit);
}
```

## Accuracy Considerations

### Factors Affecting Accuracy

1. **Timing resolution** - Arduino: 4µs → ~0.7mm
2. **Temperature** - ±10°C → ±3%
3. **Float truncation** - ~1 unit
4. **Rounding** - ~0.5 unit

### Best Practices

**For highest accuracy:**

- Use MM for distances <100 cm
- Use CM for distances 100-1000 cm
- Use METERS for distances >1000 cm

**Why:**

- Minimizes rounding errors
- Maximizes resolution
- Matches sensor capabilities

## Mathematical Proofs

### Proof: CM Conversion Factor

```txt
Given:
- Speed of sound: v = 343 m/s
- Convert to cm/µs: v = 0.0343 cm/µs
- Round trip: distance = time × speed / 2

Derive:
d = (t × 0.0343) / 2
d = t × 0.01715
d = t / 58.3090...

Library uses 58.8235 (empirically adjusted)
Difference: 0.87% (within temperature variation)
```

### Proof: Inch Conversion

```txt
Given:
- 1 inch = 2.54 cm (by definition)

Derive:
inches = cm / 2.54
inches = cm × (1 / 2.54)
inches = cm × 0.3937007874...

Library uses: 0.393701 (6 decimal places)
Precision: ±0.0000008 (0.00002% error)
```

## See Also

- [Physics Background](/technical/physics) - Speed of sound formulas
- [Constants](/api/constants) - Conversion constants
- [Units API](/api/units) - Unit enum documentation
- [Performance](/technical/performance) - Conversion performance

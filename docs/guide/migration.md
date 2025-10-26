# Migration Guide

Upgrading from MinimalUltrasonic v1.x to v2.0.

## Quick Migration Checklist

- [ ] Update include statement (recommended)
- [ ] Change distance variables to `float` type
- [ ] Update unit constants to enum style (optional)
- [ ] Test all functionality
- [ ] Update documentation/comments

## Step-by-Step Migration

### 1. Update Include Statement

**Recommended** but not required:

```cpp
// Old
#include <Ultrasonic.h>

// New (recommended)
#include <MinimalUltrasonic.h>
```

::: tip Backward Compatible
Both includes work in v2.0, but `<MinimalUltrasonic.h>` is preferred for consistency with the class name.
:::

### 2. Update Variable Types

If you explicitly typed your distance variables:

```cpp
// Old
unsigned int distance = sensor.read();

// New
float distance = sensor.read();
```

If you didn't specify types, no change needed:

```cpp
// This works in both versions
auto distance = sensor.read();
```

### 3. Update Unit Constants (Optional)

Old style still works, but new style is preferred:

```cpp
// Old (still works via macros)
float cm = sensor.read(CM);
float inches = sensor.read(INC);

// New (type-safe enums)
float cm = sensor.read(MinimalUltrasonic::CM);
float inches = sensor.read(MinimalUltrasonic::INCHES);
```

## Breaking Changes

### Return Type Changed

`read()` now returns `float` instead of `unsigned int`:

```cpp
// v1.x
unsigned int distance = sensor.read();
int whole = distance;  // No decimal places

// v2.0
float distance = sensor.read();
int whole = (int)distance;  // Cast if needed
```

**Impact**: More precision, especially for non-cm units.

### Include Name (Recommended)

While both work, the recommended include changed:

```cpp
// v1.x
#include <Ultrasonic.h>

// v2.0 recommended
#include <MinimalUltrasonic.h>
```

**Impact**: Better consistency with class name.

## New Features to Adopt

### 1. Multiple Units

Take advantage of 4 new units:

```cpp
// New in v2.0
float meters = sensor.read(MinimalUltrasonic::METERS);
float mm = sensor.read(MinimalUltrasonic::MM);
float yards = sensor.read(MinimalUltrasonic::YARDS);
float miles = sensor.read(MinimalUltrasonic::MILES);
```

### 2. Default Unit Configuration

Set a default unit for cleaner code:

```cpp
// Set default to meters
sensor.setUnit(MinimalUltrasonic::METERS);

// Now all reads return meters
float dist1 = sensor.read();  // meters
float dist2 = sensor.read();  // meters

// Still can override
float cm = sensor.read(MinimalUltrasonic::CM);
```

### 3. Query Methods

New methods to query state:

```cpp
// Check current timeout
unsigned long timeout = sensor.getTimeout();

// Check current default unit
MinimalUltrasonic::Unit unit = sensor.getUnit();
```

## Migration Examples

### Before (v1.x)

```cpp
#include <Ultrasonic.h>

Ultrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
}

void loop() {
  int distance = sensor.read();
  Serial.println(distance);
  delay(1000);
}
```

### After (v2.0) - Minimal Changes

```cpp
#include <MinimalUltrasonic.h>  // Updated include

MinimalUltrasonic sensor(12, 13);  // Updated class name

void setup() {
  Serial.begin(9600);
}

void loop() {
  float distance = sensor.read();  // Changed to float
  Serial.println(distance);
  delay(1000);
}
```

### After (v2.0) - With New Features

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  // Use new features
  sensor.setUnit(MinimalUltrasonic::METERS);
  sensor.setTimeout(30000UL);
}

void loop() {
  float distance = sensor.read();  // Returns meters
  
  if (distance > 0) {
    Serial.print(distance, 3);  // 3 decimal places
    Serial.println(" m");
  }
  
  delay(1000);
}
```

## Common Migration Issues

### Issue: Implicit Conversions

**Problem**: Passing float to function expecting int

```cpp
void processDist(int dist) { /* ... */ }

float distance = sensor.read();
processDist(distance);  // Warning: implicit conversion
```

**Solution**: Explicit cast

```cpp
processDist((int)distance);  // OK
```

### Issue: Format Specifiers

**Problem**: Old printf-style formatting

```cpp
// v1.x
unsigned int dist = sensor.read();
sprintf(buffer, "%u cm", dist);

// v2.0
float dist = sensor.read();
sprintf(buffer, "%u cm", dist);  // Wrong!
```

**Solution**: Use correct format

```cpp
sprintf(buffer, "%.2f cm", dist);  // Correct
// Or use Serial.print which handles float
Serial.print(dist);
```

### Issue: Comparison Precision

**Problem**: Exact equality with floats

```cpp
float dist = sensor.read();
if (dist == 15.0) {  // May not work as expected
  // ...
}
```

**Solution**: Use range comparison

```cpp
const float EPSILON = 0.1;
if (abs(dist - 15.0) < EPSILON) {
  // ...
}
```

## Testing Your Migration

### Basic Test

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  Serial.println("Migration Test");
  
  // Test new methods exist
  sensor.setTimeout(30000UL);
  sensor.setUnit(MinimalUltrasonic::CM);
  
  unsigned long timeout = sensor.getTimeout();
  MinimalUltrasonic::Unit unit = sensor.getUnit();
  
  Serial.println("All new methods available!");
}

void loop() {
  float distance = sensor.read();
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(1000);
}
```

### Unit Test

```cpp
void testUnits() {
  MinimalUltrasonic sensor(12, 13);
  
  // Test all units work
  float cm = sensor.read(MinimalUltrasonic::CM);
  float m = sensor.read(MinimalUltrasonic::METERS);
  float mm = sensor.read(MinimalUltrasonic::MM);
  float in = sensor.read(MinimalUltrasonic::INCHES);
  float yd = sensor.read(MinimalUltrasonic::YARDS);
  float mi = sensor.read(MinimalUltrasonic::MILES);
  
  Serial.println("All units working!");
}
```

## Rollback Plan

If you need to roll back to v1.x:

1. Reinstall v1.x from Library Manager
2. Change `float` back to `unsigned int`
3. Revert include if changed
4. Remove any v2.0-specific code

## Getting Help

- **Documentation**: [Getting Started Guide](/guide/getting-started)
- **Issues**: [GitHub Issues](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/issues)
- **Discussions**: [GitHub Discussions](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/discussions)

## Summary

Most code requires minimal changes:

- Update include (recommended)
- Change `unsigned int` to `float`
- Optionally adopt new features

The library maintains backward compatibility for smooth migration!

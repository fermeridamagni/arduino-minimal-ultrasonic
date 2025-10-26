# Changelog

All notable changes to the MinimalUltrasonic library are documented here.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.0.0] - 2025-10-25

### 🎉 Added

- **Multiple Unit Support** - Added 4 new measurement units:
  - `METERS` - Measurements in meters
  - `MM` - Measurements in millimeters  
  - `YARDS` - Measurements in yards
  - `MILES` - Measurements in miles
  - Existing `CM` (centimeters) and `INCHES` (backward compatible with `INC`)

- **Unit Enum** - Introduced `MinimalUltrasonic::Unit` enum for type-safe unit selection

- **New Methods**:
  - `setUnit(Unit unit)` - Set default measurement unit
  - `getUnit()` - Get current default unit
  - `getTimeout()` - Retrieve current timeout value

- **Comprehensive Documentation**:
  - Doxygen-style inline documentation
  - Complete API reference
  - Best practices guide
  - Troubleshooting section
  - VitePress documentation site

- **New Example** - `AllUnits.ino` demonstrating all 6 measurement units

- **Better Error Handling** - `read()` returns `0` on timeout/error

### 🔄 Changed

- **Breaking**: `read()` method now returns `float` instead of `unsigned int`
- **Breaking**: Recommended include changed to `<MinimalUltrasonic.h>`
- **Improved Accuracy** - Uses precise speed of sound constant (29.1 µs/cm)
- **Code Structure** - Refactored for better readability:
  - Private member variables use `_` prefix
  - Const correctness applied throughout
  - Removed unnecessary 30ms delay in timing
  - More efficient timeout checking
- **Constructor** - Improved delegation for 3-pin sensors
- **Examples** - Updated with better documentation

### 🐛 Fixed

- Removed unnecessary 30ms delay (improved speed)
- Fixed timing inaccuracies in echo detection
- Improved timeout handling

### ⚠️ Deprecated

- `INC` constant (use `MinimalUltrasonic::INCHES`)
- `<Ultrasonic.h>` include (use `<MinimalUltrasonic.h>`)

### ✅ Backward Compatibility

- Legacy `CM` and `INC` constants still work
- Default behavior unchanged (returns cm)
- Constructor signatures unchanged

## [1.0.0] - Previous Release

### Features

- Basic ultrasonic distance measurement
- Support for HC-SR04, Ping))), and Seeed sensors
- Centimeter and inch measurements
- Configurable timeout
- Multiple sensor support

---

## 🔄 Migration Guide v1.x → v2.0

Migrating from v1.x is simple. Here's what to change:

### Step 1: Update Include

```cpp
// Old
#include <Ultrasonic.h>

// New (recommended)
#include <MinimalUltrasonic.h>
```

::: tip
Both includes work, but `MinimalUltrasonic.h` is preferred for consistency.
:::

### Step 2: Update Variable Types

If you explicitly typed variables:

```cpp
// Old
unsigned int distance = sensor.read();

// New
float distance = sensor.read();
```

::: info
If you didn't specify type (`auto` or no type), no change needed!
:::

### Step 3: Update Units (Optional)

Old style still works, but new style is preferred:

```cpp
// Old (still works)
float inches = sensor.read(INC);

// New (preferred)
float inches = sensor.read(MinimalUltrasonic::INCHES);
```

### That's It!

Your existing code should now work. Test thoroughly!

---

## 🚀 New Features in v2.0

Take advantage of new capabilities:

### Multiple Units

```cpp
// All these work now!
float cm = sensor.read(MinimalUltrasonic::CM);
float m = sensor.read(MinimalUltrasonic::METERS);
float mm = sensor.read(MinimalUltrasonic::MM);
float in = sensor.read(MinimalUltrasonic::INCHES);
float yd = sensor.read(MinimalUltrasonic::YARDS);
float mi = sensor.read(MinimalUltrasonic::MILES);
```

### Default Units

```cpp
// Set a default unit
sensor.setUnit(MinimalUltrasonic::METERS);

// Now read() returns meters
float distance = sensor.read();

// Check current unit
MinimalUltrasonic::Unit unit = sensor.getUnit();
```

### Query Timeout

```cpp
// Get current timeout
unsigned long timeout = sensor.getTimeout();
Serial.print("Timeout: ");
Serial.print(timeout);
Serial.println(" µs");
```

---

## 📊 Version Comparison

| Feature | v1.0 | v2.0 |
|---------|------|------|
| Units | 2 (cm, in) | 6 (cm, m, mm, in, yd, mi) |
| Return Type | `unsigned int` | `float` |
| Documentation | Basic | Comprehensive |
| Examples | 3 | 4 |
| API Methods | 4 | 7 |
| Type Safety | Defines | Enums |
| Memory/Instance | 8 bytes | 8 bytes |
| Code Size | ~1.3 KB | ~1.4 KB |

---

## 🔗 Links

- [Full Documentation](/guide/getting-started)
- [API Reference](/api/reference)
- [Examples](/examples/basic)
- [GitHub Repository](https://github.com/fermeridamagni/arduino-minimal-ultrasonic)
- [Report Issues](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/issues)

---

**Questions?** Check the [Migration Guide](/guide/migration) or open a [Discussion](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/discussions).

# Changelog

All notable changes to the MinimalUltrasonic library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.0.0] - 2025-10-25

### Added

- **New Unit Support**: Added support for multiple units of measurement:
  - `METERS` - Measurements in meters
  - `MM` - Measurements in millimeters
  - `YARDS` - Measurements in yards
  - `MILES` - Measurements in miles
  - Existing `CM` (centimeters) and `INCHES` (backward compatible with `INC`)
- **Unit Enum**: Introduced `MinimalUltrasonic::Unit` enum for type-safe unit selection
- **New Methods**:
  - `setUnit(Unit unit)` - Set default measurement unit
  - `getUnit()` - Get current default unit
  - `getTimeout()` - Retrieve current timeout value
- **Improved Documentation**:
  - Comprehensive Doxygen-style inline documentation
  - Detailed API reference in README
  - Best practices guide
  - Troubleshooting section
- **New Example**: `AllUnits.ino` demonstrating all available measurement units
- **Better Error Handling**: `read()` now returns `0` on timeout/error (float type)

### Changed

- **Breaking**: `read()` method now returns `float` instead of `unsigned int` for better precision
- **Breaking**: Include changed from `<Ultrasonic.h>` to `<MinimalUltrasonic.h>`
- **Improved Accuracy**: Updated timing calculation using more precise speed of sound constant (29.1 µs/cm)
- **Code Structure**: Refactored for better readability and maintainability
  - Private member variables now use `_` prefix convention
  - Const correctness applied throughout
  - Removed unnecessary `delay(30)` in timing routine
  - More efficient timeout checking
- **Constructor**: Improved constructor delegation for 3-pin sensors
- **Examples**: Updated all examples with better documentation and modern practices

### Fixed

- Removed unnecessary 30ms delay in `timing()` method, improving measurement speed
- Fixed potential timing inaccuracies in echo pulse detection
- Improved timeout handling to prevent hangs

### Deprecated

- Legacy `INC` constant still works but `MinimalUltrasonic::INCHES` is preferred
- Old include `<Ultrasonic.h>` - use `<MinimalUltrasonic.h>` instead

### Backward Compatibility

- Legacy `CM` and `INC` constants still work via macros
- Existing code using `read()` without parameters continues to work (returns cm)
- Constructor signatures remain the same

## [1.0.0] - Previous Release

### Features

- Basic ultrasonic distance measurement
- Support for HC-SR04, Ping))), and Seeed sensors
- Centimeter and inch measurements
- Configurable timeout
- Multiple sensor support

---

## Migration Guide v1.x to v2.0

### Update Include Statement

```cpp
// Old
#include <Ultrasonic.h>

// New
#include <MinimalUltrasonic.h>
```

### Update Variable Type (if explicitly typed)

```cpp
// Old
unsigned int distance = ultrasonic.read();

// New
float distance = ultrasonic.read();
```

### Update Unit Constants (optional, old still works)

```cpp
// Old (still works)
float inches = ultrasonic.read(INC);

// New (preferred)
float inches = ultrasonic.read(MinimalUltrasonic::INCHES);
```

### Enjoy New Features

```cpp
// New units
float meters = ultrasonic.read(MinimalUltrasonic::METERS);
float mm = ultrasonic.read(MinimalUltrasonic::MM);
float yards = ultrasonic.read(MinimalUltrasonic::YARDS);

// Set default unit
ultrasonic.setUnit(MinimalUltrasonic::METERS);
float dist = ultrasonic.read();  // Returns meters now

// Check timeout value
unsigned long timeout = ultrasonic.getTimeout();
```

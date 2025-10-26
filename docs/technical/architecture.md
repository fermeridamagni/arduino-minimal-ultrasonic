# Architecture

Internal design and architecture of the MinimalUltrasonic library.

## Design Philosophy

MinimalUltrasonic is built on three core principles:

1. **Minimalism** - Only essential features, no bloat
2. **Efficiency** - Optimized for resource-constrained microcontrollers
3. **Simplicity** - Easy to use, hard to misuse

## Class Structure

### Single Class Design

```cpp
class MinimalUltrasonic {
public:
    // Public interface
    enum Unit { CM, METERS, MM, INCHES, YARDS, MILES };
    
    // Constructors
    MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin);
    MinimalUltrasonic(uint8_t pin);
    
    // Public methods
    float read();
    void setUnit(Unit unit);
    Unit getUnit() const;
    void setTimeout(unsigned long timeout);
    void setMaxDistance(float maxDistance);
    unsigned long getTimeout() const;
    
private:
    // Internal state
    uint8_t _trigPin;
    uint8_t _echoPin;
    Unit _unit;
    unsigned long _timeout;
    bool _singlePin;
    
    // Internal methods
    unsigned long timing();
    float convertUnit(float cm);
};
```

**Design Choice:** Single class instead of inheritance hierarchy

- **Benefit:** Smaller code size, no virtual function overhead
- **Trade-off:** Less extensible, but not needed for this use case

## Memory Layout

### Instance Size: 8 Bytes

```cpp
MinimalUltrasonic sensor(12, 13);
// sizeof(sensor) = 8 bytes

Memory breakdown:
┌─────────────┬──────┬─────────────────────┐
│ Member      │ Size │ Offset              │
├─────────────┼──────┼─────────────────────┤
│ _trigPin    │ 1B   │ 0                   │
│ _echoPin    │ 1B   │ 1                   │
│ _unit       │ 1B   │ 2                   │
│ (padding)   │ 1B   │ 3 (alignment)       │
│ _timeout    │ 4B   │ 4-7                 │
│ _singlePin  │ 1B   │ Packed with padding │
└─────────────┴──────┴─────────────────────┘
Total: 8 bytes (may vary by compiler/platform)
```

### Comparison with Other Libraries

| Library | Instance Size | Overhead |
|---------|--------------|----------|
| MinimalUltrasonic | 8 bytes | Minimal |
| NewPing | 20-40 bytes | High |
| Ultrasonic | 16-24 bytes | Medium |

## Code Architecture

### Layered Design

```txt
┌─────────────────────────────────┐
│  Public API                     │
│  (read, setUnit, setTimeout)    │
├─────────────────────────────────┤
│  Unit Conversion Layer          │
│  (convertUnit)                  │
├─────────────────────────────────┤
│  Timing Layer                   │
│  (timing)                       │
├─────────────────────────────────┤
│  Hardware Abstraction           │
│  (pinMode, digitalWrite, etc)   │
└─────────────────────────────────┘
```

### Method Flow

#### read() Method Flow

```txt
read()
  │
  ├─> timing()
  │     │
  │     ├─> pinMode()          // Set pin modes
  │     ├─> digitalWrite()     // Send trigger
  │     ├─> delayMicroseconds() // Wait 10µs
  │     ├─> digitalWrite()     // End trigger
  │     └─> pulseIn()          // Measure echo
  │
  ├─> Check if timeout (0)
  │
  ├─> Convert µs to cm
  │
  └─> convertUnit()
        │
        └─> Apply conversion factor
```

## Constructor Delegation

Efficient code reuse through constructor delegation:

```cpp
// 3-pin constructor delegates to 4-pin constructor
MinimalUltrasonic::MinimalUltrasonic(uint8_t pin)
    : MinimalUltrasonic(pin, pin) {  // Delegate
    _singlePin = true;
}

// 4-pin constructor (primary)
MinimalUltrasonic::MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin)
    : _trigPin(trigPin), _echoPin(echoPin),
      _unit(CM), _timeout(20000UL), _singlePin(false) {
}
```

**Benefits:**

- Reduces code duplication
- Single initialization path
- Easier maintenance
- Smaller binary size

## Pin Handling

### Lazy Initialization

Pin modes are set on first read, not in constructor:

```cpp
// Constructor: No pinMode() calls
MinimalUltrasonic::MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin)
    : _trigPin(trigPin), _echoPin(echoPin) {
    // No pinMode() here
}

// timing(): Set pin modes when needed
unsigned long MinimalUltrasonic::timing() {
    pinMode(_trigPin, OUTPUT);  // Set here
    pinMode(_echoPin, INPUT);   // Set here
    
    // ... rest of timing code
}
```

**Rationale:**

- Faster construction
- Allows sensors to be created before `setup()`
- No side effects during construction

### Pin Mode Management

```cpp
// For 4-pin sensors
pinMode(_trigPin, OUTPUT);
pinMode(_echoPin, INPUT);

// For 3-pin sensors (same pin)
// Pin mode switches between INPUT and OUTPUT
pinMode(_trigPin, OUTPUT);  // For trigger
// ... trigger pulse ...
pinMode(_trigPin, INPUT);   // For echo (3-pin only)
```

## Unit Conversion System

### Conversion Factor Table

```cpp
float MinimalUltrasonic::convertUnit(float cm) {
    switch (_unit) {
        case CM:     return cm;
        case METERS: return cm * 0.01;
        case MM:     return cm * 10.0;
        case INCHES: return cm * 0.393701;
        case YARDS:  return cm * 0.0109361;
        case MILES:  return cm * 0.000006213712;
        default:     return cm;
    }
}
```

**Design Choice:** Switch statement instead of lookup table

- **Benefit:** Compiler optimizes to jump table or conditional branches
- **Trade-off:** Slightly more code than array lookup, but type-safe
- **Performance:** Negligible difference (<10 CPU cycles)

### Conversion Flow

```txt
Raw Echo Time (µs)
       ↓
  Divide by 58.8235
       ↓
  Distance in CM (base unit)
       ↓
  Apply unit conversion factor
       ↓
  Distance in selected unit
```

## Timing Implementation

### Pulse Timing

```cpp
unsigned long MinimalUltrasonic::timing() {
    // Prepare
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    
    // Trigger pulse (10µs)
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);
    
    // Measure echo
    unsigned long duration = pulseIn(_echoPin, HIGH, _timeout);
    
    return duration;
}
```

### pulseIn() Usage

```cpp
pulseIn(_echoPin, HIGH, _timeout)
```

**Parameters:**

- `_echoPin`: Pin to measure
- `HIGH`: Wait for HIGH pulse
- `_timeout`: Maximum wait time (µs)

**Return:**

- Pulse duration in microseconds
- `0` if timeout

**Why pulseIn()?**

- Built-in Arduino function
- Hardware-optimized on some platforms
- Reliable and well-tested
- Handles timeout automatically

## Error Handling Strategy

### Return Value Convention

```cpp
float read() {
    unsigned long time = timing();
    
    if (time == 0) {
        return 0;  // Error indicator
    }
    
    float cm = time / 58.8235;
    return convertUnit(cm);
}
```

**Design Choice:** Return 0 for errors

- **Benefit:** Simple, single return type
- **Trade-off:** 0 is a valid distance (though unlikely)
- **Rationale:** Ultrasonic sensors can't reliably measure <2cm anyway

### No Exceptions

Library does not use exceptions:

- **Reason:** Many Arduino platforms don't support exceptions
- **Alternative:** Return value indicates success/failure
- **Pattern:** Check for 0 return value

## Platform Compatibility

### Arduino Core Dependencies

```cpp
// Required Arduino functions
pinMode()
digitalWrite()
digitalRead()
delayMicroseconds()
pulseIn()
micros()
millis()
```

All standard Arduino functions, available on all platforms.

### Platform-Specific Optimizations

```cpp
// Future: Could use direct port manipulation
// For now: Use standard Arduino functions for compatibility
```

**Trade-off:** Slightly slower, but works everywhere

## Optimization Techniques

### 1. Inline Keyword Usage

```cpp
// Small, frequently called methods marked inline
inline float convertUnit(float cm);
inline unsigned long getTimeout() const;
```

**Benefit:** Compiler can eliminate function call overhead

### 2. Const Correctness

```cpp
// Methods that don't modify state marked const
Unit getUnit() const;
unsigned long getTimeout() const;
```

**Benefit:**

- Allows use with const objects
- Compiler optimization opportunities
- Clear intent

### 3. Integer vs Float

```cpp
// Use unsigned long for timing (integer arithmetic faster)
unsigned long timing();
unsigned long _timeout;

// Use float only when necessary (measurements)
float read();
float convertUnit(float cm);
```

### 4. No Dynamic Memory

```cpp
// Everything stack-allocated
// No new/delete
// No malloc/free
```

**Benefit:**

- Predictable memory usage
- No fragmentation
- Faster execution

## Code Size Optimization

### Minimal Dependencies

```cpp
// Only includes Arduino.h
#include <Arduino.h>

// No additional libraries
// Self-contained implementation
```

### Feature Minimalism

```cpp
// Only essential features included:
// - Distance reading
// - Unit conversion
// - Timeout configuration

// Not included (keep size small):
// - Temperature compensation
// - Statistical analysis
// - Built-in filtering
// - Display functions
```

**Philosophy:** Library does one thing well. User adds advanced features as needed.

## Thread Safety

### Not Thread-Safe

```cpp
// Instance is not thread-safe
// Don't call methods from multiple threads/interrupts simultaneously
```

**Rationale:**

- Most Arduino code is single-threaded
- Thread safety adds overhead
- User can add mutex if needed

### Multiple Instances Are Independent

```cpp
MinimalUltrasonic sensor1(12, 13);
MinimalUltrasonic sensor2(10, 11);

// These are independent
// Can be used from different contexts
// As long as each instance accessed from single thread
```

## Future Improvements

### Possible Enhancements

1. **Optional Temperature Compensation**

   ```cpp
   float readCompensated(float tempCelsius);
   ```

2. **Direct Port Manipulation** (platform-specific)

   ```cpp
   #ifdef AVR
     // Use direct port access for speed
   #endif
   ```

3. **Interrupt-Based Timing** (advanced)

   ```cpp
   // Non-blocking measurements using interrupts
   ```

4. **DMA Support** (advanced platforms)

   ```cpp
   #ifdef STM32
     // Use DMA for ultra-low CPU usage
   #endif
   ```

### Design Constraints

Any enhancement must:

- Maintain backward compatibility
- Keep code size minimal
- Not impact performance of basic usage
- Be optional (not forced on all users)

## Design Patterns Used

### 1. Factory Pattern (Constructor Overloading)

```cpp
MinimalUltrasonic sensor1(12, 13);  // 4-pin
MinimalUltrasonic sensor2(10);      // 3-pin
```

### 2. Strategy Pattern (Unit Conversion)

```cpp
enum Unit { CM, METERS, MM, INCHES, YARDS, MILES };
// Different strategies for conversion
```

### 3. Template Method (Implicit)

```cpp
// read() is template:
// - timing() - specific to hardware
// - convertUnit() - specific to unit
```

## Testing Strategy

### Unit Tests (Not Included)

```cpp
// Could add:
// - Test unit conversions
// - Test timeout calculations
// - Test edge cases
```

### Hardware Tests

```cpp
// Included examples serve as integration tests
// - Basic functionality
// - Multiple sensors
// - All units
```

## Documentation Philosophy

- **API documentation** in header file (Doxygen comments)
- **Usage examples** in separate files
- **Technical details** in this document
- **User guides** in online documentation

## See Also

- [Physics Background](/technical/physics) - Ultrasonic principles
- [Performance](/technical/performance) - Performance characteristics
- [Compatibility](/technical/compatibility) - Platform compatibility
- [API Reference](/api/reference) - Complete API documentation

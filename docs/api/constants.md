# Constants

Physical constants and default values used in MinimalUltrasonic.

## Physical Constants

### Speed of Sound

```cpp
const float SOUND_SPEED = 343.0;  // meters/second at 20°C
```

**Description:**  
Speed of sound in dry air at 20°C (68°F) and standard atmospheric pressure.

**Usage:**  
Used internally for all distance calculations. This value is fixed and assumes standard conditions.

**Temperature Variation:**

```cpp
// Speed of sound varies with temperature:
// v = 331.3 + (0.606 × T) m/s
// Where T is temperature in Celsius

// At different temperatures:
// 0°C:  331.3 m/s
// 10°C: 337.4 m/s
// 20°C: 343.0 m/s (used by library)
// 30°C: 349.1 m/s
```

**Note:** MinimalUltrasonic uses a fixed value. For high-precision applications at extreme temperatures, consider external compensation.

---

### Time Divisor for CM

```cpp
const float TIME_DIVISOR_CM = 58.8235;  // µs per cm (round trip)
```

**Description:**  
Number of microseconds for sound to travel 1 cm and back.

**Derivation:**

```cpp
// Speed of sound: 343 m/s = 0.0343 cm/µs
// Round trip distance: 2 × distance
// Time = distance / (speed / 2)
// Time per cm = 1 / (0.0343 / 2) = 58.8235 µs/cm
```

**Usage:**

```cpp
// Convert microseconds to centimeters
float cm = microseconds / 58.8235;

// Convert centimeters to microseconds
unsigned long microseconds = cm * 58.8235;
```

---

## Default Values

### Default Timeout

```cpp
const unsigned long DEFAULT_TIMEOUT = 20000UL;  // microseconds
```

**Description:**  
Default maximum time to wait for echo pulse.

**Equivalent Range:**

```cpp
// Max distance = (timeout × 343) / (2 × 10000)
// Max distance = 20000 / 58.8235
// Max distance ≈ 340 cm = 3.4 meters
```

**Why 20000µs?**

- Provides ~3.4m range
- Suitable for most applications
- Good balance between range and response time
- Standard for HC-SR04 sensors

**Modification:**

```cpp
MinimalUltrasonic sensor(12, 13);
sensor.setTimeout(10000UL);  // Change to 1.7m range
```

---

### Default Unit

```cpp
const Unit DEFAULT_UNIT = CM;  // Centimeters
```

**Description:**  
Default measurement unit when sensor is created.

**Rationale:**

- Most universally understood
- Provides good precision
- Matches typical sensor range (2-400cm)
- Standard in Arduino community

**Modification:**

```cpp
MinimalUltrasonic sensor(12, 13);
sensor.setUnit(MinimalUltrasonic::METERS);  // Change to meters
```

---

## Conversion Factors

### Metric Units

```cpp
const float FACTOR_CM = 1.0;         // Base unit
const float FACTOR_METERS = 0.01;    // 1 m = 100 cm
const float FACTOR_MM = 10.0;        // 1 cm = 10 mm
```

### Imperial Units

```cpp
const float FACTOR_INCHES = 0.393701;  // 1 cm = 0.393701 inches
const float FACTOR_YARDS = 0.0109361;  // 1 cm = 0.0109361 yards
const float FACTOR_MILES = 6.21371e-6; // 1 cm = 6.21371×10⁻⁶ miles
```

### Derivation

```cpp
// INCHES
// 1 inch = 2.54 cm
// 1 cm = 1 / 2.54 = 0.393701 inches

// YARDS
// 1 yard = 91.44 cm
// 1 cm = 1 / 91.44 = 0.0109361 yards

// MILES
// 1 mile = 160934.4 cm
// 1 cm = 1 / 160934.4 = 6.21371×10⁻⁶ miles
```

---

## Timing Constants

### Trigger Pulse Duration

```cpp
const unsigned int TRIGGER_PULSE = 10;  // microseconds
```

**Description:**  
Duration of trigger pulse sent to sensor.

**HC-SR04 Specification:**

- Minimum: 10µs
- Typical: 10µs
- Maximum: Not specified

**Why 10µs?**

- Minimum required by HC-SR04
- Widely compatible
- Reliable trigger

---

### Minimum Delay Between Readings

```cpp
const unsigned int MIN_DELAY = 60;  // milliseconds (recommended)
```

**Description:**  
Recommended minimum time between consecutive readings.

**Why 60ms?**

- Allows time for ultrasonic burst to dissipate
- Prevents echo interference
- Sensor recovery time
- Based on: max_time (40ms) + margin (20ms)

**Usage:**

```cpp
void loop() {
    float distance = sensor.read();
    delay(60);  // Minimum recommended delay
}
```

---

## Range Constants

### Sensor Physical Limits (HC-SR04)

```cpp
const float MIN_DISTANCE_CM = 2.0;    // Minimum reliable distance
const float MAX_DISTANCE_CM = 400.0;  // Maximum distance
```

**Min Distance (2 cm):**

- Physical limitation of sensor
- Below this: blind zone
- Transducers too close for timing

**Max Distance (400 cm):**

- Based on default timeout
- Can be extended with longer timeout
- Practical limit: ~6-7 meters
- Beyond: signal too weak

**Range Variation:**

```cpp
// Different timeout values:
// 5000µs  → ~85 cm max
// 10000µs → ~170 cm max
// 20000µs → ~340 cm max (default)
// 40000µs → ~680 cm max
```

---

## Accuracy Constants

### Theoretical Accuracy

```cpp
const float THEORETICAL_ACCURACY = 0.3;  // cm (±3mm)
```

**Description:**  
Theoretical accuracy based on timing resolution.

**Factors:**

- Arduino timing resolution: 4µs (on 16MHz AVR)
- Distance per 4µs: ~0.7mm
- Typical measured: ±3mm

### Practical Accuracy

```cpp
const float PRACTICAL_ACCURACY = 1.0;  // cm (±1cm)
```

**Description:**  
Real-world accuracy considering all factors.

**Factors affecting accuracy:**

- Temperature variation
- Humidity
- Air pressure
- Target surface
- Sensor quality
- Electrical noise

---

## Memory Constants

### Instance Size

```cpp
const size_t INSTANCE_SIZE = 8;  // bytes per sensor instance
```

**Memory Layout:**

```cpp
struct MinimalUltrasonic {
    uint8_t _trigPin;           // 1 byte
    uint8_t _echoPin;           // 1 byte
    Unit _unit;                 // 1 byte
    unsigned long _timeout;     // 4 bytes
    bool _singlePin;            // 1 byte
    // Total: 8 bytes
};
```

### Code Size

Approximate compiled sizes (AVR):

```cpp
const size_t CODE_SIZE_BASIC = 500;      // bytes (basic usage)
const size_t CODE_SIZE_FULL = 800;       // bytes (all features)
```

**Comparison:**

- NewPing library: ~1.5KB
- Ultrasonic library: ~1KB
- MinimalUltrasonic: ~0.5-0.8KB

---

## Environmental Constants

### Standard Conditions

```cpp
const float STANDARD_TEMP = 20.0;        // °C
const float STANDARD_HUMIDITY = 50.0;    // %
const float STANDARD_PRESSURE = 101.325; // kPa
```

**Note:** Library assumes standard conditions. For extreme environments, consider external compensation.

### Speed of Sound Temperature Coefficient

```cpp
const float TEMP_COEFFICIENT = 0.606;  // (m/s) per °C
```

**Usage for temperature compensation:**

```cpp
float compensateForTemperature(float rawDistance, float tempC) {
    float actualSpeed = 331.3 + (0.606 * tempC);
    float correctionFactor = actualSpeed / 343.0;
    return rawDistance * correctionFactor;
}
```

---

## Usage Examples

### Using Constants in Your Code

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    
    // Use default timeout
    Serial.print("Default timeout: ");
    Serial.print(sensor.getTimeout());
    Serial.println(" µs");
    
    // Calculate max range
    float maxRange = sensor.getTimeout() / 58.8235;
    Serial.print("Max range: ");
    Serial.print(maxRange);
    Serial.println(" cm");
}

void loop() {
    float distance = sensor.read();
    
    // Check against sensor limits
    if (distance < 2.0) {
        Serial.println("Too close! (< 2 cm)");
    } else if (distance > 400.0) {
        Serial.println("Too far! (> 400 cm)");
    } else if (distance > 0) {
        Serial.print("Valid: ");
        Serial.print(distance);
        Serial.println(" cm");
    }
    
    delay(60);  // Minimum recommended delay
}
```

### Reference Table

```cpp
void printReferenceTable() {
    Serial.println("=== MinimalUltrasonic Constants ===");
    Serial.print("Speed of sound: 343 m/s");
    Serial.print("Time per cm: 58.8235 µs");
    Serial.print("Default timeout: 20000 µs");
    Serial.print("Default unit: CM");
    Serial.print("Min distance: 2 cm");
    Serial.print("Max distance: 400 cm");
    Serial.print("Min delay: 60 ms");
    Serial.print("Instance size: 8 bytes");
    Serial.println("===================================");
}
```

---

## Constant Modification

Constants are **hardcoded** and cannot be changed at runtime. However, you can:

### Override Defaults

```cpp
// Change timeout (affects max range)
sensor.setTimeout(10000UL);  // Override default 20000µs

// Change unit
sensor.setUnit(MinimalUltrasonic::METERS);  // Override default CM
```

### Library Modification

To change constants permanently:

1. Edit `src/MinimalUltrasonic.cpp`
2. Modify constant values
3. Recompile

**Example:**

```cpp
// In MinimalUltrasonic.cpp
// Change default timeout to 30000µs
MinimalUltrasonic::MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin)
    : _trigPin(trigPin), _echoPin(echoPin), 
      _unit(CM), _timeout(30000UL), _singlePin(false) {  // Changed
}
```

---

## See Also

- [MinimalUltrasonic Class](/api/class) - Class overview
- [Physics Background](/technical/physics) - Physics theory
- [Unit Conversions](/technical/conversions) - Conversion details
- [Performance](/technical/performance) - Performance characteristics

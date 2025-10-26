# MinimalUltrasonic - Technical Documentation

## Architecture Overview

### Design Principles

1. **Minimalism**: Keep the library lean with only essential features
2. **Efficiency**: Optimize for low memory footprint and fast execution
3. **Precision**: Use accurate calculations for distance measurements
4. **Flexibility**: Support multiple sensor types and measurement units
5. **Clarity**: Provide clear, well-documented API

### Class Structure

```
MinimalUltrasonic
├── Public Interface
│   ├── Constructors (2 overloads)
│   ├── read() - Main measurement method
│   ├── setTimeout() - Configure timeout
│   ├── setMaxDistance() - Set max range
│   ├── getTimeout() - Query timeout
│   ├── setUnit() - Set default unit
│   └── getUnit() - Query default unit
│
├── Private Implementation
│   ├── timing() - Perform ultrasonic measurement
│   └── convertToUnit() - Convert microseconds to distance
│
└── Data Members
    ├── _trigPin - Trigger pin number
    ├── _echoPin - Echo pin number
    ├── _isThreePin - 3-pin vs 4-pin mode
    ├── _timeout - Maximum wait time
    └── _defaultUnit - Default measurement unit
```

## Measurement Process

### Physics Background

Ultrasonic sensors use the time-of-flight principle:

1. **Trigger Pulse**: Send 10µs HIGH pulse on trigger pin
2. **Echo Wait**: Wait for echo pin to go HIGH (object detected)
3. **Echo Duration**: Measure time echo pin stays HIGH
4. **Distance Calculation**: `distance = (time × speed_of_sound) / 2`

### Speed of Sound

At 20°C (68°F) at sea level:

- Speed: 343 m/s = 34,300 cm/s = 0.0343 cm/µs
- Round-trip time per cm: 1 / (2 × 0.0343) ≈ 29.1 µs/cm

The library uses 29.1 µs/cm for balance between accuracy and computation.

### Timing Sequence (4-pin HC-SR04)

```
Trigger Pin:  ______|‾‾‾‾‾‾‾‾‾‾|____________
              LOW   |  10µs   | LOW
                    |         |
Echo Pin:     ______|         |_________|‾‾‾‾‾‾‾‾‾‾‾|_____
              LOW   |  wait   | LOW     | duration  | LOW
                                        |<--------->|
                                         measured time
```

### Timing Sequence (3-pin Ping/Seeed)

```
Signal Pin:   ______|‾‾‾‾‾‾‾‾‾‾|____________
(as OUTPUT)   LOW   |  10µs   | LOW
              
Signal Pin:   _____________________|‾‾‾‾‾‾‾‾‾‾‾|_____
(as INPUT)    LOW       wait       | duration  | LOW
                                   |<--------->|
                                    measured time
```

## Unit Conversions

All conversions start from the time-of-flight in microseconds:

### Conversion Formulas

1. **Centimeters**: `cm = (µs / 29.1) / 2`
2. **Meters**: `m = cm / 100`
3. **Millimeters**: `mm = cm × 10`
4. **Inches**: `in = cm / 2.54`
5. **Yards**: `yd = cm / 91.44`
6. **Miles**: `mi = cm / 160934.4`

### Conversion Constants

| From | To | Factor |
|------|-----|---------|
| cm | m | ÷ 100 |
| cm | mm | × 10 |
| cm | in | ÷ 2.54 |
| cm | yd | ÷ 91.44 |
| cm | mi | ÷ 160934.4 |

## Timeout Configuration

### Timeout vs Range Relationship

The timeout determines maximum detectable distance:

```
max_distance (cm) = (timeout_µs / 29.1) / 2
timeout_µs = max_distance_cm × 29.1 × 2
```

### Common Timeout Values

| Timeout (µs) | Max Range (cm) | Max Range (m) |
|--------------|----------------|---------------|
| 10,000 | 172 | 1.72 |
| 20,000 | 344 | 3.44 |
| 30,000 | 516 | 5.16 |
| 40,000 | 688 | 6.88 |
| 50,000 | 860 | 8.60 |

### Timeout Considerations

**Shorter Timeout (< 20ms)**

- Pros: Faster reading cycle, lower latency
- Cons: Limited detection range
- Use case: Close-range detection (< 3m)

**Medium Timeout (20-30ms)**

- Pros: Good balance of speed and range
- Cons: None significant
- Use case: General purpose (3-5m)

**Longer Timeout (> 30ms)**

- Pros: Extended range detection
- Cons: Slower readings if no object detected
- Use case: Long-range detection (> 5m)

## Memory Usage

### Static Memory (per instance)

```
uint8_t _trigPin;        // 1 byte
uint8_t _echoPin;        // 1 byte
bool _isThreePin;        // 1 byte
unsigned long _timeout;  // 4 bytes
Unit _defaultUnit;       // 1 byte
-----------------------------------
Total:                   // 8 bytes per instance
```

### Code Size

Approximate compiled size (Arduino Uno):

- Library code: ~1.2 KB
- Basic usage: ~150 bytes
- Total overhead: ~1.4 KB

## Performance Characteristics

### Timing Performance

- **Trigger pulse**: 12 µs (2µs LOW + 10µs HIGH)
- **Pin mode switch** (3-pin): ~4 µs
- **Distance read** (no object): timeout duration
- **Distance read** (with object): ~30 µs + echo time
- **Unit conversion**: < 10 µs

### Measurement Accuracy

Factors affecting accuracy:

1. **Sensor Quality**: ±3mm (HC-SR04 typical)
2. **Temperature**: ±1% per 10°C deviation from 20°C
3. **Surface Angle**: Best with perpendicular surfaces
4. **Surface Material**: Soft/absorbent materials reduce accuracy
5. **Ambient Noise**: Ultrasonic interference

### Recommended Practices

1. **Sampling**: Take multiple readings and average for better accuracy
2. **Delay**: Wait 30-60ms between consecutive readings
3. **Filtering**: Discard outliers (readings that vary significantly)
4. **Calibration**: Measure known distances to verify accuracy

## Error Handling

### Return Value of 0

The `read()` method returns `0` in these cases:

1. **Timeout**: No echo received within timeout period
2. **No Object**: No object in detection range
3. **Echo Lost**: Echo pulse starts but exceeds timeout

### Detecting Errors

```cpp
float distance = sensor.read();

if (distance == 0) {
    // No valid reading
    // Could be: timeout, no object, or out of range
} else if (distance < 2) {
    // Too close - minimum range ~2cm
    // Reading may be unreliable
} else if (distance > maxExpectedDistance) {
    // Suspiciously far - could be false echo
    // Consider as error or re-read
} else {
    // Valid reading
}
```

## Optimization Tips

### For Speed

```cpp
// 1. Use shorter timeout for close-range
sensor.setMaxDistance(200);  // 2m max

// 2. Avoid unnecessary conversions
float cm = sensor.read();  // Use default unit

// 3. Store sensor objects globally
MinimalUltrasonic sensor(12, 13);  // Global, not in loop
```

### For Accuracy

```cpp
// 1. Average multiple readings
float total = 0;
for (int i = 0; i < 5; i++) {
    total += sensor.read();
    delay(30);
}
float average = total / 5;

// 2. Median filter
float readings[5];
for (int i = 0; i < 5; i++) {
    readings[i] = sensor.read();
    delay(30);
}
// Sort and take middle value
```

### For Range

```cpp
// Increase timeout for longer distances
sensor.setTimeout(50000UL);  // ~8.6m max range

// Note: Longer timeout = slower readings on miss
```

## Compatibility Notes

### Arduino Boards

Tested and compatible with:

- Arduino Uno
- Arduino Mega
- Arduino Nano
- Arduino Pro Mini
- ESP32 (most variants)
- ESP8266

### Sensor Compatibility

**Fully Compatible:**

- HC-SR04 (4-pin)
- HC-SR04+ (3.3V/5V)
- US-015 (4-pin)
- Ping))) (3-pin)
- Seeed SEN136B5B (3-pin)

**May Work:**

- JSN-SR04T (waterproof variant)
- HY-SRF05 (similar to HC-SR04)

### Voltage Considerations

- **5V Sensors**: Use directly with 5V Arduino boards
- **3.3V Sensors**: Use with 3.3V boards or level shifters
- **Mixed Voltage**: Use level shifters for signal pins

## Troubleshooting Guide

### Problem: Reading always 0

**Possible Causes:**

1. Incorrect wiring
2. Insufficient power supply
3. Sensor damaged
4. Timeout too short
5. No object in range

**Solutions:**

1. Verify connections (VCC, GND, Trig, Echo)
2. Use separate 5V power for sensor
3. Test with different sensor
4. Increase timeout: `sensor.setTimeout(40000UL)`
5. Place object in front of sensor

### Problem: Erratic readings

**Possible Causes:**

1. Electrical interference
2. Multiple sensors interfering
3. Surface too soft/angled
4. Unstable power supply
5. Temperature effects

**Solutions:**

1. Add decoupling capacitor (100µF) near sensor
2. Space sensors apart, add delays
3. Use flat, hard surface for testing
4. Use regulated power supply
5. Apply temperature compensation

### Problem: Limited range

**Possible Causes:**

1. Default timeout too short
2. Sensor degradation
3. Acoustic damping
4. Object too small/angled

**Solutions:**

1. Increase timeout: `sensor.setMaxDistance(600)`
2. Replace sensor
3. Remove acoustic absorbing materials
4. Use larger, flat target surface

## Advanced Usage

### Temperature Compensation

```cpp
float getCompensatedDistance(float rawDistance, float tempC) {
    // Speed of sound varies with temperature
    // v = 331.3 + 0.606 × T (m/s)
    float speedRatio = (331.3 + 0.606 * tempC) / 343.0;
    return rawDistance * speedRatio;
}
```

### Median Filter Implementation

```cpp
float medianRead(MinimalUltrasonic& sensor, int samples = 5) {
    float readings[samples];
    
    // Collect samples
    for (int i = 0; i < samples; i++) {
        readings[i] = sensor.read();
        delay(30);
    }
    
    // Sort (simple bubble sort for small arrays)
    for (int i = 0; i < samples - 1; i++) {
        for (int j = 0; j < samples - i - 1; j++) {
            if (readings[j] > readings[j + 1]) {
                float temp = readings[j];
                readings[j] = readings[j + 1];
                readings[j + 1] = temp;
            }
        }
    }
    
    // Return median
    return readings[samples / 2];
}
```

### Moving Average Filter

```cpp
class FilteredUltrasonic {
private:
    MinimalUltrasonic sensor;
    float readings[10];
    int index = 0;
    
public:
    FilteredUltrasonic(uint8_t trig, uint8_t echo) 
        : sensor(trig, echo) {}
    
    float read() {
        readings[index] = sensor.read();
        index = (index + 1) % 10;
        
        float sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += readings[i];
        }
        return sum / 10.0;
    }
};
```

## Version History & Compatibility

### v2.0.0 (Current)

- Multiple unit support
- Float return type
- Enhanced documentation
- Backward compatible with v1.x (with minor changes)

### v1.0.0 (Legacy)

- Basic cm/inch support
- Integer return type
- Original implementation

### Migration Checklist

- [ ] Update include to `<MinimalUltrasonic.h>`
- [ ] Change distance variables from `int` to `float`
- [ ] Update unit constants to enum style (optional)
- [ ] Test all functionality
- [ ] Update documentation/comments

---

**Document Version**: 2.0.0  
**Last Updated**: October 25, 2025  
**Author**: fermeridamagni (Magni Development)

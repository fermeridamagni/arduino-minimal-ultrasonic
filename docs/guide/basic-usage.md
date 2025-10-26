# Basic Usage

Learn the essential methods and patterns for using MinimalUltrasonic.

## Creating a Sensor Object

### 4-Pin Sensors (HC-SR04)

```cpp
#include <MinimalUltrasonic.h>

// Create sensor with trigger on pin 12, echo on pin 13
MinimalUltrasonic sensor(12, 13);
```

### 3-Pin Sensors (Ping, Seeed)

```cpp
#include <MinimalUltrasonic.h>

// Create sensor with signal on pin 13
MinimalUltrasonic sensor(13);
```

### With Custom Timeout

```cpp
// 4-pin with 40ms timeout (~6.8m range)
MinimalUltrasonic sensor(12, 13, 40000UL);

// 3-pin with 30ms timeout (~5.1m range)
MinimalUltrasonic sensor(13, 30000UL);
```

## Reading Distance

### Basic Reading

The `read()` method returns distance as a float:

```cpp
float distance = sensor.read();
Serial.println(distance);  // Prints in cm (default)
```

### With Specific Unit

```cpp
float cm = sensor.read(MinimalUltrasonic::CM);
float meters = sensor.read(MinimalUltrasonic::METERS);
float inches = sensor.read(MinimalUltrasonic::INCHES);
```

### Handling Return Values

```cpp
float distance = sensor.read();

if (distance == 0) {
    Serial.println("No object detected or timeout");
} else if (distance < 2) {
    Serial.println("Object too close");
} else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}
```

## Complete Example

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
    Serial.begin(9600);
    Serial.println("Distance Measurement");
    Serial.println("===================");
}

void loop() {
    float distance = sensor.read();
    
    if (distance > 0) {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
    } else {
        Serial.println("Out of range");
    }
    
    delay(500);
}
```

## Configuration Methods

### setTimeout()

Set maximum wait time for echo response:

```cpp
sensor.setTimeout(30000UL);  // 30ms timeout
```

### setMaxDistance()

Set maximum distance in centimeters:

```cpp
sensor.setMaxDistance(500);  // 5 meters max
```

This automatically calculates the appropriate timeout.

### setUnit()

Set default unit for all read() calls:

```cpp
sensor.setUnit(MinimalUltrasonic::METERS);
float dist = sensor.read();  // Returns meters now
```

## Query Methods

### getTimeout()

Get current timeout value:

```cpp
unsigned long timeout = sensor.getTimeout();
Serial.print("Timeout: ");
Serial.print(timeout);
Serial.println(" microseconds");
```

### getUnit()

Get current default unit:

```cpp
MinimalUltrasonic::Unit unit = sensor.getUnit();

if (unit == MinimalUltrasonic::CM) {
    Serial.println("Using centimeters");
}
```

## Best Practices

### Add Delays Between Readings

```cpp
void loop() {
    float distance = sensor.read();
    Serial.println(distance);
    delay(30);  // Minimum 30ms recommended
}
```

### Store Sensor Objects Globally

```cpp
// ✅ Good - created once
MinimalUltrasonic sensor(12, 13);

void loop() {
    float dist = sensor.read();
}
```

```cpp
// ❌ Bad - recreated every loop
void loop() {
    MinimalUltrasonic sensor(12, 13);
    float dist = sensor.read();
}
```

### Check for Valid Readings

```cpp
float distance = sensor.read();

// Check for errors
if (distance == 0) {
    // Handle timeout/error
    return;
}

// Check range
if (distance < 2 || distance > 400) {
    // Out of reliable range
    return;
}

// Valid reading
processDistance(distance);
```

## Common Patterns

### Average Multiple Readings

```cpp
float getAverageDistance(int samples = 5) {
    float total = 0;
    
    for (int i = 0; i < samples; i++) {
        total += sensor.read();
        delay(30);
    }
    
    return total / samples;
}
```

### Distance with Filtering

```cpp
float lastDistance = 0;
const float FILTER_FACTOR = 0.7;

float getFilteredDistance() {
    float current = sensor.read();
    
    if (current > 0) {
        lastDistance = (lastDistance * FILTER_FACTOR) + 
                      (current * (1.0 - FILTER_FACTOR));
    }
    
    return lastDistance;
}
```

### Threshold Detection

```cpp
const float THRESHOLD = 30.0;  // 30cm

void loop() {
    float distance = sensor.read();
    
    if (distance > 0 && distance < THRESHOLD) {
        Serial.println("Object detected!");
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
    
    delay(100);
}
```

## Next Steps

- **[Multiple Units](/guide/multiple-units)** - Work with different measurements
- **[Timeout Configuration](/guide/timeout-configuration)** - Adjust maximum range
- **[Multiple Sensors](/guide/multiple-sensors)** - Use multiple sensors
- **[Examples](/examples/basic)** - See complete working examples

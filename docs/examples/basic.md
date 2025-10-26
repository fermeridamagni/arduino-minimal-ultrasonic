# Basic Example

Simple distance measurement example.

## Code

```cpp
#include <MinimalUltrasonic.h>

// For HC-SR04 (4-pin): specify trigger and echo pins
MinimalUltrasonic sensor(12, 13);

// For Ping/Seeed (3-pin): specify only signal pin
// MinimalUltrasonic sensor(13);

void setup() {
  Serial.begin(9600);
  Serial.println("Distance Sensor - Basic Example");
  Serial.println("================================");
}

void loop() {
  // Read distance in centimeters (default)
  float distance = sensor.read();
  
  // Display result
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Wait before next reading
  delay(1000);
}
```

## Hardware Setup

### For HC-SR04

```txt
HC-SR04      Arduino
-------------------------
VCC     →    5V
Trig    →    Pin 12
Echo    →    Pin 13
GND     →    GND
```

### For Ping))) or Seeed

```txt
Sensor       Arduino
-------------------------
VCC     →    5V
SIG     →    Pin 13
GND     →    GND
```

## Expected Output

```txt
Distance Sensor - Basic Example
================================
Distance: 15.23 cm
Distance: 15.18 cm
Distance: 15.31 cm
Distance: 23.45 cm
Distance: 23.52 cm
...
```

## How It Works

1. **Include Library** - Makes MinimalUltrasonic available
2. **Create Sensor** - Initializes sensor on specified pins
3. **Setup** - Initializes Serial communication
4. **Read Distance** - `sensor.read()` returns distance in cm
5. **Display** - Prints distance to Serial Monitor
6. **Delay** - Waits 1 second before next measurement

## Variations

### Different Update Rate

```cpp
void loop() {
  float distance = sensor.read();
  Serial.println(distance);
  delay(100);  // Read every 100ms
}
```

### With Error Checking

```cpp
void loop() {
  float distance = sensor.read();
  
  if (distance == 0) {
    Serial.println("ERROR: No object detected");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  
  delay(1000);
}
```

### Formatted Output

```cpp
void loop() {
  float distance = sensor.read();
  
  Serial.print("Distance: ");
  Serial.print(distance, 2);  // 2 decimal places
  Serial.println(" cm");
  
  delay(1000);
}
```

## Next Steps

- [All Units Example](/examples/all-units) - Measure in different units
- [Timeout Example](/examples/timeout) - Configure timeout
- [Multiple Sensors](/examples/multiple-sensors) - Use multiple sensors

## Download

You can also find this example in Arduino IDE:

`File` → `Examples` → `MinimalUltrasonic` → `UltrasonicSimple`

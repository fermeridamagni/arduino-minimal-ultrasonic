# All Units Example

Demonstrate distance measurement in all supported units.

## Overview

This example shows how to use all six measurement units supported by MinimalUltrasonic:

- Centimeters (CM)
- Meters (METERS)
- Millimeters (MM)
- Inches (INCHES)
- Yards (YARDS)
- Miles (MILES)

## Hardware Required

- Arduino Uno (or compatible)
- HC-SR04 Ultrasonic Sensor
- Jumper wires

## Circuit

Same as [Basic Example](/examples/basic):

```txt
HC-SR04    Arduino
--------   -------
VCC    ->  5V
GND    ->  GND
TRIG   ->  Pin 12
ECHO   ->  Pin 13
```

## Code

```cpp
/**
 * All Units Example - MinimalUltrasonic
 * 
 * Demonstrates measurement in all supported units:
 * - Centimeters (CM)
 * - Meters (METERS)
 * - Millimeters (MM)
 * - Inches (INCHES)
 * - Yards (YARDS)
 * - Miles (MILES)
 * 
 * Hardware:
 * - HC-SR04 Ultrasonic Sensor
 * - Arduino Uno or compatible
 */

#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  Serial.println("MinimalUltrasonic - All Units Example");
  Serial.println("======================================");
  Serial.println();
}

void loop() {
  Serial.println("Measuring distance in all units:");
  Serial.println("----------------------------------");
  
  // Centimeters (default)
  sensor.setUnit(MinimalUltrasonic::CM);
  float cm = sensor.read();
  Serial.print("Centimeters:  ");
  Serial.print(cm, 2);
  Serial.println(" cm");
  delay(50);
  
  // Meters
  sensor.setUnit(MinimalUltrasonic::METERS);
  float meters = sensor.read();
  Serial.print("Meters:       ");
  Serial.print(meters, 4);
  Serial.println(" m");
  delay(50);
  
  // Millimeters
  sensor.setUnit(MinimalUltrasonic::MM);
  float mm = sensor.read();
  Serial.print("Millimeters:  ");
  Serial.print(mm, 1);
  Serial.println(" mm");
  delay(50);
  
  // Inches
  sensor.setUnit(MinimalUltrasonic::INCHES);
  float inches = sensor.read();
  Serial.print("Inches:       ");
  Serial.print(inches, 2);
  Serial.println(" in");
  delay(50);
  
  // Yards
  sensor.setUnit(MinimalUltrasonic::YARDS);
  float yards = sensor.read();
  Serial.print("Yards:        ");
  Serial.print(yards, 4);
  Serial.println(" yd");
  delay(50);
  
  // Miles
  sensor.setUnit(MinimalUltrasonic::MILES);
  float miles = sensor.read();
  Serial.print("Miles:        ");
  Serial.print(miles, 7);
  Serial.println(" mi");
  
  Serial.println("==================================");
  Serial.println();
  
  // Wait before next cycle
  delay(2000);
}
```

## Expected Output

```txt
MinimalUltrasonic - All Units Example
======================================

Measuring distance in all units:
----------------------------------
Centimeters:  100.23 cm
Meters:       1.0023 m
Millimeters:  1002.3 mm
Inches:       39.46 in
Yards:        1.0961 yd
Miles:        0.0006227 mi
==================================

Measuring distance in all units:
----------------------------------
Centimeters:  50.12 cm
Meters:       0.5012 m
Millimeters:  501.2 mm
Inches:       19.73 in
Yards:        0.5481 yd
Miles:        0.0003115 mi
==================================
```

## How It Works

### Setting Units

```cpp
sensor.setUnit(MinimalUltrasonic::CM);
float cm = sensor.read();
```

The `setUnit()` method changes the measurement unit. The next `read()` call returns distance in the new unit.

### Available Units

| Unit | Enum Value | Symbol | Typical Range |
|------|-----------|--------|---------------|
| Centimeters | `MinimalUltrasonic::CM` | cm | 2-400 |
| Meters | `MinimalUltrasonic::METERS` | m | 0.02-4 |
| Millimeters | `MinimalUltrasonic::MM` | mm | 20-4000 |
| Inches | `MinimalUltrasonic::INCHES` | in | 0.79-157 |
| Yards | `MinimalUltrasonic::YARDS` | yd | 0.022-4.37 |
| Miles | `MinimalUltrasonic::MILES` | mi | 0.000012-0.0025 |

### Precision

Different units work better at different ranges:

```cpp
// Short distances (< 10 cm): Use MM
sensor.setUnit(MinimalUltrasonic::MM);
float mm = sensor.read();  // e.g., 54.3 mm

// Medium distances (10-100 cm): Use CM
sensor.setUnit(MinimalUltrasonic::CM);
float cm = sensor.read();  // e.g., 45.2 cm

// Long distances (> 100 cm): Use METERS
sensor.setUnit(MinimalUltrasonic::METERS);
float m = sensor.read();   // e.g., 2.345 m
```

## Practical Examples

### Example 1: Display with Unit Name

```cpp
void displayWithUnit() {
  sensor.setUnit(MinimalUltrasonic::CM);
  float distance = sensor.read();
  
  // Get current unit
  MinimalUltrasonic::Unit currentUnit = sensor.getUnit();
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" ");
  
  // Print unit name
  switch (currentUnit) {
    case MinimalUltrasonic::CM:
      Serial.println("centimeters");
      break;
    case MinimalUltrasonic::METERS:
      Serial.println("meters");
      break;
    case MinimalUltrasonic::MM:
      Serial.println("millimeters");
      break;
    case MinimalUltrasonic::INCHES:
      Serial.println("inches");
      break;
    case MinimalUltrasonic::YARDS:
      Serial.println("yards");
      break;
    case MinimalUltrasonic::MILES:
      Serial.println("miles");
      break;
  }
}
```

### Example 2: User-Selected Unit

```cpp
void setup() {
  Serial.begin(9600);
  
  Serial.println("Select measurement unit:");
  Serial.println("1 - Centimeters");
  Serial.println("2 - Meters");
  Serial.println("3 - Inches");
  
  while (!Serial.available());
  int choice = Serial.read() - '0';
  
  switch (choice) {
    case 1:
      sensor.setUnit(MinimalUltrasonic::CM);
      Serial.println("Using Centimeters");
      break;
    case 2:
      sensor.setUnit(MinimalUltrasonic::METERS);
      Serial.println("Using Meters");
      break;
    case 3:
      sensor.setUnit(MinimalUltrasonic::INCHES);
      Serial.println("Using Inches");
      break;
    default:
      sensor.setUnit(MinimalUltrasonic::CM);
      Serial.println("Default: Centimeters");
  }
}

void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    Serial.println(distance);
  }
  
  delay(100);
}
```

### Example 3: Automatic Unit Selection

```cpp
void autoSelectUnit() {
  // Read in CM first
  sensor.setUnit(MinimalUltrasonic::CM);
  float cm = sensor.read();
  
  if (cm == 0) {
    Serial.println("No reading");
    return;
  }
  
  // Select best unit based on distance
  if (cm < 10) {
    // Very close: use MM
    sensor.setUnit(MinimalUltrasonic::MM);
    Serial.print(sensor.read(), 1);
    Serial.println(" mm");
  } else if (cm < 100) {
    // Medium: use CM
    Serial.print(cm, 2);
    Serial.println(" cm");
  } else {
    // Far: use METERS
    sensor.setUnit(MinimalUltrasonic::METERS);
    Serial.print(sensor.read(), 3);
    Serial.println(" m");
  }
}
```

### Example 4: Conversion Table

```cpp
void printConversionTable() {
  // Read once in CM
  sensor.setUnit(MinimalUltrasonic::CM);
  float cm = sensor.read();
  
  if (cm == 0) {
    Serial.println("No reading");
    return;
  }
  
  Serial.println("Distance Conversions:");
  Serial.println("=====================");
  
  // Calculate all units from CM
  Serial.print("CM:     "); Serial.println(cm, 2);
  Serial.print("Meters: "); Serial.println(cm * 0.01, 4);
  Serial.print("MM:     "); Serial.println(cm * 10.0, 1);
  Serial.print("Inches: "); Serial.println(cm * 0.393701, 2);
  Serial.print("Yards:  "); Serial.println(cm * 0.0109361, 4);
  Serial.print("Miles:  "); Serial.println(cm * 0.000006213712, 8);
  Serial.println("=====================");
}
```

## Performance Considerations

### Reading Frequency

Each `read()` call triggers a new measurement (~20-40ms):

```cpp
// SLOW: 6 readings = ~120-240ms total
sensor.setUnit(MinimalUltrasonic::CM);
float cm = sensor.read();
sensor.setUnit(MinimalUltrasonic::METERS);
float m = sensor.read();
sensor.setUnit(MinimalUltrasonic::INCHES);
float in = sensor.read();

// FASTER: Read once, convert manually
sensor.setUnit(MinimalUltrasonic::CM);
float cm = sensor.read();
float meters = cm * 0.01;
float inches = cm * 0.393701;
```

### Optimal Approach

For displaying multiple units, read once and convert:

```cpp
void efficientMultiUnit() {
  // Single reading
  sensor.setUnit(MinimalUltrasonic::CM);
  float cm = sensor.read();
  
  if (cm > 0) {
    // Manual conversions (instant)
    float meters = cm * 0.01;
    float mm = cm * 10.0;
    float inches = cm * 0.393701;
    
    Serial.print(cm, 2); Serial.print(" cm = ");
    Serial.print(meters, 4); Serial.print(" m = ");
    Serial.print(mm, 1); Serial.print(" mm = ");
    Serial.print(inches, 2); Serial.println(" in");
  }
}
```

## Unit Recommendations

### By Application

| Application | Recommended Unit | Reason |
|-------------|------------------|--------|
| Robot navigation | CM | Good precision, familiar |
| Parking sensor | CM or INCHES | Regional preference |
| Level monitoring | MM | High precision |
| Room dimensions | METERS | Appropriate scale |
| Science projects | METERS | SI unit |
| US consumer products | INCHES | Market expectation |

### By Distance Range

| Distance | Best Unit | Alternative |
|----------|-----------|-------------|
| 0-10 cm | MM | CM |
| 10-50 cm | CM | INCHES |
| 50-200 cm | CM | METERS |
| 200-400 cm | METERS | CM |

## Complete Sketch

Full example with all features:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

const char* getUnitName(MinimalUltrasonic::Unit unit) {
  switch (unit) {
    case MinimalUltrasonic::CM: return "Centimeters";
    case MinimalUltrasonic::METERS: return "Meters";
    case MinimalUltrasonic::MM: return "Millimeters";
    case MinimalUltrasonic::INCHES: return "Inches";
    case MinimalUltrasonic::YARDS: return "Yards";
    case MinimalUltrasonic::MILES: return "Miles";
    default: return "Unknown";
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("All Units Demo");
  Serial.println("==============");
}

void loop() {
  MinimalUltrasonic::Unit units[] = {
    MinimalUltrasonic::CM,
    MinimalUltrasonic::METERS,
    MinimalUltrasonic::MM,
    MinimalUltrasonic::INCHES,
    MinimalUltrasonic::YARDS,
    MinimalUltrasonic::MILES
  };
  
  for (int i = 0; i < 6; i++) {
    sensor.setUnit(units[i]);
    float distance = sensor.read();
    
    Serial.print(getUnitName(units[i]));
    Serial.print(": ");
    
    if (distance > 0) {
      Serial.println(distance, 4);
    } else {
      Serial.println("Error");
    }
    
    delay(50);
  }
  
  Serial.println("---");
  delay(2000);
}
```

## Next Steps

- **[Timeout Example](/examples/timeout)** - Configure detection range
- **[Multiple Sensors](/examples/multiple-sensors)** - Use multiple sensors
- **[Advanced Example](/examples/advanced)** - Filtering and error handling

## See Also

- [Multiple Units Guide](/guide/multiple-units)
- [Unit Enum Reference](/api/units)
- [Unit Conversions](/technical/conversions)

# Multiple Units

Learn how to measure distance in different units with MinimalUltrasonic.

## Available Units

MinimalUltrasonic supports 6 different units of measurement:

| Unit | Enum Value | Description | Use Case |
|------|------------|-------------|----------|
| Centimeters | `MinimalUltrasonic::CM` | Default unit | General purpose |
| Meters | `MinimalUltrasonic::METERS` | SI base unit | Scientific applications |
| Millimeters | `MinimalUltrasonic::MM` | High precision | Small measurements |
| Inches | `MinimalUltrasonic::INCHES` | Imperial unit | US applications |
| Yards | `MinimalUltrasonic::YARDS` | Imperial unit | Larger distances |
| Miles | `MinimalUltrasonic::MILES` | Imperial unit | Very large distances |

## Reading in Different Units

### Method 1: Specify Unit in read()

The simplest way is to pass the unit as a parameter:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read in different units
  float cm = sensor.read(MinimalUltrasonic::CM);
  float meters = sensor.read(MinimalUltrasonic::METERS);
  float mm = sensor.read(MinimalUltrasonic::MM);
  float inches = sensor.read(MinimalUltrasonic::INCHES);
  float yards = sensor.read(MinimalUltrasonic::YARDS);
  float miles = sensor.read(MinimalUltrasonic::MILES);
  
  // Display results
  Serial.print("CM: "); Serial.println(cm);
  Serial.print("Meters: "); Serial.println(meters, 4);
  Serial.print("MM: "); Serial.println(mm, 1);
  Serial.print("Inches: "); Serial.println(inches, 2);
  Serial.print("Yards: "); Serial.println(yards, 4);
  Serial.print("Miles: "); Serial.println(miles, 6);
  
  delay(2000);
}
```

### Method 2: Set Default Unit

Set a default unit and all subsequent reads use that unit:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  // Set default unit to meters
  sensor.setUnit(MinimalUltrasonic::METERS);
}

void loop() {
  // Now returns meters by default
  float distance = sensor.read();
  
  Serial.print("Distance: ");
  Serial.print(distance, 3);
  Serial.println(" m");
  
  delay(1000);
}
```

## Unit Conversion Reference

All conversions are calculated from the time-of-flight measurement:

### Conversion Factors

From centimeters:

```cpp
Meters = CM / 100
Millimeters = CM × 10
Inches = CM / 2.54
Yards = CM / 91.44
Miles = CM / 160934.4
```

### Example Conversions

| Distance | CM | Meters | MM | Inches | Yards |
|----------|-------|--------|-----|--------|-------|
| Close | 10 cm | 0.1 m | 100 mm | 3.94 in | 0.109 yd |
| Medium | 50 cm | 0.5 m | 500 mm | 19.69 in | 0.547 yd |
| Far | 200 cm | 2.0 m | 2000 mm | 78.74 in | 2.187 yd |

## Practical Examples

### Imperial System Application

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  sensor.setUnit(MinimalUltrasonic::INCHES);
}

void loop() {
  float inches = sensor.read();
  
  if (inches > 0) {
    int feet = inches / 12;
    float remainingInches = inches - (feet * 12);
    
    Serial.print(feet);
    Serial.print("' ");
    Serial.print(remainingInches, 1);
    Serial.println("\"");
  }
  
  delay(1000);
}
```

### Metric System Application

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  sensor.setUnit(MinimalUltrasonic::METERS);
}

void loop() {
  float meters = sensor.read();
  
  if (meters > 0) {
    if (meters < 1.0) {
      // Show in cm for small distances
      Serial.print(meters * 100, 1);
      Serial.println(" cm");
    } else {
      // Show in meters for larger distances
      Serial.print(meters, 2);
      Serial.println(" m");
    }
  }
  
  delay(1000);
}
```

### High Precision Measurement

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  sensor.setUnit(MinimalUltrasonic::MM);
}

void loop() {
  float mm = sensor.read();
  
  if (mm > 0) {
    Serial.print("Distance: ");
    Serial.print(mm, 2);
    Serial.println(" mm");
    
    // Precision indicator
    if (mm < 20) {
      Serial.println("⚠️ Too close for accurate reading");
    } else {
      Serial.println("✓ Valid measurement");
    }
  }
  
  delay(1000);
}
```

## Switching Units Dynamically

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);
int unitIndex = 0;

MinimalUltrasonic::Unit units[] = {
  MinimalUltrasonic::CM,
  MinimalUltrasonic::METERS,
  MinimalUltrasonic::INCHES
};

String unitNames[] = {"cm", "m", "in"};

void setup() {
  Serial.begin(9600);
  Serial.println("Press any key to switch units");
}

void loop() {
  // Switch unit when serial data received
  if (Serial.available() > 0) {
    Serial.read();
    unitIndex = (unitIndex + 1) % 3;
    sensor.setUnit(units[unitIndex]);
    Serial.print("Switched to: ");
    Serial.println(unitNames[unitIndex]);
  }
  
  float distance = sensor.read();
  
  if (distance > 0) {
    Serial.print(distance, 2);
    Serial.print(" ");
    Serial.println(unitNames[unitIndex]);
  }
  
  delay(500);
}
```

## Query Current Unit

Check what unit is currently set:

```cpp
MinimalUltrasonic::Unit currentUnit = sensor.getUnit();

if (currentUnit == MinimalUltrasonic::CM) {
  Serial.println("Using centimeters");
} else if (currentUnit == MinimalUltrasonic::METERS) {
  Serial.println("Using meters");
} else if (currentUnit == MinimalUltrasonic::INCHES) {
  Serial.println("Using inches");
}
```

## Legacy Compatibility

For backward compatibility with v1.x, you can still use:

```cpp
// Old style (still works)
float cm = sensor.read(CM);
float inches = sensor.read(INC);

// New style (recommended)
float cm = sensor.read(MinimalUltrasonic::CM);
float inches = sensor.read(MinimalUltrasonic::INCHES);
```

## Best Practices

### Choose the Right Unit

- **CM** - Default, good for most applications (2-400 cm range)
- **METERS** - Scientific work, longer distances
- **MM** - Precision work, small measurements
- **INCHES** - US market, consumer applications
- **YARDS** - Larger outdoor measurements
- **MILES** - Very specialized, demonstration purposes

### Decimal Places

Use appropriate decimal places for each unit:

```cpp
Serial.println(sensor.read(MinimalUltrasonic::CM), 1);      // 1 decimal
Serial.println(sensor.read(MinimalUltrasonic::METERS), 3);  // 3 decimals
Serial.println(sensor.read(MinimalUltrasonic::MM), 0);      // 0 decimals
Serial.println(sensor.read(MinimalUltrasonic::INCHES), 2);  // 2 decimals
```

### Unit Consistency

Keep units consistent within your application:

```cpp
// ✓ Good - consistent unit
sensor.setUnit(MinimalUltrasonic::METERS);
float dist1 = sensor.read();
float dist2 = sensor.read();
float average = (dist1 + dist2) / 2;

// ✗ Bad - mixing units
float cm = sensor.read(MinimalUltrasonic::CM);
float meters = sensor.read(MinimalUltrasonic::METERS);
float sum = cm + meters;  // Wrong! Different units
```

## Next Steps

- [Timeout Configuration](/guide/timeout-configuration) - Adjust maximum range
- [Error Handling](/guide/error-handling) - Handle measurement errors
- [Examples](/examples/all-units) - See complete working examples

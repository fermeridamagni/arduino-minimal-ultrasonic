# Multiple Sensors Example

Use multiple ultrasonic sensors simultaneously for comprehensive coverage.

## Overview

This example demonstrates how to use multiple MinimalUltrasonic sensors together for:

- Multi-directional obstacle detection
- Robot navigation
- Security perimeter monitoring
- Parking assistance systems

## Hardware Required

- Arduino Uno (or Arduino Mega for more sensors)
- 3-4 HC-SR04 Ultrasonic Sensors
- Jumper wires
- Breadboard

## Circuit - 4 Sensors

```txt
Sensor 1 (Front)     Arduino
----------------     -------
VCC              ->  5V
GND              ->  GND
TRIG             ->  Pin 12
ECHO             ->  Pin 13

Sensor 2 (Back)      Arduino
---------------      -------
VCC              ->  5V
GND              ->  GND
TRIG             ->  Pin 10
ECHO             ->  Pin 11

Sensor 3 (Left)      Arduino
---------------      -------
VCC              ->  5V
GND              ->  GND
TRIG             ->  Pin 8
ECHO             ->  Pin 9

Sensor 4 (Right)     Arduino
----------------     -------
VCC              ->  5V
GND              ->  GND
TRIG             ->  Pin 6
ECHO             ->  Pin 7
```

## Basic Multi-Sensor Example

```cpp
/**
 * Multiple Sensors Example
 * 
 * Demonstrates using 4 ultrasonic sensors
 * for complete 360° coverage.
 */

#include <MinimalUltrasonic.h>

// Create four sensor instances
MinimalUltrasonic frontSensor(12, 13);
MinimalUltrasonic backSensor(10, 11);
MinimalUltrasonic leftSensor(8, 9);
MinimalUltrasonic rightSensor(6, 7);

void setup() {
  Serial.begin(9600);
  
  Serial.println("Multiple Sensors Example");
  Serial.println("========================");
  Serial.println();
}

void loop() {
  // Read each sensor with delays to prevent interference
  float front = frontSensor.read();
  delay(50);  // Important: prevent crosstalk
  
  float back = backSensor.read();
  delay(50);
  
  float left = leftSensor.read();
  delay(50);
  
  float right = rightSensor.read();
  delay(50);
  
  // Display readings
  Serial.print("F:");
  Serial.print(front, 1);
  Serial.print(" | B:");
  Serial.print(back, 1);
  Serial.print(" | L:");
  Serial.print(left, 1);
  Serial.print(" | R:");
  Serial.println(right, 1);
  
  delay(200);
}
```

## Robot Navigation Example

Complete obstacle avoidance system:

```cpp
#include <MinimalUltrasonic.h>

// Sensors for all directions
MinimalUltrasonic front(12, 13);
MinimalUltrasonic back(10, 11);
MinimalUltrasonic left(8, 9);
MinimalUltrasonic right(6, 7);

// Safety thresholds
const float DANGER_ZONE = 20.0;   // 20cm - STOP
const float WARNING_ZONE = 50.0;  // 50cm - Caution

void setup() {
  Serial.begin(9600);
  
  Serial.println("Robot Navigation System");
  Serial.println("======================");
  Serial.println();
  
  // Configure all sensors
  front.setTimeout(15000UL);
  back.setTimeout(15000UL);
  left.setTimeout(15000UL);
  right.setTimeout(15000UL);
}

void loop() {
  // Read all sensors
  float f = front.read();
  delay(30);
  float b = back.read();
  delay(30);
  float l = left.read();
  delay(30);
  float r = right.read();
  delay(30);
  
  // Decision making
  Serial.print("[FRONT] ");
  if (f > 0 && f < DANGER_ZONE) {
    Serial.print("🛑 STOP! ");
    Serial.print(f);
    Serial.println(" cm - TOO CLOSE!");
  } else if (f > 0 && f < WARNING_ZONE) {
    Serial.print("⚠️  SLOW ");
    Serial.print(f);
    Serial.println(" cm");
  } else if (f > 0) {
    Serial.print("✓ CLEAR ");
    Serial.print(f);
    Serial.println(" cm");
  } else {
    Serial.println("No object");
  }
  
  // Check other directions
  if (l > 0 && l < WARNING_ZONE) {
    Serial.print("[LEFT]  ⚠️  ");
    Serial.print(l);
    Serial.println(" cm");
  }
  
  if (r > 0 && r < WARNING_ZONE) {
    Serial.print("[RIGHT] ⚠️  ");
    Serial.print(r);
    Serial.println(" cm");
  }
  
  if (b > 0 && b < WARNING_ZONE) {
    Serial.print("[BACK]  ⚠️  ");
    Serial.print(b);
    Serial.println(" cm");
  }
  
  Serial.println();
  delay(200);
}
```

## Parking Sensor Example

Multi-zone parking assistance:

```cpp
#include <MinimalUltrasonic.h>

// Front and rear sensors
MinimalUltrasonic frontLeft(12, 13);
MinimalUltrasonic frontRight(10, 11);
MinimalUltrasonic rearLeft(8, 9);
MinimalUltrasonic rearRight(6, 7);

// Distance zones (in cm)
const float ZONE_RED = 30.0;     // Danger zone
const float ZONE_YELLOW = 60.0;  // Warning zone
const float ZONE_GREEN = 100.0;  // Safe zone

void printZone(const char* name, float distance) {
  Serial.print(name);
  Serial.print(": ");
  
  if (distance == 0) {
    Serial.println("---");
  } else if (distance < ZONE_RED) {
    Serial.print("🔴 ");
    Serial.print(distance, 0);
    Serial.println(" cm - STOP!");
  } else if (distance < ZONE_YELLOW) {
    Serial.print("🟡 ");
    Serial.print(distance, 0);
    Serial.println(" cm - SLOW");
  } else if (distance < ZONE_GREEN) {
    Serial.print("🟢 ");
    Serial.print(distance, 0);
    Serial.println(" cm - OK");
  } else {
    Serial.print("   ");
    Serial.print(distance, 0);
    Serial.println(" cm - Clear");
  }
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Parking Sensor System");
  Serial.println("====================");
  Serial.println();
}

void loop() {
  // Read all sensors
  float fl = frontLeft.read();
  delay(40);
  float fr = frontRight.read();
  delay(40);
  float rl = rearLeft.read();
  delay(40);
  float rr = rearRight.read();
  delay(40);
  
  // Display zones
  Serial.println("FRONT:");
  printZone("  Left ", fl);
  printZone("  Right", fr);
  
  Serial.println("REAR:");
  printZone("  Left ", rl);
  printZone("  Right", rr);
  
  Serial.println("--------------------");
  delay(300);
}
```

## Sensor Array Example

Manage many sensors with arrays:

```cpp
#include <MinimalUltrasonic.h>

// Sensor configuration structure
struct SensorConfig {
  uint8_t trigPin;
  uint8_t echoPin;
  const char* name;
};

// Define sensor array
const int SENSOR_COUNT = 4;
SensorConfig configs[SENSOR_COUNT] = {
  {12, 13, "Front"},
  {10, 11, "Back"},
  {8, 9, "Left"},
  {6, 7, "Right"}
};

// Create sensor objects
MinimalUltrasonic* sensors[SENSOR_COUNT];

void setup() {
  Serial.begin(9600);
  
  Serial.println("Sensor Array Example");
  Serial.println("====================");
  Serial.println();
  
  // Initialize all sensors
  for (int i = 0; i < SENSOR_COUNT; i++) {
    sensors[i] = new MinimalUltrasonic(
      configs[i].trigPin,
      configs[i].echoPin
    );
    
    Serial.print("Initialized: ");
    Serial.println(configs[i].name);
  }
  
  Serial.println();
}

void loop() {
  Serial.println("Readings:");
  
  // Read all sensors
  for (int i = 0; i < SENSOR_COUNT; i++) {
    float distance = sensors[i]->read();
    
    Serial.print("  ");
    Serial.print(configs[i].name);
    Serial.print(": ");
    
    if (distance > 0) {
      Serial.print(distance, 1);
      Serial.println(" cm");
    } else {
      Serial.println("No reading");
    }
    
    delay(40);  // Prevent interference
  }
  
  Serial.println();
  delay(500);
}
```

## Closest Object Detection

Find the nearest obstacle:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensors[] = {
  MinimalUltrasonic(12, 13),
  MinimalUltrasonic(10, 11),
  MinimalUltrasonic(8, 9),
  MinimalUltrasonic(6, 7)
};

const char* names[] = {"Front", "Back", "Left", "Right"};
const int SENSOR_COUNT = 4;

void findClosest() {
  float minDistance = 999999.0;
  int closestSensor = -1;
  
  // Read all sensors and find minimum
  for (int i = 0; i < SENSOR_COUNT; i++) {
    float distance = sensors[i].read();
    
    if (distance > 0 && distance < minDistance) {
      minDistance = distance;
      closestSensor = i;
    }
    
    delay(40);
  }
  
  // Report closest
  if (closestSensor >= 0) {
    Serial.print("Closest object: ");
    Serial.print(names[closestSensor]);
    Serial.print(" - ");
    Serial.print(minDistance, 1);
    Serial.println(" cm");
  } else {
    Serial.println("No objects detected");
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Closest Object Detection");
  Serial.println("=======================");
  Serial.println();
}

void loop() {
  findClosest();
  delay(500);
}
```

## Security Perimeter Example

Monitor a protected area:

```cpp
#include <MinimalUltrasonic.h>

// Perimeter sensors
MinimalUltrasonic north(12, 13);
MinimalUltrasonic south(10, 11);
MinimalUltrasonic east(8, 9);
MinimalUltrasonic west(6, 7);

const float PERIMETER_DISTANCE = 200.0;  // 2 meters
const int LED_PIN = LED_BUILTIN;

bool systemArmed = true;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("Security Perimeter System");
  Serial.println("========================");
  Serial.println();
  
  // Configure for long range
  north.setTimeout(30000UL);
  south.setTimeout(30000UL);
  east.setTimeout(30000UL);
  west.setTimeout(30000UL);
  
  Serial.println("System ARMED");
  Serial.println("Perimeter: 200cm");
  Serial.println();
}

void loop() {
  if (!systemArmed) {
    delay(1000);
    return;
  }
  
  // Check all perimeter sensors
  bool breach = false;
  
  float n = north.read();
  delay(40);
  if (n > 0 && n < PERIMETER_DISTANCE) {
    Serial.print("⚠️  BREACH - NORTH: ");
    Serial.print(n);
    Serial.println(" cm");
    breach = true;
  }
  
  float s = south.read();
  delay(40);
  if (s > 0 && s < PERIMETER_DISTANCE) {
    Serial.print("⚠️  BREACH - SOUTH: ");
    Serial.print(s);
    Serial.println(" cm");
    breach = true;
  }
  
  float e = east.read();
  delay(40);
  if (e > 0 && e < PERIMETER_DISTANCE) {
    Serial.print("⚠️  BREACH - EAST: ");
    Serial.print(e);
    Serial.println(" cm");
    breach = true;
  }
  
  float w = west.read();
  delay(40);
  if (w > 0 && w < PERIMETER_DISTANCE) {
    Serial.print("⚠️  BREACH - WEST: ");
    Serial.print(w);
    Serial.println(" cm");
    breach = true;
  }
  
  // Alarm
  if (breach) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("✓ Perimeter secure");
  }
  
  delay(200);
}
```

## Important Considerations

### 1. Prevent Crosstalk

Always add delays between sensor readings:

```cpp
// BAD: No delays - sensors interfere
float d1 = sensor1.read();
float d2 = sensor2.read();
float d3 = sensor3.read();

// GOOD: Delays prevent interference
float d1 = sensor1.read();
delay(40);
float d2 = sensor2.read();
delay(40);
float d3 = sensor3.read();
```

### 2. Power Considerations

For 3+ sensors, use external power:

```cpp
// External 5V power supply
// Connect all GNDs together:
// - Arduino GND
// - External supply GND
// - All sensor GNDs
```

### 3. Pin Management

Track which pins are used:

```cpp
// Document your pin usage
// Pin 12-13: Front sensor
// Pin 10-11: Back sensor
// Pin 8-9:   Left sensor
// Pin 6-7:   Right sensor
```

## Timing Strategies

### Sequential Reading (Reliable)

```cpp
void sequentialRead() {
  float d1 = sensor1.read();
  delay(50);  // Wait between sensors
  float d2 = sensor2.read();
  delay(50);
  float d3 = sensor3.read();
  // Total time: ~150-200ms
}
```

### Round-Robin (Balanced)

```cpp
int currentSensor = 0;
const int TOTAL_SENSORS = 4;

void roundRobinRead() {
  float distance = sensors[currentSensor].read();
  
  Serial.print("Sensor ");
  Serial.print(currentSensor);
  Serial.print(": ");
  Serial.println(distance);
  
  currentSensor = (currentSensor + 1) % TOTAL_SENSORS;
  delay(100);
}
```

### Priority-Based (Smart)

```cpp
void priorityRead() {
  // Read critical sensor more frequently
  float front = frontSensor.read();
  handleFront(front);
  delay(30);
  
  // Other sensors less frequently
  static int counter = 0;
  if (counter % 3 == 0) {
    float back = backSensor.read();
    handleBack(back);
  }
  
  counter++;
  delay(100);
}
```

## Troubleshooting Multiple Sensors

### All Sensors Return 0

**Cause:** Insufficient power

**Solution:** Use external 5V power supply

### Erratic Readings

**Cause:** Crosstalk between sensors

**Solution:** Increase delays or separate physically

### Some Sensors Don't Work

**Cause:** Pin conflicts or wiring

**Solution:** Test each sensor individually first

## Best Practices

1. **Add delays** - 30-50ms between sensor readings
2. **Test individually** - Verify each sensor works alone
3. **Use external power** - For 3+ sensors
4. **Document pins** - Keep track of connections
5. **Consider interference** - Mount sensors appropriately
6. **Validate all readings** - Always check for 0

## Next Steps

- **[Advanced Example](/examples/advanced)** - Error handling and filtering
- **[Multiple Sensors Guide](/guide/multiple-sensors)** - Detailed guide
- **[Troubleshooting](/guide/troubleshooting)** - Solve issues

## See Also

- [Multiple Sensors Guide](/guide/multiple-sensors)
- [Error Handling](/guide/error-handling)
- [Best Practices](/guide/best-practices)

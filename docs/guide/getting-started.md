# Getting Started

Get up and running with MinimalUltrasonic in just a few minutes!

## Prerequisites

Before you begin, make sure you have:

- Arduino IDE (1.6.0 or later) or PlatformIO
- An ultrasonic sensor (HC-SR04, Ping, or Seeed)
- An Arduino board (Uno, Mega, Nano, etc.)
- Basic knowledge of Arduino programming

## Installation

### Method 1: Arduino Library Manager (Recommended)

1. Open Arduino IDE
2. Go to `Sketch` → `Include Library` → `Manage Libraries...`
3. Search for "MinimalUltrasonic"
4. Click "Install"

### Method 2: Manual Installation

1. Download the [latest release](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/releases)
2. Extract the ZIP file
3. Move to Arduino libraries folder:
   - Windows: `Documents\Arduino\libraries\`
   - Mac: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
4. Restart Arduino IDE

See [Installation Guide](/guide/installation) for detailed instructions.

## Hardware Setup

### For HC-SR04 (4-pin sensor)

```txt
HC-SR04      Arduino
-------------------------
VCC     →    5V
Trig    →    Pin 12
Echo    →    Pin 13
GND     →    GND
```

### For Ping))) or Seeed (3-pin sensor)

```txt
Sensor       Arduino
-------------------------
VCC     →    5V
SIG     →    Pin 13
GND     →    GND
```

::: warning Power Supply
Always use a stable 5V power supply. Insufficient power can cause erratic readings.
:::

## Your First Sketch

Create a new sketch and copy this code:

```cpp
#include <MinimalUltrasonic.h>

// For HC-SR04 (4-pin): specify trigger and echo pins
MinimalUltrasonic sensor(12, 13);

// For Ping/Seeed (3-pin): specify only signal pin
// MinimalUltrasonic sensor(13);

void setup() {
  Serial.begin(9600);
  Serial.println("MinimalUltrasonic - Getting Started");
  Serial.println("===================================");
}

void loop() {
  // Read distance in centimeters (default)
  float distance = sensor.read();
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Wait 1 second before next measurement
  delay(1000);
}
```

### Upload and Test

1. Connect your Arduino to computer
2. Select correct board: `Tools` → `Board`
3. Select correct port: `Tools` → `Port`
4. Click Upload
5. Open Serial Monitor (`Tools` → `Serial Monitor`)
6. Set baud rate to 9600

You should see distance measurements updating every second!

## Understanding the Code

Let's break down what's happening:

### 1. Include the Library

```cpp
#include <MinimalUltrasonic.h>
```

This makes the MinimalUltrasonic library available to your sketch.

### 2. Create Sensor Object

```cpp
MinimalUltrasonic sensor(12, 13);
```

Creates a sensor object for a 4-pin sensor with:

- Trigger pin: 12
- Echo pin: 13

For 3-pin sensors:

```cpp
MinimalUltrasonic sensor(13);
```

### 3. Initialize Serial

```cpp
void setup() {
  Serial.begin(9600);
}
```

Sets up Serial communication at 9600 baud.

### 4. Read Distance

```cpp
float distance = sensor.read();
```

Reads the distance and returns it as a float in centimeters (default unit).

## Measuring in Different Units

Want to measure in inches or meters? Just specify the unit:

```cpp
// Centimeters (default)
float cm = sensor.read(MinimalUltrasonic::CM);

// Meters
float m = sensor.read(MinimalUltrasonic::METERS);

// Inches
float inches = sensor.read(MinimalUltrasonic::INCHES);

// Millimeters
float mm = sensor.read(MinimalUltrasonic::MM);

// Yards
float yards = sensor.read(MinimalUltrasonic::YARDS);

// Miles (why not? 😄)
float miles = sensor.read(MinimalUltrasonic::MILES);
```

Learn more in the [Multiple Units Guide](/guide/units).

## Common Issues

### Reading Always Returns 0

**Possible causes:**

- Incorrect wiring
- Insufficient power supply
- No object in range
- Timeout too short

**Solution:**

1. Double-check wiring connections
2. Verify 5V power supply is stable
3. Place an object in front of sensor
4. Increase timeout: `sensor.setTimeout(40000UL);`

### Erratic Readings

**Possible causes:**

- Electrical interference
- Unstable power supply
- Object too close or too far

**Solution:**

1. Add 100µF capacitor near sensor VCC/GND
2. Use regulated power supply
3. Ensure object is 2cm-400cm away
4. Add delay between readings

See [Troubleshooting Guide](/guide/troubleshooting) for more help.

## Next Steps

Now that you have a working setup, explore more features:

- **[Basic Usage](/guide/basic-usage)** - Learn essential methods
- **[Multiple Units](/guide/units)** - Work with different measurements
- **[Timeout Configuration](/guide/timeout)** - Adjust maximum range
- **[Examples](/examples/basic)** - See working code samples

## Quick Reference Card

```cpp
// Include library
#include <MinimalUltrasonic.h>

// Create sensor
MinimalUltrasonic sensor(trig, echo);      // 4-pin
MinimalUltrasonic sensor(sig);             // 3-pin

// Read distance
float dist = sensor.read();                // cm (default)
float dist = sensor.read(Unit);            // specific unit

// Configure
sensor.setTimeout(30000UL);                // Set timeout
sensor.setMaxDistance(500);                // Set max distance
sensor.setUnit(MinimalUltrasonic::METERS); // Set default unit

// Query
unsigned long timeout = sensor.getTimeout();
Unit unit = sensor.getUnit();
```

::: tip Pro Tip
Keep the sensor mounted firmly and pointing straight at your target for most accurate readings!
:::

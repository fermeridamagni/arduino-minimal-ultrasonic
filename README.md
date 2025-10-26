# MinimalUltrasonic

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue.svg)](https://www.arduino.cc/reference/en/libraries/minimalultrasonic/)
[![Version](https://img.shields.io/badge/version-2.0.0-green.svg)](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/releases)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

> A minimalist, efficient Arduino library for ultrasonic distance sensors with comprehensive unit support.

**Compatible with HC-SR04, Ping, and Seeed SEN136B5B sensors**

## ✨ Features

- 🎯 **Multiple Units** - Measure in cm, meters, mm, inches, yards, and miles
- ⚡ **Resource Efficient** - Only 8 bytes per sensor instance, ~940 bytes flash
- 🔧 **Flexible Configuration** - Support for both 3-pin and 4-pin sensors
- ⏱️ **Configurable Timeout** - Control maximum detection range
- 📦 **Multiple Sensors** - Use multiple sensors simultaneously without conflicts
- 📝 **Comprehensive Documentation** - Full VitePress documentation site with guides and examples
- 🔄 **Backward Compatible** - Seamless migration from v1.x
- 🎨 **Modern C++** - Clean API with type-safe enums and const correctness

## 📖 Documentation

**[📚 Full Documentation Site](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/)**

Quick links:

- [Getting Started Guide](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/getting-started)
- [API Reference](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/api/reference)
- [Examples](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/examples/basic)
- [Migration Guide (v1.x → v2.0)](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/migration)
- [Troubleshooting](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/troubleshooting)

## 🎮 Try it Online

Run examples on **Wokwi Arduino Simulator** (free, browser-based):

- [Basic 3-pin and 4-pin sensor usage](https://wokwi.com/arduino/projects/296099969420493322)
- [Multiple units demonstration](https://wokwi.com/arduino/projects/312346565007114818)

## 🔌 Wiring

### HC-SR04 (4-pin sensor)

```txt
HC-SR04      Arduino
-------------------------
VCC     ->   5V
Trig    ->   Pin 12
Echo    ->   Pin 13
GND     ->   GND
```

![HC-SR04 with Arduino](assets/HC-SR04-with-Arduino.jpg?raw=true "HC-SR04 with Arduino")

### Ping or Seeed SEN136B5B (3-pin sensor)

```txt
Sensor       Arduino
-------------------------
VCC     ->   5V
SIG     ->   Pin 13
GND     ->   GND
```

## 📦 Installation

### Method 1: Arduino Library Manager (Recommended)

1. Open Arduino IDE
2. Go to `Sketch > Include Library > Manage Libraries...`
3. Search for: `MinimalUltrasonic`
4. Find `Minimal Ultrasonic by fermeridamagni`
5. Click `Install`

### Method 2: Manual Installation

1. Download the [latest release](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/archive/master.zip)
2. In Arduino IDE: `Sketch > Include Library > Add .ZIP Library...`
3. Select the downloaded file

## 🚀 Quick Start

### Basic Distance Measurement

```cpp
#include <MinimalUltrasonic.h>

// For HC-SR04 (4-pin): specify trigger and echo pins
MinimalUltrasonic sensor(12, 13);

// For Ping))) or Seeed (3-pin): specify only signal pin
// MinimalUltrasonic sensor(13);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float distance = sensor.read();  // Returns distance in cm (default)
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(1000);
}
```

### Multiple Units Support

```cpp
// Specify unit directly in read() call
float cm = sensor.read(MinimalUltrasonic::CM);        // Centimeters (default)
float meters = sensor.read(MinimalUltrasonic::METERS);  // Meters
float mm = sensor.read(MinimalUltrasonic::MM);        // Millimeters
float inches = sensor.read(MinimalUltrasonic::INCHES);  // Inches
float yards = sensor.read(MinimalUltrasonic::YARDS);   // Yards
float miles = sensor.read(MinimalUltrasonic::MILES);   // Miles

// Or set default unit
sensor.setUnit(MinimalUltrasonic::METERS);
float distance = sensor.read();  // Now returns meters
```

### Configuration Options

```cpp
// Configure timeout (max range)
sensor.setTimeout(40000UL);      // Timeout in microseconds (~6.8m max)
sensor.setMaxDistance(680);      // Or set by distance in cm

// Query current settings
unsigned long timeout = sensor.getTimeout();
MinimalUltrasonic::Unit unit = sensor.getUnit();
```

## 📚 Complete API Reference

### Constructors

```cpp
// 3-pin sensor (Ping, Seeed)
MinimalUltrasonic(uint8_t sigPin, unsigned long timeOut = 20000UL)

// 4-pin sensor (HC-SR04)
MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut = 20000UL)
```

### Core Methods

| Method | Description | Return Type |
|--------|-------------|-------------|
| `read(Unit unit)` | Read distance in specified unit | `float` |
| `setTimeout(unsigned long)` | Set timeout in microseconds | `void` |
| `setMaxDistance(unsigned int)` | Set max distance in cm | `void` |
| `getTimeout()` | Get current timeout | `unsigned long` |
| `setUnit(Unit)` | Set default unit | `void` |
| `getUnit()` | Get current unit | `Unit` |

### Available Units

| Unit Enum | Description | Typical Use |
|-----------|-------------|-------------|
| `MinimalUltrasonic::CM` | Centimeters | General purpose (default) |
| `MinimalUltrasonic::METERS` | Meters | Long distances |
| `MinimalUltrasonic::MM` | Millimeters | High precision |
| `MinimalUltrasonic::INCHES` | Inches | Imperial system |
| `MinimalUltrasonic::YARDS` | Yards | Imperial system |
| `MinimalUltrasonic::MILES` | Miles | Very long distances |

**[See full API documentation →](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/api/reference)**

## 💡 Examples

The library includes comprehensive examples:

1. **UltrasonicSimple** - Basic distance measurement
2. **Timeout** - Configuring timeout for different ranges
3. **MultipleUltrasonicSensors** - Using multiple sensors simultaneously
4. **AllUnits** - Demonstrating all 6 measurement units

Access in Arduino IDE: `File > Examples > MinimalUltrasonic`

**[View all examples online →](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/examples/basic)**

## 🎯 Usage Guides

- **[Getting Started](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/getting-started)** - First-time setup
- **[Basic Usage](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/basic-usage)** - Essential patterns
- **[Multiple Units](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/multiple-units)** - Working with different measurements
- **[Timeout Configuration](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/timeout-configuration)** - Adjusting range
- **[Multiple Sensors](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/multiple-sensors)** - Managing sensor arrays
- **[Error Handling](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/error-handling)** - Robust error management
- **[Best Practices](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/best-practices)** - Tips and techniques
- **[Optimization](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/optimization)** - Performance tuning
- **[Troubleshooting](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/troubleshooting)** - Common issues and solutions

## 🔬 Technical Documentation

- **[Architecture](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/technical/architecture)** - Library design and implementation
- **[Physics Background](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/technical/physics)** - How ultrasonic sensing works
- **[Unit Conversions](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/technical/conversions)** - Conversion mathematics
- **[Performance](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/technical/performance)** - Benchmarks and optimization
- **[Compatibility](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/technical/compatibility)** - Board and sensor compatibility

## ⚡ Performance Characteristics

- **Memory**: 8 bytes RAM per sensor instance
- **Flash**: ~940 bytes
- **Speed**: 6.1ms measurement @ 100cm
- **Max Rate**: 16 Hz safe (mixed range), up to 164 Hz @ 100cm
- **Accuracy**: ±3mm (sensor dependent)
- **Range**: 2cm to 400cm (HC-SR04)
- **Power**: <15mA peak, ~2mA idle

## 🔧 Hardware Compatibility

### Arduino Boards

| Board | Status | Notes |
|-------|--------|-------|
| Arduino Uno | ✅ Tested | Fully compatible |
| Arduino Nano | ✅ Tested | Fully compatible |
| Arduino Mega | ✅ Tested | Fully compatible |
| Arduino Leonardo | ✅ Compatible | Works perfectly |
| ESP32 | ⚠️ Level shifter required | 3.3V logic |
| ESP8266 | ⚠️ Level shifter required | 3.3V logic |
| Arduino Due | ⚠️ Level shifter required | 3.3V logic |

**[See full compatibility guide →](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/technical/compatibility)**

### Ultrasonic Sensors

| Sensor | Pins | Voltage | Range | Status |
|--------|------|---------|-------|--------|
| HC-SR04 | 4-pin | 5V | 2-400cm | ✅ Tested |
| Ping))) | 3-pin | 5V | 3-300cm | ✅ Tested |
| Seeed SEN136B5B | 3-pin | 3.3-5V | 3-400cm | ✅ Compatible |
| HY-SRF05 | 4-pin | 5V | 2-450cm | ✅ Compatible |
| US-100 | 4-pin | 5V | 2-450cm | ✅ Compatible |

## 🐛 Troubleshooting

### Reading always returns 0

**Causes:**

- Incorrect wiring
- Insufficient power supply
- No object in range
- Timeout too short

**Solutions:**

```cpp
// Increase timeout
sensor.setTimeout(40000UL);

// Or set max distance
sensor.setMaxDistance(680);  // 6.8 meters

// Check wiring and power supply
```

### Unstable/erratic readings

**Causes:**

- Electrical interference
- Unstable power
- Object too close/far or angled

**Solutions:**

```cpp
// Add delay between readings
delay(30);  // Minimum 30ms recommended

// Use filtering
float filtered = (lastReading * 0.7) + (newReading * 0.3);

// Check sensor mounting and object position
```

### Won't compile

**Solutions:**

- Verify library is installed: `Sketch > Include Library > Manage Libraries`
- Check for typos in unit enum: `MinimalUltrasonic::CM` (not just `CM`)
- Ensure `#include <MinimalUltrasonic.h>` is at top of sketch

**[Full troubleshooting guide →](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/troubleshooting)**

## � What's New in v2.0

### Added

- ✨ 6 measurement units (cm, m, mm, in, yd, mi)
- 🎯 Type-safe `Unit` enum
- 📝 Comprehensive VitePress documentation site
- 🔧 `setUnit()` and `getUnit()` methods
- 📊 `setMaxDistance()` for easier range configuration
- 💡 Advanced examples with filtering and statistics

### Changed

- ⚡ Optimized memory layout (12 → 8 bytes per instance)
- 🔄 Improved timeout handling
- 📖 Enhanced inline documentation with Doxygen
- 🎨 Modern C++ patterns (const correctness, delegation)

### Migration from v1.x

Most code works without changes! Key differences:

```cpp
// v1.x - still works
float distance = sensor.read();

// v2.0 - new features
float meters = sensor.read(MinimalUltrasonic::METERS);
sensor.setUnit(MinimalUltrasonic::INCHES);
```

**[Complete migration guide →](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/guide/migration)**

## 📄 License

MIT License - Copyright © 2025 [fermeridamagni](http://magni.dev) (Magni Development)

Based on [the work of Erick Simões](https://github.com/ErickSimoes/Ultrasonic).

See [LICENSE](LICENSE) for full details.

## 🤝 Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for:

- Code of Conduct
- Development setup
- Coding standards
- Pull request process

## 📞 Support & Community

- 📖 **Documentation**: [fermeridamagni.github.io/arduino-minimal-ultrasonic](https://fermeridamagni.github.io/arduino-minimal-ultrasonic/)
- 🐛 **Issues**: [GitHub Issues](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/issues)
- 💬 **Discussions**: [GitHub Discussions](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/discussions)
- 📧 **Email**: <hello@magni.dev>

## ⭐ Show Your Support

If you find this library useful, please:

- ⭐ Star this repository
- 🐛 Report bugs and suggest features
- 📢 Share with the Arduino community
- 💝 Consider [sponsoring](https://github.com/sponsors/fermeridamagni)

---

**Made with ❤️ by [Magni Development](http://magni.dev)**

*Building better tools for the maker community.*

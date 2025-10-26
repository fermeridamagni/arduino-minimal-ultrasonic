---
layout: home

hero:
  name: "MinimalUltrasonic"
  text: "Ultrasonic Distance Sensing Made Simple"
  tagline: A minimalist Arduino library for HC-SR04, Ping, and Seeed sensors with multiple unit support
  image:
    src: /hero-image.png
    alt: MinimalUltrasonic
  actions:
    - theme: brand
      text: Get Started
      link: /guide/getting-started
    - theme: alt
      text: View on GitHub
      link: https://github.com/fermeridamagni/arduino-minimal-ultrasonic
    - theme: alt
      text: API Reference
      link: /api/reference

features:
  - icon: 🎯
    title: Multiple Units
    details: Measure distance in centimeters, meters, millimeters, inches, yards, or miles. Flexible unit selection for any project.
  
  - icon: ⚡
    title: Resource Efficient
    details: Minimal memory footprint (~8 bytes per instance) and optimized code execution. Perfect for memory-constrained projects.
  
  - icon: 🔧
    title: Flexible Configuration
    details: Support for both 3-pin (Ping, Seeed) and 4-pin (HC-SR04) sensors. Configurable timeout for range control.
  
  - icon: 📦
    title: Multiple Sensors
    details: Use multiple sensors simultaneously without conflicts. Each sensor can have its own configuration.
  
  - icon: 📝
    title: Well Documented
    details: Comprehensive inline documentation, API reference, technical guides, and working examples for all features.
  
  - icon: 🔄
    title: Backward Compatible
    details: Works with existing v1.x code with minimal changes. Legacy constants CM and INC still supported.
  
  - icon: 🎓
    title: Easy to Use
    details: Simple, intuitive API. Get started with just 3 lines of code. Extensive examples included.
  
  - icon: 🚀
    title: High Performance
    details: Optimized timing routines with accurate physics-based calculations. No unnecessary delays.
  
  - icon: 🛠️
    title: Production Ready
    details: Tested with Arduino Uno, Mega, Nano, ESP32, and more. Professional-grade code quality.
---

## Quick Start

Install the library and start measuring distance in minutes:

```cpp
#include <MinimalUltrasonic.h>

// For HC-SR04 (4-pin)
MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float distance = sensor.read();  // Distance in cm
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);
}
```

## Multiple Units Support

Measure in any unit you need:

```cpp
float cm = sensor.read(MinimalUltrasonic::CM);
float meters = sensor.read(MinimalUltrasonic::METERS);
float inches = sensor.read(MinimalUltrasonic::INCHES);
float yards = sensor.read(MinimalUltrasonic::YARDS);
```

## Compatible Sensors

- **HC-SR04** - Most common 4-pin ultrasonic sensor
- **HC-SR04+** - 3.3V/5V compatible variant
- **Ping)))** - 3-pin sensor from Parallax
- **Seeed SEN136B5B** - 3-pin Grove sensor
- **And more!** - Compatible with most ultrasonic sensors

## Key Features at a Glance

| Feature | Description |
|---------|-------------|
| **Units** | 6 units: cm, m, mm, inches, yards, miles |
| **Sensors** | 3-pin and 4-pin configurations |
| **Timeout** | Configurable (default: 20ms ≈ 3.4m range) |
| **Memory** | 8 bytes per sensor instance |
| **Code Size** | ~1.4 KB compiled |
| **Return Type** | Float for precision |
| **Accuracy** | ±3mm (sensor dependent) |

## Why MinimalUltrasonic?

::: tip Minimalist Philosophy
Other libraries include unnecessary features that bloat your code. MinimalUltrasonic focuses on what matters: accurate distance measurement with minimal overhead.
:::

::: info Modern Design
Built with modern C++ practices including type-safe enums, const correctness, and comprehensive documentation. Version 2.0 adds multiple units while maintaining the minimalist approach.
:::

::: warning Production Ready
Used in real-world projects. Well-tested across multiple Arduino platforms. Professional-grade code with comprehensive error handling.
:::

## What's New in v2.0

- ✨ **Multiple Unit Support** - 6 measurement units
- 🎯 **Enhanced API** - New methods for unit configuration
- 📊 **Better Precision** - Float return type
- 🚀 **Improved Performance** - Removed unnecessary delays
- 📚 **Complete Documentation** - This comprehensive guide!
- ✅ **Backward Compatible** - Existing code still works

## Community & Support

- **GitHub Issues** - [Report bugs or request features](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/issues)
- **Discussions** - [Ask questions and share projects](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/discussions)
- **Email** - hello@magni.dev

## License

MinimalUltrasonic is released under the [MIT License](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/blob/master/LICENSE).

Based on the excellent work of [Erick Simões](https://github.com/ErickSimoes/Ultrasonic).

---

<div style="text-align: center; margin-top: 2rem;">
  <p><strong>Made with ❤️ by <a href="http://magni.dev">Magni Development</a></strong></p>
</div>


# MinimalUltrasonic

## Compatible with **HC-SR04**, **Ping)))** and **Seeed SEN136B5B**

A minimalist, efficient Arduino library for ultrasonic distance sensors. This library prioritizes resource efficiency, code simplicity, and accurate measurements while supporting multiple units of measurement.

### ✨ Features

- 🎯 **Multiple Units**: cm, meters, mm, inches, yards, and miles
- ⚡ **Resource Efficient**: Minimal memory footprint and optimized code execution
- 🔧 **Flexible Configuration**: Support for both 3-pin and 4-pin sensors
- ⏱️ **Configurable Timeout**: Control maximum detection range
- 📦 **Multiple Sensors**: Use multiple sensors simultaneously
- 📝 **Well Documented**: Comprehensive inline documentation and examples
- 🔄 **Backward Compatible**: Works with existing code using CM and INC constants

### 🎮 Try it Online

Run examples on the **Wokwi Arduino Simulator** (free, browser-based):

- [Example 1 - Basic 3-pin and 4-pin sensor usage](https://wokwi.com/arduino/projects/296099969420493322)
- [Example 2 - Multiple units demonstration](https://wokwi.com/arduino/projects/312346565007114818)

### 🔌 Wiring

#### HC-SR04 (4-pin sensor)

```txt
HC-SR04      Arduino
-------------------------
VCC     ->   5V
Trig    ->   Pin 12
Echo    ->   Pin 13
GND     ->   GND
```

![HC-SR04 with Arduino](assets/HC-SR04-with-Arduino.jpg?raw=true "HC-SR04 with Arduino")

#### Ping))) or Seeed SEN136B5B (3-pin sensor)

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

### Basic Usage

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

### Reading in Different Units

```cpp
// Method 1: Specify unit in read() call
float cm = sensor.read(MinimalUltrasonic::CM);
float meters = sensor.read(MinimalUltrasonic::METERS);
float mm = sensor.read(MinimalUltrasonic::MM);
float inches = sensor.read(MinimalUltrasonic::INCHES);
float yards = sensor.read(MinimalUltrasonic::YARDS);
float miles = sensor.read(MinimalUltrasonic::MILES);

// Method 2: Set default unit
sensor.setUnit(MinimalUltrasonic::METERS);
float distance = sensor.read();  // Now returns meters by default

// Backward compatibility with v1.x
float cm = sensor.read(CM);
float inches = sensor.read(INC);
```

### Available Units

| Unit | Enum Value | Description |
|------|------------|-------------|
| Centimeters | `MinimalUltrasonic::CM` | Default, 1 cm precision |
| Meters | `MinimalUltrasonic::METERS` | For longer distances |
| Millimeters | `MinimalUltrasonic::MM` | For high precision |
| Inches | `MinimalUltrasonic::INCHES` | Imperial system |
| Yards | `MinimalUltrasonic::YARDS` | Imperial system |
| Miles | `MinimalUltrasonic::MILES` | Very long distances |

### Configuring Timeout

The timeout determines the maximum detectable distance:

```cpp
// Method 1: Set in constructor
MinimalUltrasonic sensor(12, 13, 40000UL);  // ~6.8m max range

// Method 2: Set after construction
sensor.setTimeout(40000UL);  // Timeout in microseconds

// Method 3: Set by distance (easier!)
sensor.setMaxDistance(680);  // 680 cm = 6.8m

// Get current timeout
unsigned long timeout = sensor.getTimeout();
```

**Timeout Guidelines:**

- `20000µs` (default) ≈ 3.4m max range
- `30000µs` ≈ 5.1m max range
- `40000µs` ≈ 6.8m max range

### Using Multiple Sensors

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor1(12, 13);  // HC-SR04
MinimalUltrasonic sensor2(10);      // Ping)))
MinimalUltrasonic sensor3(8);       // Seeed

void setup() {
  Serial.begin(9600);
  
  // Optional: Set different default units
  sensor1.setUnit(MinimalUltrasonic::CM);
  sensor2.setUnit(MinimalUltrasonic::METERS);
  sensor3.setUnit(MinimalUltrasonic::INCHES);
}

void loop() {
  Serial.print("Sensor 1: ");
  Serial.print(sensor1.read());
  Serial.println(" cm");
  
  Serial.print("Sensor 2: ");
  Serial.print(sensor2.read());
  Serial.println(" m");
  
  Serial.print("Sensor 3: ");
  Serial.print(sensor3.read());
  Serial.println(" inches");
  
  delay(1000);
}
```

## 📚 API Reference

### Constructors

```cpp
// 3-pin sensor (Ping, Seeed)
MinimalUltrasonic(uint8_t sigPin)

// 4-pin sensor (HC-SR04)
MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut = 20000UL)
```

### Methods

```cpp
// Read distance in specified unit (default: CM)
float read(Unit unit = CM) const

// Set timeout in microseconds
void setTimeout(unsigned long timeOut)

// Set maximum distance in centimeters
void setMaxDistance(unsigned int distance)

// Get current timeout
unsigned long getTimeout() const

// Set default unit for read() calls
void setUnit(Unit unit)

// Get current default unit
Unit getUnit() const
```

## 💡 Examples

The library includes several examples:

1. **UltrasonicSimple** - Basic distance measurement
2. **Timeout** - Configuring timeout for max range
3. **MultipleUltrasonicSensors** - Using multiple sensors
4. **AllUnits** - Demonstrating all available units

Access examples in Arduino IDE: `File > Examples > MinimalUltrasonic`

## 🎯 Best Practices

1. **Sensor Placement**: Ensure sensor has clear line of sight to target
2. **Timeout Configuration**: Set appropriate timeout for your use case
3. **Delay Between Readings**: Add small delay (20-30ms minimum) between measurements
4. **Error Handling**: Check for `0` return value indicating timeout/no detection
5. **Avoid Interference**: Space multiple sensors apart to prevent crosstalk
6. **Power Supply**: Ensure stable 5V power supply for best results

## 🔧 Technical Details

- **Speed of Sound**: 343 m/s (assumed at 20°C, sea level)
- **Detection Range**: 2cm to ~400cm (depends on sensor and timeout)
- **Measurement Accuracy**: ±3mm (sensor dependent)
- **Angular Coverage**: ~15° cone (HC-SR04)
- **Operating Voltage**: 5V DC
- **Current Consumption**: <15mA

## 🐛 Troubleshooting

**Problem: Always returns 0**

- Check wiring connections
- Verify power supply (5V)
- Increase timeout with `setTimeout()`
- Ensure object is within detection range

**Problem: Unstable readings**

- Add delay between measurements
- Check for electrical interference
- Ensure sensor is mounted firmly
- Verify object surface is not too absorbent

**Problem: Won't compile**

- Verify library is installed correctly
- Check for typos in code
- Ensure Arduino.h is available

## 📄 License

MinimalUltrasonic by [fermeridamagni](http://magni.dev "fermeridamagni - Magni Development") is licensed under the MIT License.

Based on [the work of Erick Simões](https://github.com/ErickSimoes/Ultrasonic).

See [LICENSE](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/blob/master/LICENSE) for details.

## 🤝 Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/issues)
- **Discussions**: [GitHub Discussions](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/discussions)
- **Email**: <hello@magni.dev>

---

**Made with ❤️ by [Magni Development](http://magni.dev)**

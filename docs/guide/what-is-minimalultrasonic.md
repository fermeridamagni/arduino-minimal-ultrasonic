# What is MinimalUltrasonic?

MinimalUltrasonic is a lightweight, efficient Arduino library designed for ultrasonic distance measurement. It provides a clean, simple API for working with popular ultrasonic sensors like the HC-SR04, Ping))), and Seeed SEN136B5B.

## Philosophy

The library is built on three core principles:

### 1. Minimalism

Keep it simple. No bloat, no unnecessary features. Just accurate distance measurement with the features you actually need.

### 2. Efficiency

Optimized for minimal memory usage and fast execution. Perfect for memory-constrained Arduino projects.

### 3. Accuracy

Physics-based calculations using the actual speed of sound, not arbitrary divisors.

## Why Choose MinimalUltrasonic?

### Compared to Other Libraries

| Feature | MinimalUltrasonic | Other Libraries |
|---------|-------------------|-----------------|
| Memory per sensor | 8 bytes | 20-40 bytes |
| Code size | ~1.4 KB | 3-5 KB |
| Units supported | 6 | 1-2 |
| Documentation | Comprehensive | Basic |
| Type safety | Enum-based | Define-based |
| Precision | Float | Integer |

### Key Advantages

- **Multiple Units** - Measure in cm, meters, mm, inches, yards, or miles
- **Small Footprint** - Minimal memory usage, perfect for resource-constrained projects
- **Type Safety** - Enum-based API prevents errors at compile time
- **Well Documented** - Complete API reference, guides, and examples
- **Proven** - Used in real-world projects, thoroughly tested
- **Modern** - Built with modern C++ practices

## Supported Sensors

### HC-SR04 (4-pin)

The most common ultrasonic sensor. Uses separate trigger and echo pins.

**Specifications:**

- Operating Voltage: 5V DC
- Range: 2cm - 400cm
- Accuracy: ±3mm
- Measuring Angle: ~15°

### Ping))) (3-pin)

Parallax ultrasonic sensor with a single signal pin.

**Specifications:**

- Operating Voltage: 5V DC
- Range: 2cm - 300cm
- Single pin for trigger and echo

### Seeed SEN136B5B (3-pin)

Grove-compatible ultrasonic sensor.

**Specifications:**

- Operating Voltage: 3.3V - 5V DC
- Range: 3cm - 400cm
- Grove connector compatible

### Other Compatible Sensors

- HC-SR04+ (3.3V/5V)
- US-015
- JSN-SR04T (waterproof)
- HY-SRF05

## How Ultrasonic Sensors Work

Ultrasonic sensors use sound waves to measure distance:

1. **Trigger** - Send a 10µs HIGH pulse to trigger pin
2. **Transmit** - Sensor emits 8 ultrasonic pulses at 40kHz
3. **Wait** - Sound travels to object and bounces back
4. **Receive** - Echo pin goes HIGH when echo received
5. **Calculate** - Measure echo pulse duration, calculate distance

### The Physics

```txt
Speed of Sound = 343 m/s (at 20°C, sea level)
Distance = (Time × Speed of Sound) / 2
```

The division by 2 is because sound travels to the object and back (round trip).

## Use Cases

### Robotics

- Obstacle avoidance
- Wall following
- Distance keeping
- Object detection

### Home Automation

- Parking assist systems
- Water level monitoring
- Waste bin fill level
- Automatic lighting

### Educational

- Physics experiments
- Arduino learning projects
- STEM education
- Sensor integration practice

### Industrial

- Tank level monitoring
- Proximity sensing
- Object counting
- Position verification

## Version History

### v2.0.0 (Current)

- Multiple unit support (6 units)
- Enhanced API with new methods
- Float return type for precision
- Comprehensive documentation
- Improved performance

### v1.0.0 (Legacy)

- Basic cm/inch support
- Integer return type
- Simple API

## Next Steps

Ready to get started? Check out the [Getting Started Guide](/guide/getting-started) or jump straight to [Installation](/guide/installation).

::: tip Learn by Doing
The best way to understand the library is to try it! Start with the [Basic Usage Example](/examples/basic).
:::

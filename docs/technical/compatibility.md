# Hardware Compatibility

Complete compatibility information for Arduino boards and ultrasonic sensors.

## Supported Arduino Boards

### Fully Compatible

| Board | Clock | RAM | Flash | GPIO | Status |
|-------|-------|-----|-------|------|--------|
| Arduino Uno | 16 MHz | 2 KB | 32 KB | 5V | ✅ Tested |
| Arduino Nano | 16 MHz | 2 KB | 32 KB | 5V | ✅ Tested |
| Arduino Mega 2560 | 16 MHz | 8 KB | 256 KB | 5V | ✅ Tested |
| Arduino Leonardo | 16 MHz | 2.5 KB | 32 KB | 5V | ✅ Compatible |
| Arduino Micro | 16 MHz | 2.5 KB | 32 KB | 5V | ✅ Compatible |
| Arduino Pro Mini | 16 MHz | 2 KB | 32 KB | 5V | ✅ Compatible |

### Compatible with Voltage Conversion

| Board | Clock | RAM | GPIO Logic | Requires |
|-------|-------|-----|------------|----------|
| Arduino Due | 84 MHz | 96 KB | 3.3V | Level shifter |
| Arduino Zero | 48 MHz | 32 KB | 3.3V | Level shifter |
| Arduino MKR1000 | 48 MHz | 32 KB | 3.3V | Level shifter |
| ESP32 | 240 MHz | 320 KB | 3.3V | Level shifter |
| ESP8266 | 80 MHz | 80 KB | 3.3V | Level shifter |
| Teensy 3.x | 72-180 MHz | 64-256 KB | 3.3V | Level shifter |
| Teensy 4.x | 600 MHz | 512 KB-1 MB | 3.3V | Level shifter |

### Not Recommended

| Board | Issue | Workaround |
|-------|-------|------------|
| Arduino 101 | Different architecture | Use native libraries |
| Arduino Yún | Limited GPIO | Use standard pins only |
| Gemma/Trinket | Limited pins | Use 3-pin mode |

## Supported Sensors

### HC-SR04 (Primary)

**Specifications:**

- Voltage: 5V DC
- Current: 15mA (peak), 2mA (idle)
- Range: 2cm - 400cm
- Accuracy: ±3mm
- Angle: 15° effective
- Frequency: 40kHz
- Trigger: 10µs pulse
- Echo: Pulse width proportional to distance

**Pinout:**

```txt
┌─────────────────┐
│  HC-SR04        │
│                 │
│  ┌───┐   ┌───┐ │
│  │ T │   │ R │ │ T = Transmitter
│  └───┘   └───┘ │ R = Receiver
│                 │
│ VCC GND TRG ECH │
└──┬──┬───┬───┬──┘
   │  │   │   └── Echo (output)
   │  │   └────── Trigger (input)
   │  └────────── Ground
   └───────────── 5V power
```

**Wiring:**

```cpp
// 4-pin mode (recommended)
VCC  → 5V
GND  → GND
TRIG → Digital Pin 7
ECHO → Digital Pin 8

// 3-pin mode (shared trigger/echo)
VCC  → 5V
GND  → GND
TRIG → Digital Pin 7
ECHO → Digital Pin 7 (via 1kΩ resistor)
```

**Code:**

```cpp
// 4-pin
MinimalUltrasonic sensor(7, 8);

// 3-pin
MinimalUltrasonic sensor(7);
```

### Ping))) (Parallax)

**Specifications:**

- Voltage: 5V DC
- Current: 35mA (peak), <2mA (idle)
- Range: 3cm - 300cm
- Accuracy: ±1cm
- Angle: 45° wide beam
- Frequency: 40kHz
- Single pin: Combined trigger/echo

**Pinout:**

```txt
┌─────────────┐
│   Ping)))   │
│             │
│    ┌───┐    │
│    │ o │    │ Transducer
│    └───┘    │
│             │
│ GND  SIG  V+│
└──┬────┬───┬─┘
   │    │   └─── +5V
   │    └─────── Signal (I/O)
   └──────────── Ground
```

**Wiring:**

```cpp
GND → GND
SIG → Digital Pin 7
V+  → 5V
```

**Code:**

```cpp
// Use 3-pin mode (shared trigger/echo)
MinimalUltrasonic sensor(7);
```

### Seeed Studio Ultrasonic Ranger

**Specifications:**

- Voltage: 3.3-5V DC
- Current: 20mA (peak)
- Range: 3cm - 400cm
- Accuracy: ±1cm
- Angle: 15° beam
- Frequency: 42kHz
- Single pin: Combined trigger/echo

**Pinout:**

```txt
┌─────────────┐
│ Seeed       │
│ Ultrasonic  │
│             │
│ VCC SIG GND │
└──┬───┬───┬──┘
   │   │   └─── Ground
   │   └─────── Signal (I/O)
   └─────────── 3.3-5V power
```

**Wiring:**

```cpp
VCC → 5V (or 3.3V)
SIG → Digital Pin 7
GND → GND
```

**Code:**

```cpp
MinimalUltrasonic sensor(7);
```

### HY-SRF05

**Specifications:**

- Voltage: 5V DC
- Current: 15mA
- Range: 2cm - 450cm
- Accuracy: ±3mm
- Mode 1: Separate trigger/echo (like HC-SR04)
- Mode 2: Single pin (OUT pin unconnected)

**Wiring:**

Mode 1 (4-pin):

```cpp
VCC  → 5V
GND  → GND
TRIG → Digital Pin 7
ECHO → Digital Pin 8
OUT  → Leave unconnected

MinimalUltrasonic sensor(7, 8);
```

Mode 2 (3-pin):

```cpp
VCC  → 5V
GND  → GND
TRIG → Digital Pin 7
ECHO → Leave unconnected
OUT  → Leave unconnected

MinimalUltrasonic sensor(7);
```

### US-100

**Specifications:**

- Voltage: 5V DC (also 3.3V)
- Current: 20mA
- Range: 2cm - 450cm
- Accuracy: ±3mm
- Modes: Ultrasonic or UART
- Temperature sensor: Optional

**Ultrasonic Mode (Jumper removed):**

```cpp
VCC  → 5V
GND  → GND
TRIG → Digital Pin 7
ECHO → Digital Pin 8

MinimalUltrasonic sensor(7, 8);
```

**Note:** Remove jumper for ultrasonic mode. With jumper, uses UART (not compatible with this library).

## Voltage Level Conversion

### Why Needed

HC-SR04 requires 5V logic:

- **Input (Trigger):** Needs 5V HIGH
- **Output (Echo):** Outputs 5V HIGH

3.3V boards (ESP32, Due, etc.):

- **Output:** Only 3.3V HIGH (may not trigger HC-SR04)
- **Input:** 5V can damage GPIO (max 3.3V)

### Level Shifter Solutions

**Option 1: Bidirectional Level Shifter (Best)**

```txt
3.3V Board          Level Shifter       HC-SR04
──────────         ───────────────      ─────────
      
3.3V ─────────────── LV              
                     HV ─────────────── 5V
                                    
Pin 7 ──────────────── A1              
(Trig)               B1 ─────────────── TRIG
                                    
Pin 8 ──────────────── A2              
(Echo)               B2 ─────────────── ECHO
                                    
GND ─────────────────GND─────────────── GND
```

Products:

- Adafruit BSS138 (4-channel)
- SparkFun BOB-12009 (4-channel)
- Generic 4-channel MOSFET shifter

**Option 2: Voltage Divider (Echo only)**

```txt
HC-SR04              3.3V Board
────────             ──────────

ECHO ────┬───────────── Pin 8
         │
        ┌┴┐
        │ │ 1kΩ
        └┬┘
         ├───────────── Pin 7 (Trig)
        ┌┴┐
        │ │ 2kΩ
        └┬┘
         │
        GND
```

**Math:**

```txt
V_out = V_in × R2 / (R1 + R2)
V_out = 5V × 2kΩ / (1kΩ + 2kΩ)
V_out = 3.33V
```

**Limitations:**

- Echo protection only
- Trigger may not reach 5V (HC-SR04 might work with 3.3V trigger, depends on module)

**Option 3: Dedicated 5V Supply + Level Shifter**

Best for ESP32/ESP8266:

```txt
Power Supply         ESP32            HC-SR04
────────────        ───────          ─────────
                    
5V ─────────┬─────── VIN (if present)
            └────────────────────── VCC
                                
3.3V ─────────────── LV (Shifter)
                                
5V ─────────────────── HV (Shifter)
                                
GND ────────┴────────────────────── GND
```

## Pin Requirements

### GPIO Pin Selection

**Any digital pins work, but consider:**

1. **Avoid PWM pins if using PWM elsewhere**
   - Uno: 3, 5, 6, 9, 10, 11
   - Mega: 2-13, 44-46

2. **Avoid I2C pins if using I2C**
   - Uno: A4 (SDA), A5 (SCL)
   - Mega: 20 (SDA), 21 (SCL)

3. **Avoid SPI pins if using SPI**
   - Uno: 10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK)
   - Mega: 50 (MISO), 51 (MOSI), 52 (SCK), 53 (SS)

4. **Avoid Serial pins if using Serial**
   - Uno: 0 (RX), 1 (TX)
   - Mega: 0/1 (Serial), 14-19 (Serial1-3)

**Recommended pins for Uno:**

```cpp
// Good choices (no special functions):
Pin 2, 4, 7, 8

// Example:
MinimalUltrasonic sensor(7, 8);
```

**Recommended pins for Mega:**

```cpp
// Plenty of options, example:
MinimalUltrasonic sensor(22, 23);
```

### 3-Pin vs 4-Pin Mode

**4-Pin Mode (Recommended):**

```cpp
MinimalUltrasonic sensor(7, 8);

Advantages:
✅ Simpler wiring (no resistor)
✅ Clearer pin usage
✅ Works with all sensors

Disadvantages:
❌ Uses 2 GPIO pins
```

**3-Pin Mode:**

```cpp
MinimalUltrasonic sensor(7);

Advantages:
✅ Saves 1 GPIO pin
✅ Required for Ping))) sensors

Disadvantages:
❌ Needs resistor (HC-SR04)
❌ More complex wiring
```

## Power Requirements

### Current Draw

**Per sensor:**

```txt
State           | Current
----------------|--------
Idle            | 2 mA
During ping     | 15 mA (HC-SR04)
                | 35 mA (Ping)))
                | 20 mA (Others)
Average @ 1Hz   | ~2 mA
Average @ 10Hz  | ~3 mA
Average @ 50Hz  | ~5 mA
```

**Multiple sensors:**

```txt
Sensors | Peak Current | Safe Supply
--------|--------------|-------------
1       | 15 mA        | 50 mA
2       | 30 mA        | 100 mA
5       | 75 mA        | 150 mA
10      | 150 mA       | 300 mA
```

**Arduino power limits:**

- USB power: 500 mA total
- 5V pin: 500 mA max (via USB)
- 5V pin: 800 mA max (via barrel jack, depends on regulator)
- Per GPIO: 40 mA max

### Power Supply Options

**Option 1: Arduino 5V Pin (Small projects)**

```cpp
// 1-3 sensors, fine
VCC → Arduino 5V pin
GND → Arduino GND
```

**Option 2: External 5V Supply (Many sensors)**

```cpp
// 4+ sensors, recommended
VCC → External 5V supply (+)
GND → External GND (-) AND Arduino GND (common ground!)
```

**Option 3: Separate Regulator**

```cpp
// Best for >10 sensors
Battery → 7805 regulator → Sensors VCC
                          → Arduino VIN
         Common GND ─────────────────
```

## Compatibility Issues

### Common Problems

**Problem 1: 3.3V board with HC-SR04**

```txt
Symptom: No readings or sporadic readings
Cause: Voltage mismatch
Solution: Use level shifter
```

**Problem 2: Insufficient power**

```txt
Symptom: Readings drop out, sensor resets
Cause: Current draw exceeds supply
Solution: External power supply
```

**Problem 3: GPIO conflicts**

```txt
Symptom: Sensor or other device stops working
Cause: Pin used by multiple peripherals
Solution: Use different pins
```

**Problem 4: Shared ground missing**

```txt
Symptom: Erratic behavior with external power
Cause: Arduino and sensor don't share GND
Solution: Connect GNDs together
```

**Problem 5: Crosstalk (multiple sensors)**

```txt
Symptom: Incorrect readings with multiple sensors
Cause: Ultrasonic interference
Solution: Time-multiplex or space sensors apart
```

## Testing Compatibility

### Quick Test Sketch

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(7, 8);

void setup() {
    Serial.begin(9600);
    Serial.println("MinimalUltrasonic Compatibility Test");
    Serial.println("====================================");
    
    // Board info
    Serial.print("Board: ");
    #if defined(__AVR_ATmega328P__)
        Serial.println("Arduino Uno/Nano");
    #elif defined(__AVR_ATmega2560__)
        Serial.println("Arduino Mega");
    #elif defined(__AVR_ATmega32U4__)
        Serial.println("Arduino Leonardo/Micro");
    #elif defined(ESP32)
        Serial.println("ESP32");
    #elif defined(ESP8266)
        Serial.println("ESP8266");
    #else
        Serial.println("Unknown");
    #endif
    
    // Sensor test
    Serial.println("\nSensor test (10 readings):");
    int successes = 0;
    
    for (int i = 0; i < 10; i++) {
        unsigned int dist = sensor.read();
        
        if (dist > 0) {
            successes++;
            Serial.print("✓ ");
        } else {
            Serial.print("✗ ");
        }
        
        Serial.print(dist);
        Serial.println(" cm");
        
        delay(100);
    }
    
    Serial.println("\n====================================");
    Serial.print("Success rate: ");
    Serial.print(successes * 10);
    Serial.println("%");
    
    if (successes >= 8) {
        Serial.println("✅ Fully compatible");
    } else if (successes >= 5) {
        Serial.println("⚠️ Marginal - check wiring/power");
    } else {
        Serial.println("❌ Incompatible - check voltage levels");
    }
}

void loop() {}
```

### Expected Results

**Fully compatible (Uno, Nano, Mega with HC-SR04):**

```txt
Success rate: 90-100%
✅ Fully compatible
```

**Needs level shifter (ESP32 with HC-SR04, no shifter):**

```txt
Success rate: 0%
❌ Incompatible - check voltage levels
```

**Power issues:**

```txt
Success rate: 30-70%
⚠️ Marginal - check wiring/power
```

## See Also

- [Getting Started](/guide/getting-started) - Setup instructions
- [Installation](/guide/installation) - Library installation
- [Troubleshooting](/guide/troubleshooting) - Fix common issues
- [Hardware Guide](https://create.arduino.cc/projecthub) - Arduino project tutorials

# Physics Background

Understanding the physics behind ultrasonic distance measurement.

## Ultrasonic Sound

### What is Ultrasonic?

**Ultrasonic** refers to sound waves with frequencies above the human hearing range.

- **Human hearing:** 20 Hz to 20 kHz
- **Ultrasonic range:** >20 kHz
- **HC-SR04 frequency:** 40 kHz

### Why 40 kHz?

1. **Above human hearing** - No audible noise
2. **Good directivity** - Focused beam pattern
3. **Reasonable range** - Balance of range and accuracy
4. **Low cost** - Standard piezoelectric transducers available

## Speed of Sound

### Basic Formula

```txt
speed = distance / time

or

distance = speed × time
```

### Speed of Sound in Air

At standard conditions (20°C, 1 atm):

```txt
v = 343 m/s
  = 0.343 m/ms
  = 0.0343 cm/µs
  = 1235 km/h
  = 767 mph
```

### Temperature Dependence

Speed of sound varies with temperature:

```txt
v = 331.3 + (0.606 × T) m/s
```

Where T is temperature in Celsius.

**Examples:**

```txt
T = 0°C:   v = 331.3 m/s
T = 10°C:  v = 337.4 m/s
T = 20°C:  v = 343.0 m/s  ← Used by library
T = 30°C:  v = 349.1 m/s
T = 40°C:  v = 355.2 m/s
```

**Effect on accuracy:**

- At 0°C vs 20°C: ~3.4% slower
- At 40°C vs 20°C: ~3.6% faster

## Distance Measurement Principle

### Time-of-Flight

HC-SR04 uses **Time-of-Flight (ToF)** measurement:

1. Send ultrasonic pulse
2. Pulse travels to object
3. Pulse reflects back
4. Measure total time
5. Calculate distance

### Round-Trip Distance

```txt
Total time = time to object + time back
           = 2 × (distance / speed)

Therefore:
distance = (total time × speed) / 2
```

### Formula Derivation

Starting with:

```txt
distance = (time × speed) / 2
```

Using v = 343 m/s = 0.0343 cm/µs:

```txt
distance_cm = (time_µs × 0.0343) / 2
            = time_µs × 0.01715
            = time_µs / 58.31
```

Library uses 58.8235 (more precise):

```txt
distance_cm = time_µs / 58.8235
```

## HC-SR04 Operation

### Measurement Cycle

```txt
1. Trigger Pulse (10µs)
   ┌──┐
   │  │
───┘  └───────────

2. Ultrasonic Burst (8 pulses at 40kHz)
   ┌┐┌┐┌┐┌┐┌┐┌┐┌┐┌┐
   ││││││││││││││││
   └┘└┘└┘└┘└┘└┘└┘└┘
   |<-- 200µs -->|

3. Echo Wait
   Sound travels to object and back

4. Echo Pulse
   ┌────────────┐
   │            │
───┘            └───
   |<-- pulse -->|
   Duration = distance measurement
```

### Timing Details

**Trigger:**

- Minimum pulse width: 10µs
- Typical: 10µs
- Initiates measurement

**Ultrasonic Burst:**

- 8 cycles of 40kHz
- Duration: 200µs (8 / 40000 Hz)
- Emitted automatically after trigger

**Echo Pulse:**

- Width proportional to distance
- Formula: width_µs = distance_cm × 58.8235

### Beam Pattern

HC-SR04 has a cone-shaped detection pattern:

```txt
         Sensor
           ||
          /  \
         /    \
        /      \
       /  15°   \
      /          \
     /            \
    /      30°     \
   /                \
  /                  \
 
Effective angle: ~15°
Max angle: ~30°
```

**Implications:**

- Object must be within beam
- Small objects may be missed
- Angled surfaces may not reflect

## Physical Limitations

### Minimum Distance (~2 cm)

**Causes:**

1. **Transducer recovery time** - Takes time to switch from TX to RX
2. **Blind zone** - Residual vibration during echo wait
3. **Close reflections** - Pulse overlaps with echo

### Maximum Distance (~4-7 m)

**Limiting factors:**

1. **Signal attenuation** - Sound weakens with distance
2. **Air absorption** - Higher frequency = more absorption
3. **Ambient noise** - Reflections, interference
4. **Transducer sensitivity** - Limited dynamic range

### Accuracy (±3mm typical)

**Factors:**

1. **Timing resolution** - Arduino: 4µs → ~0.7mm
2. **Temperature variation** - ±5°C → ±1.5% error
3. **Air pressure** - Minor effect
4. **Humidity** - Minor effect at 40kHz

## Environmental Factors

### Temperature

Effect: Changes speed of sound

```txt
Error = (v_actual - v_assumed) / v_assumed × 100%

Example at 30°C:
Error = (349.1 - 343.0) / 343.0 × 100%
      = 1.78%

At 100cm: Error = ±1.78cm
```

### Humidity

Effect: Slight increase in speed (~0.1-0.3%)

Negligible for most applications.

### Air Pressure

Effect: Minimal at normal variations

Only significant at extreme altitudes.

### Wind

Effect: Adds/subtracts from sound speed

```txt
v_effective = v_sound ± v_wind

At 10 m/s wind, 100cm distance:
Error ≈ ±2.9%
```

## Surface Properties

### Reflectivity

Different materials reflect sound differently:

| Material | Reflectivity | Notes |
|----------|-------------|-------|
| Hard plastic | Excellent | Best |
| Metal | Excellent | Flat surfaces |
| Wood | Good | Dense wood better |
| Cardboard | Fair | Absorbs some sound |
| Fabric | Poor | Absorbs sound |
| Foam | Very poor | Highly absorbent |

### Angle of Incidence

Sound reflects like light:

```txt
Angle of incidence = Angle of reflection

Perpendicular (90°): Best reflection
      ||
      \/
    ──────
    
Angled (45°): Reflects away
      ||
     /  \
    /    \
   ──────
```

**Best practice:** Mount sensor perpendicular to target (±15°)

## Calculation Examples

### Example 1: Echo Time to Distance

Given: Echo pulse width = 1470 µs

```txt
distance_cm = 1470 / 58.8235
            = 25.0 cm
```

### Example 2: Distance to Echo Time

Given: Want to measure 100 cm

```txt
time_µs = 100 × 58.8235
        = 5882.35 µs
        ≈ 5882 µs
```

### Example 3: Temperature Compensation

Measured at 20°C: 100.0 cm
Actual temp: 30°C

```txt
v_20C = 343.0 m/s
v_30C = 331.3 + (0.606 × 30) = 349.1 m/s

Correction = v_30C / v_20C
           = 349.1 / 343.0
           = 1.0178

Corrected distance = 100.0 × 1.0178
                   = 101.78 cm
```

### Example 4: Maximum Range Calculation

Timeout: 20000 µs

```txt
max_range_cm = 20000 / 58.8235
             = 340.0 cm
             = 3.4 m
```

### Example 5: Timeout for Desired Range

Want: 2 meter range

```txt
timeout_µs = 200 cm × 58.8235
           = 11764.7 µs
           ≈ 11765 µs
```

## Ultrasonic vs Other Technologies

### Comparison

| Technology | Range | Accuracy | Cost | Power |
|------------|-------|----------|------|-------|
| Ultrasonic | 2-400cm | ±3mm | Low | Low |
| IR Sharp | 10-80cm | ±5mm | Low | Low |
| Laser ToF | 5-400cm | ±1mm | High | Medium |
| Radar | 10-5000cm | ±1cm | High | Medium |

### When to Use Ultrasonic

**Advantages:**

- Low cost
- Simple interface
- Works in dust/fog
- Wide beam (finds objects easily)

**Disadvantages:**

- Temperature sensitive
- Affected by soft surfaces
- Slow update rate
- Limited range

## Advanced Topics

### Doppler Effect

Moving objects shift frequency:

```txt
f_observed = f_source × (v_sound ± v_observer) / (v_sound ± v_source)
```

HC-SR04 doesn't measure frequency, so Doppler doesn't affect distance reading.

### Multi-path Reflections

Sound can bounce multiple times:

```txt
Direct: Sensor → Object → Sensor
Multi-path: Sensor → Wall → Object → Wall → Sensor
```

Can cause incorrect readings.

**Mitigation:** Use shortest measured distance

### Interference

40kHz is common frequency:

- Other ultrasonic sensors
- Some electronic ballasts
- Animal calls (bats)

**Mitigation:** Time multiplex multiple sensors

## Mathematical Formulas

### Core Formulas

**Distance from time:**

```txt
d = (t × v) / 2
```

**Time from distance:**

```txt
t = (2 × d) / v
```

**CM conversion:**

```txt
d_cm = t_µs / 58.8235
```

**Temperature-corrected speed:**

```txt
v = 331.3 + (0.606 × T)
```

### Unit Conversions

**CM to other units:**

```txt
meters = cm × 0.01
mm = cm × 10.0
inches = cm × 0.393701
yards = cm × 0.0109361
miles = cm × 0.000006213712
```

## Practical Considerations

### Measurement Rate

Minimum time between readings:

```txt
t_min = 2 × (d_max / v) + overhead
      = 2 × (4m / 343 m/s) + 20ms
      ≈ 43ms

Practical minimum: 60ms
Maximum rate: ~16 Hz
```

### Power Consumption

HC-SR04 typical:

- Idle: <2 mA
- During ping: ~15 mA for ~200µs
- Average (1 Hz): ~2-3 mA
- Average (10 Hz): ~3-5 mA

### Resolution

Theoretical resolution:

```txt
Arduino timing resolution: 4µs (16MHz)
Distance resolution: 4µs / 58.8235 ≈ 0.068 cm

Practical resolution: ~3mm (0.3cm)
```

## See Also

- [Unit Conversions](/technical/conversions) - Detailed conversion math
- [Constants](/api/constants) - Physical constants used
- [Performance](/technical/performance) - Performance characteristics
- [Architecture](/technical/architecture) - Implementation details

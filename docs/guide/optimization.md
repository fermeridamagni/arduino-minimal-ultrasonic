# Optimization

Performance optimization techniques for MinimalUltrasonic sensor applications.

## Understanding Performance

### Key Performance Metrics

1. **Reading Frequency** - How often you can get measurements
2. **Response Time** - Time from trigger to result
3. **Accuracy** - Precision of measurements
4. **Resource Usage** - RAM and code size
5. **Reliability** - Success rate of readings

### Performance Trade-offs

```txt
Fast Readings ←→ Accuracy
  ↑                  ↑
  |                  |
Short Timeout    Long Timeout
Less Filtering   More Filtering
```

## Optimize Reading Speed

### 1. Reduce Timeout

Match timeout to your maximum detection range:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
  
  // For 50cm maximum range
  // Time = (50cm × 2) / 343m/s = 2917µs
  // Add 20% margin = 3500µs
  sensor.setTimeout(3500UL);
  
  // Results:
  // - Faster error detection (3.5ms vs 20ms default)
  // - Reduced loop time
  // - Can't detect beyond 60cm
}

void loop() {
  float distance = sensor.read();
  
  if (distance > 0 && distance <= 50.0) {
    Serial.println(distance);
  }
  
  delay(50);  // Can read at ~20 Hz
}
```

**Timeout Calculation:**

```cpp
// Formula: timeout = (maxDistance_cm × 2 × 10) / 343 + margin
// Factor of 2: sound travels to object and back
// Factor of 10: convert cm to mm, result to µs
// 343: speed of sound in m/s

unsigned long calculateTimeout(float maxDistanceCm) {
  // Round trip time in microseconds
  unsigned long timeout = (unsigned long)((maxDistanceCm * 2.0 * 10.0) / 0.343);
  
  // Add 20% safety margin
  timeout = timeout * 1.2;
  
  return timeout;
}

void setup() {
  sensor.setTimeout(calculateTimeout(100.0));  // 100cm max
}
```

### 2. Minimize Processing

Keep loop code lean:

```cpp
// SLOW: Excessive processing
void loop() {
  float distance = sensor.read();
  
  String msg = "Distance is: " + String(distance) + " centimeters";
  Serial.println(msg);
  
  for (int i = 0; i < 100; i++) {
    // Heavy processing
  }
  
  delay(100);
}

// FAST: Minimal processing
void loop() {
  float distance = sensor.read();
  
  // Direct printing (no String objects)
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(100);
}
```

### 3. Optimize Serial Communication

```cpp
// SLOW: Low baud rate
void setup() {
  Serial.begin(9600);  // 960 bytes/sec
}

// FAST: Higher baud rate
void setup() {
  Serial.begin(115200);  // 11,520 bytes/sec
}

// FASTEST: Reduce output
void loop() {
  float distance = sensor.read();
  
  // Instead of printing every reading
  // Only print on change
  static float lastDistance = 0;
  
  if (abs(distance - lastDistance) > 0.5) {  // 0.5cm threshold
    Serial.println(distance);
    lastDistance = distance;
  }
  
  delay(50);
}
```

## Memory Optimization

### 1. Minimize RAM Usage

MinimalUltrasonic is already optimized (8 bytes per instance):

```cpp
// Each sensor uses only 8 bytes
MinimalUltrasonic sensor1(12, 13);  // 8 bytes
MinimalUltrasonic sensor2(10, 11);  // 8 bytes
// Total: 16 bytes

// Compare to typical alternatives: 20-40 bytes each
```

### 2. Efficient Data Structures

```cpp
// BAD: Wasteful storage
float readings[100];  // 400 bytes

// GOOD: Appropriate size
float readings[10];   // 40 bytes

// BETTER: Use circular buffer
const int BUFFER_SIZE = 5;
float buffer[BUFFER_SIZE];
int index = 0;

void addReading(float distance) {
  buffer[index] = distance;
  index = (index + 1) % BUFFER_SIZE;  // Wrap around
}
```

### 3. Use PROGMEM for Constants

```cpp
#include <avr/pgmspace.h>

// Store strings in flash memory instead of RAM
const char msg1[] PROGMEM = "Sensor initialized";
const char msg2[] PROGMEM = "Error detected";
const char msg3[] PROGMEM = "Distance out of range";

void printMessage(const char* message) {
  char buffer[50];
  strcpy_P(buffer, message);
  Serial.println(buffer);
}

void setup() {
  printMessage(msg1);
}
```

## Filtering Optimization

### 1. Simple Moving Average (Fast)

```cpp
const int SAMPLES = 3;  // Small sample size
float buffer[SAMPLES];
int index = 0;

float getFiltered() {
  buffer[index] = sensor.read();
  index = (index + 1) % SAMPLES;
  
  float sum = 0;
  for (int i = 0; i < SAMPLES; i++) {
    sum += buffer[i];
  }
  
  return sum / SAMPLES;
}

// Time complexity: O(n) where n = SAMPLES
// Memory: 12 bytes for buffer
```

### 2. Exponential Moving Average (Faster)

```cpp
float ema = 0;
const float ALPHA = 0.3;  // Smoothing factor (0-1)

float getEMA() {
  float current = sensor.read();
  
  if (ema == 0) {
    ema = current;  // Initialize
  } else {
    ema = (ALPHA * current) + ((1 - ALPHA) * ema);
  }
  
  return ema;
}

// Time complexity: O(1) - constant time
// Memory: 4 bytes for ema
// Faster than moving average!
```

### 3. Conditional Filtering

```cpp
// Only filter when needed
float getConditionalFiltered() {
  static float lastValue = 0;
  float current = sensor.read();
  
  // If reading is stable, don't filter
  if (abs(current - lastValue) < 1.0) {
    lastValue = current;
    return current;
  }
  
  // If unstable, use simple average
  float filtered = (current + lastValue) / 2.0;
  lastValue = filtered;
  return filtered;
}
```

## Multiple Sensors Optimization

### 1. Parallel Reading (Fast but Risky)

```cpp
// Read multiple sensors quickly
// Risk: May cause interference

void fastRead() {
  float d1 = sensor1.read();  // 20ms
  float d2 = sensor2.read();  // 20ms
  // Total: ~40ms
}
```

### 2. Sequential Reading (Slower but Reliable)

```cpp
// Read with delays to prevent interference
void reliableRead() {
  float d1 = sensor1.read();  // 20ms
  delay(30);                   // 30ms
  float d2 = sensor2.read();  // 20ms
  delay(30);                   // 30ms
  // Total: ~100ms
}
```

### 3. Smart Scheduling

```cpp
// Read sensors at different rates based on priority
unsigned long lastReadTime[3] = {0};
int readInterval[3] = {50, 100, 200};  // ms

void loop() {
  unsigned long now = millis();
  
  // Check each sensor's schedule
  for (int i = 0; i < 3; i++) {
    if (now - lastReadTime[i] >= readInterval[i]) {
      float dist = sensors[i].read();
      processSensor(i, dist);
      lastReadTime[i] = now;
    }
  }
}
```

## Computational Optimization

### 1. Avoid Floating Point When Possible

```cpp
// SLOWER: Floating point
float distance = sensor.read();
float threshold = 30.5;
if (distance < threshold) { /* ... */ }

// FASTER: Integer comparison when precision allows
float distance = sensor.read();
if (distance < 31) { /* ... */ }  // Compiler optimizes
```

### 2. Use Lookup Tables

```cpp
// For frequent unit conversions
const float CM_TO_INCH = 0.393701;
const float CM_TO_METER = 0.01;

// Instead of calculating each time:
float inches = distance * 0.393701;  // Calculation

// Use constant:
float inches = distance * CM_TO_INCH;  // Faster
```

### 3. Minimize Function Calls

```cpp
// SLOW: Multiple function calls
void loop() {
  if (sensor.read() > 0) {
    if (sensor.read() < 50) {
      processDistance(sensor.read());  // 3 reads!
    }
  }
}

// FAST: Single call, store result
void loop() {
  float distance = sensor.read();  // 1 read
  
  if (distance > 0 && distance < 50) {
    processDistance(distance);
  }
}
```

## Real-time Performance

### 1. Non-blocking Approach

```cpp
// For responsive systems, avoid delay()
unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 100;  // ms

void loop() {
  unsigned long now = millis();
  
  if (now - lastReadTime >= READ_INTERVAL) {
    float distance = sensor.read();
    processDistance(distance);
    lastReadTime = now;
  }
  
  // Other non-blocking code can run here
  checkButtons();
  updateDisplay();
}
```

### 2. Interrupt-driven Alternative

```cpp
// Note: This library doesn't use interrupts by default
// But you can implement your own for ultra-fast response

volatile bool readingReady = false;
volatile unsigned long echoTime = 0;

void setup() {
  pinMode(ECHO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoISR, CHANGE);
}

void echoISR() {
  if (digitalRead(ECHO_PIN) == HIGH) {
    echoTime = micros();
  } else {
    echoTime = micros() - echoTime;
    readingReady = true;
  }
}

// Advantage: Main loop never blocks
// Disadvantage: More complex code
```

## Benchmarking

### Measure Performance

```cpp
void benchmarkReading() {
  Serial.println("=== Performance Test ===");
  
  // Test reading speed
  unsigned long startTime = millis();
  int readCount = 100;
  
  for (int i = 0; i < readCount; i++) {
    sensor.read();
  }
  
  unsigned long elapsed = millis() - startTime;
  float avgTime = (float)elapsed / readCount;
  float maxFrequency = 1000.0 / avgTime;
  
  Serial.print("Average read time: ");
  Serial.print(avgTime);
  Serial.println(" ms");
  
  Serial.print("Maximum frequency: ");
  Serial.print(maxFrequency);
  Serial.println(" Hz");
  
  // Test with different timeouts
  unsigned long timeouts[] = {5000, 10000, 20000, 40000};
  
  for (int i = 0; i < 4; i++) {
    sensor.setTimeout(timeouts[i]);
    
    startTime = micros();
    sensor.read();
    elapsed = micros() - startTime;
    
    Serial.print("Timeout ");
    Serial.print(timeouts[i]);
    Serial.print("µs -> Read time: ");
    Serial.print(elapsed);
    Serial.println("µs");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  benchmarkReading();
}
```

### Memory Usage

```cpp
void printMemoryUsage() {
  extern int __heap_start, *__brkval;
  int freeRAM;
  
  if (__brkval == 0) {
    freeRAM = ((int)&freeRAM) - ((int)&__heap_start);
  } else {
    freeRAM = ((int)&freeRAM) - ((int)__brkval);
  }
  
  Serial.print("Free RAM: ");
  Serial.print(freeRAM);
  Serial.println(" bytes");
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Before sensor creation:");
  printMemoryUsage();
  
  MinimalUltrasonic sensor(12, 13);
  
  Serial.println("After sensor creation:");
  printMemoryUsage();
  // Should show 8 bytes difference
}
```

## Platform-Specific Optimizations

### Arduino Uno / Nano (ATmega328P)

```cpp
// 2KB RAM, 16 MHz
// - Use small buffers (< 20 elements)
// - Optimize timeout for your range
// - Limit serial output
// - Max 2-3 sensors recommended

const int MAX_SENSORS = 3;
MinimalUltrasonic sensors[MAX_SENSORS] = {
  MinimalUltrasonic(12, 13),
  MinimalUltrasonic(10, 11),
  MinimalUltrasonic(8, 9)
};
```

### Arduino Mega (ATmega2560)

```cpp
// 8KB RAM, 16 MHz
// - Can handle more sensors (10+)
// - Larger buffers allowed
// - More room for filtering

const int MAX_SENSORS = 12;
MinimalUltrasonic sensors[MAX_SENSORS] = { /* ... */ };
```

### ESP32 / ESP8266

```cpp
// Much more RAM, faster CPU
// - Can handle many sensors
// - Complex filtering possible
// - Higher baud rates

void setup() {
  Serial.begin(460800);  // Much faster serial
}
```

## Optimization Checklist

✅ **Timeout**

- [ ] Set to minimum needed for your range
- [ ] Not using default if unnecessary

✅ **Reading Frequency**

- [ ] Minimum 60ms between reads
- [ ] Appropriate for your application

✅ **Filtering**

- [ ] Only filter if needed
- [ ] Use simplest filter that works

✅ **Memory**

- [ ] Buffer sizes appropriate
- [ ] No memory leaks
- [ ] Constants in PROGMEM if needed

✅ **Serial Output**

- [ ] High enough baud rate
- [ ] Not printing unnecessarily
- [ ] Output only on change

✅ **Multiple Sensors**

- [ ] Prevent crosstalk
- [ ] Sequential reading if needed
- [ ] Smart scheduling implemented

## Performance Targets

### Good Performance Benchmarks

**Single Sensor:**

- Reading time: 20-60 ms (depends on timeout)
- Frequency: 10-15 Hz typical
- RAM usage: 8 bytes
- Error rate: < 5%

**Multiple Sensors (3):**

- Sequential reading: 100-200 ms
- Frequency: 5-10 Hz typical
- RAM usage: 24 bytes
- Error rate: < 10%

**Filtered Reading:**

- Additional overhead: 5-20 ms
- Depends on filter complexity
- Memory: 20-100 bytes for buffer

## Summary

**Key Optimization Strategies:**

1. **Reduce timeout** to minimum needed range
2. **Minimize processing** in main loop
3. **Use efficient filtering** (EMA instead of moving average)
4. **Optimize serial output** (higher baud, less output)
5. **Smart scheduling** for multiple sensors
6. **Non-blocking code** for responsiveness
7. **Benchmark** to measure improvements

## Next Steps

- [Best Practices](/guide/best-practices) - General guidelines
- [Multiple Sensors](/guide/multiple-sensors) - Multi-sensor optimization
- [Performance](/technical/performance) - Technical performance details

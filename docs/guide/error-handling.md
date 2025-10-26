# Error Handling

Learn how to handle errors and invalid readings from ultrasonic sensors.

## Understanding Error Conditions

Ultrasonic sensors can return invalid readings due to various conditions:

- **No echo received** - Object too far or not detected
- **Timeout exceeded** - No valid echo within timeout period
- **Invalid angle** - Sound wave doesn't bounce back
- **Interference** - Crosstalk from other sensors or noise
- **Environmental factors** - Temperature, humidity, soft surfaces

## Return Value of `0`

When `read()` returns `0`, it indicates an error condition:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void loop() {
  float distance = sensor.read();
  
  if (distance == 0) {
    Serial.println("❌ Error: No valid reading");
  } else {
    Serial.print("✓ Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  
  delay(500);
}
```

## Basic Error Handling

### Check for Valid Readings

```cpp
void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    // Valid reading
    Serial.print("Distance: ");
    Serial.println(distance);
  } else {
    // Error condition
    Serial.println("No object detected");
  }
  
  delay(100);
}
```

### Range Validation

```cpp
const float MIN_VALID = 2.0;    // 2 cm minimum
const float MAX_VALID = 400.0;  // 4 m maximum

void loop() {
  float distance = sensor.read();
  
  if (distance >= MIN_VALID && distance <= MAX_VALID) {
    // Valid range
    Serial.print("Valid: ");
    Serial.println(distance);
  } else if (distance == 0) {
    Serial.println("Error: No echo");
  } else {
    Serial.println("Error: Out of range");
  }
  
  delay(100);
}
```

## Advanced Error Handling

### Multiple Readings with Validation

Take multiple readings and use the median to filter out errors:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

float getMedianDistance(int samples = 5) {
  float readings[samples];
  int validCount = 0;
  
  // Collect readings
  for (int i = 0; i < samples; i++) {
    float dist = sensor.read();
    if (dist > 0) {
      readings[validCount++] = dist;
    }
    delay(30);
  }
  
  // Not enough valid readings
  if (validCount < 3) {
    return 0;
  }
  
  // Sort readings
  for (int i = 0; i < validCount - 1; i++) {
    for (int j = i + 1; j < validCount; j++) {
      if (readings[i] > readings[j]) {
        float temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }
  
  // Return median
  return readings[validCount / 2];
}

void loop() {
  float distance = getMedianDistance();
  
  if (distance > 0) {
    Serial.print("Reliable distance: ");
    Serial.println(distance);
  } else {
    Serial.println("Could not get reliable reading");
  }
  
  delay(500);
}
```

### Error Counter with Threshold

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

int errorCount = 0;
const int ERROR_THRESHOLD = 5;

void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    // Valid reading
    errorCount = 0;  // Reset error counter
    Serial.print("Distance: ");
    Serial.println(distance);
  } else {
    // Error
    errorCount++;
    Serial.print("Error count: ");
    Serial.println(errorCount);
    
    if (errorCount >= ERROR_THRESHOLD) {
      Serial.println("⚠️ ALERT: Too many consecutive errors!");
      Serial.println("Check sensor connection and power");
      // Take corrective action
    }
  }
  
  delay(100);
}
```

### Moving Average Filter

Smooth out noise and reject outliers:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

const int BUFFER_SIZE = 10;
float buffer[BUFFER_SIZE];
int bufferIndex = 0;
bool bufferFilled = false;

float getFilteredDistance() {
  float distance = sensor.read();
  
  // Only add valid readings
  if (distance > 0) {
    buffer[bufferIndex] = distance;
    bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;
    
    if (bufferIndex == 0) {
      bufferFilled = true;
    }
    
    // Calculate average
    int count = bufferFilled ? BUFFER_SIZE : bufferIndex;
    if (count == 0) return 0;
    
    float sum = 0;
    for (int i = 0; i < count; i++) {
      sum += buffer[i];
    }
    
    return sum / count;
  }
  
  return 0;  // Invalid reading
}

void loop() {
  float distance = getFilteredDistance();
  
  if (distance > 0) {
    Serial.print("Filtered distance: ");
    Serial.println(distance);
  } else {
    Serial.println("Initializing filter...");
  }
  
  delay(50);
}
```

## Error Recovery Strategies

### Strategy 1: Retry on Error

```cpp
float readWithRetry(MinimalUltrasonic& sensor, int maxRetries = 3) {
  for (int i = 0; i < maxRetries; i++) {
    float distance = sensor.read();
    
    if (distance > 0) {
      return distance;
    }
    
    delay(50);  // Wait before retry
  }
  
  return 0;  // All retries failed
}

void loop() {
  float distance = readWithRetry(sensor);
  
  if (distance > 0) {
    Serial.println(distance);
  } else {
    Serial.println("Failed after retries");
  }
  
  delay(500);
}
```

### Strategy 2: Use Last Known Good Value

```cpp
float lastValidDistance = 0;
const unsigned long STALE_THRESHOLD = 5000;  // 5 seconds
unsigned long lastValidTime = 0;

void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    // New valid reading
    lastValidDistance = distance;
    lastValidTime = millis();
    Serial.print("Current: ");
    Serial.println(distance);
  } else {
    // Use last known value if not too old
    if (millis() - lastValidTime < STALE_THRESHOLD) {
      Serial.print("Using cached: ");
      Serial.println(lastValidDistance);
    } else {
      Serial.println("Data too old, no valid reading");
    }
  }
  
  delay(100);
}
```

### Strategy 3: Fallback Mechanism

```cpp
enum SensorState {
  NORMAL,
  DEGRADED,
  FAILED
};

SensorState state = NORMAL;
int consecutiveErrors = 0;

void loop() {
  float distance = sensor.read();
  
  if (distance > 0) {
    consecutiveErrors = 0;
    state = NORMAL;
    Serial.print("Distance: ");
    Serial.println(distance);
  } else {
    consecutiveErrors++;
    
    if (consecutiveErrors > 10) {
      state = FAILED;
      Serial.println("❌ SENSOR FAILED - Switch to backup system");
      // Activate backup sensor or safe mode
    } else if (consecutiveErrors > 5) {
      state = DEGRADED;
      Serial.println("⚠️ DEGRADED MODE - Unreliable readings");
    } else {
      Serial.println("Temporary error");
    }
  }
  
  delay(100);
}
```

## Debugging Invalid Readings

### Diagnostic Function

```cpp
void diagnosticCheck(MinimalUltrasonic& sensor) {
  Serial.println("=== Sensor Diagnostic ===");
  
  // Test multiple readings
  int validCount = 0;
  int invalidCount = 0;
  float minDist = 9999;
  float maxDist = 0;
  float totalDist = 0;
  
  const int TEST_COUNT = 20;
  
  for (int i = 0; i < TEST_COUNT; i++) {
    float dist = sensor.read();
    
    if (dist > 0) {
      validCount++;
      totalDist += dist;
      if (dist < minDist) minDist = dist;
      if (dist > maxDist) maxDist = dist;
    } else {
      invalidCount++;
    }
    
    delay(50);
  }
  
  Serial.print("Valid readings: ");
  Serial.print(validCount);
  Serial.print("/");
  Serial.println(TEST_COUNT);
  
  Serial.print("Invalid readings: ");
  Serial.println(invalidCount);
  
  if (validCount > 0) {
    Serial.print("Average: ");
    Serial.println(totalDist / validCount);
    Serial.print("Min: ");
    Serial.println(minDist);
    Serial.print("Max: ");
    Serial.println(maxDist);
    Serial.print("Range: ");
    Serial.println(maxDist - minDist);
  }
  
  // Diagnosis
  if (validCount == 0) {
    Serial.println("⚠️ PROBLEM: No valid readings!");
    Serial.println("  - Check wiring");
    Serial.println("  - Check power supply");
    Serial.println("  - Verify pin numbers");
  } else if (invalidCount > TEST_COUNT / 2) {
    Serial.println("⚠️ WARNING: Many invalid readings");
    Serial.println("  - Check for interference");
    Serial.println("  - Adjust timeout");
    Serial.println("  - Check sensor orientation");
  } else if (maxDist - minDist > 50) {
    Serial.println("⚠️ WARNING: High variance");
    Serial.println("  - Use filtering");
    Serial.println("  - Check for vibrations");
  } else {
    Serial.println("✓ Sensor operating normally");
  }
  
  Serial.println("=========================");
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  diagnosticCheck(sensor);
}
```

## Common Error Patterns

### Error: Always Returns 0

**Possible Causes:**

- Wrong pin numbers
- Disconnected wiring
- Insufficient power
- Incorrect sensor type (3-pin vs 4-pin)

**Debug Code:**

```cpp
void setup() {
  Serial.begin(9600);
  
  // Test trigger pin
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  Serial.println("Trigger test complete");
  
  // Test echo pin
  pinMode(13, INPUT);
  Serial.print("Echo pin state: ");
  Serial.println(digitalRead(13));
}
```

### Error: Intermittent Invalid Readings

**Possible Causes:**

- Crosstalk from nearby sensors
- Electrical noise
- Poor connections
- Environmental interference

**Solution:**

```cpp
// Add delays between readings
float distance = sensor.read();
delay(50);  // Increase delay

// Or use filtering
float filtered = getMedianDistance();
```

### Error: Out of Range Values

**Possible Causes:**

- Objects beyond detection range
- Timeout too short
- Angled or soft surfaces

**Solution:**

```cpp
// Increase timeout
sensor.setTimeout(40000UL);  // 6.8m range

// Validate range
if (distance > 0 && distance < 400) {
  // Valid range for HC-SR04
  processDistance(distance);
}
```

## Best Practices

1. **Always Validate** - Check for `0` return value
2. **Use Filtering** - Median or moving average for stability
3. **Set Appropriate Timeouts** - Match your detection range
4. **Add Retry Logic** - Single failures are common
5. **Log Errors** - Track error patterns for diagnosis
6. **Test Thoroughly** - Run diagnostics during setup
7. **Handle Gracefully** - Use fallback values or safe modes

## Error Handling Checklist

✅ Check if `read()` returns 0  
✅ Validate reading is within expected range  
✅ Implement retry mechanism for transient errors  
✅ Use filtering to smooth out noise  
✅ Track error rates and patterns  
✅ Provide fallback behavior  
✅ Log errors for debugging  
✅ Test error handling code paths  

## Next Steps

- [Best Practices](/guide/best-practices) - Overall best practices
- [Troubleshooting](/guide/troubleshooting) - Solve specific problems
- [Multiple Sensors](/guide/multiple-sensors) - Handle multiple sensor errors

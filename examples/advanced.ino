/**
 * Advanced Usage Example - MinimalUltrasonic
 *
 * Demonstrates:
 * - Median filtering for stability
 * - Error handling and recovery
 * - Performance monitoring
 * - State management
 * - Diagnostic output
 */

#include <MinimalUltrasonic.h>

// Configuration
const uint8_t TRIG_PIN = 12;
const uint8_t ECHO_PIN = 13;
const uint8_t LED_PIN = LED_BUILTIN;

// Thresholds
const float MIN_VALID_DISTANCE = 2.0;   // cm
const float MAX_VALID_DISTANCE = 350.0; // cm
const float THRESHOLD_NEAR = 30.0;      // cm
const float THRESHOLD_FAR = 200.0;      // cm

// Create sensor
MinimalUltrasonic sensor(TRIG_PIN, ECHO_PIN);

// State tracking
enum State
{
  STATE_UNKNOWN,
  STATE_TOO_CLOSE,
  STATE_NEAR,
  STATE_NORMAL,
  STATE_FAR,
  STATE_NO_OBJECT
};

State currentState = STATE_UNKNOWN;
State previousState = STATE_UNKNOWN;

// Statistics
struct Statistics
{
  unsigned long totalReadings;
  unsigned long validReadings;
  unsigned long errorReadings;
  float minDistance;
  float maxDistance;
  float avgDistance;
  unsigned long startTime;
};

Statistics stats = {0, 0, 0, 999999.0, 0, 0, 0};

// Filtering
const int FILTER_SIZE = 5;
float filterBuffer[FILTER_SIZE];
int filterIndex = 0;
bool filterFilled = false;

/**
 * Get median filtered distance
 */
float getMedianDistance()
{
  float readings[FILTER_SIZE];
  int validCount = 0;

  // Collect readings
  for (int i = 0; i < FILTER_SIZE; i++)
  {
    float dist = sensor.read();

    if (dist > 0)
    {
      readings[validCount++] = dist;
    }

    delay(20);
  }

  // Need at least 3 valid readings
  if (validCount < 3)
  {
    return 0;
  }

  // Sort readings (bubble sort)
  for (int i = 0; i < validCount - 1; i++)
  {
    for (int j = i + 1; j < validCount; j++)
    {
      if (readings[i] > readings[j])
      {
        float temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }

  // Return median
  return readings[validCount / 2];
}

/**
 * Get moving average distance
 */
float getMovingAverage()
{
  float distance = sensor.read();

  if (distance > 0)
  {
    filterBuffer[filterIndex] = distance;
    filterIndex = (filterIndex + 1) % FILTER_SIZE;

    if (filterIndex == 0)
    {
      filterFilled = true;
    }
  }

  // Calculate average
  int count = filterFilled ? FILTER_SIZE : filterIndex;
  if (count == 0)
    return 0;

  float sum = 0;
  for (int i = 0; i < count; i++)
  {
    sum += filterBuffer[i];
  }

  return sum / count;
}

/**
 * Update statistics
 */
void updateStatistics(float distance)
{
  stats.totalReadings++;

  if (distance > 0)
  {
    stats.validReadings++;

    // Update min/max
    if (distance < stats.minDistance)
    {
      stats.minDistance = distance;
    }
    if (distance > stats.maxDistance)
    {
      stats.maxDistance = distance;
    }

    // Update running average
    stats.avgDistance =
        (stats.avgDistance * (stats.validReadings - 1) + distance) /
        stats.validReadings;
  }
  else
  {
    stats.errorReadings++;
  }
}

/**
 * Determine state from distance
 */
State determineState(float distance)
{
  if (distance == 0)
  {
    return STATE_NO_OBJECT;
  }

  if (distance < MIN_VALID_DISTANCE)
  {
    return STATE_TOO_CLOSE;
  }

  if (distance < THRESHOLD_NEAR)
  {
    return STATE_NEAR;
  }

  if (distance < THRESHOLD_FAR)
  {
    return STATE_NORMAL;
  }

  if (distance <= MAX_VALID_DISTANCE)
  {
    return STATE_FAR;
  }

  return STATE_NO_OBJECT;
}

/**
 * Get state name
 */
const char *getStateName(State state)
{
  switch (state)
  {
  case STATE_TOO_CLOSE:
    return "TOO CLOSE";
  case STATE_NEAR:
    return "NEAR";
  case STATE_NORMAL:
    return "NORMAL";
  case STATE_FAR:
    return "FAR";
  case STATE_NO_OBJECT:
    return "NO OBJECT";
  default:
    return "UNKNOWN";
  }
}

/**
 * Handle state change
 */
void onStateChange(State oldState, State newState)
{
  Serial.println();
  Serial.print("State changed: ");
  Serial.print(getStateName(oldState));
  Serial.print(" -> ");
  Serial.println(getStateName(newState));

  // Visual indication
  if (newState == STATE_NEAR || newState == STATE_TOO_CLOSE)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}

/**
 * Print statistics
 */
void printStatistics()
{
  float successRate =
      (stats.totalReadings > 0) ? (float)stats.validReadings / stats.totalReadings * 100.0 : 0;

  unsigned long elapsed = (millis() - stats.startTime) / 1000;

  Serial.println();
  Serial.println("=== Statistics ===");
  Serial.print("Runtime: ");
  Serial.print(elapsed);
  Serial.println(" seconds");

  Serial.print("Total readings: ");
  Serial.println(stats.totalReadings);

  Serial.print("Valid: ");
  Serial.print(stats.validReadings);
  Serial.print(" (");
  Serial.print(successRate, 1);
  Serial.println("%)");

  Serial.print("Errors: ");
  Serial.println(stats.errorReadings);

  if (stats.validReadings > 0)
  {
    Serial.print("Min distance: ");
    Serial.print(stats.minDistance, 1);
    Serial.println(" cm");

    Serial.print("Max distance: ");
    Serial.print(stats.maxDistance, 1);
    Serial.println(" cm");

    Serial.print("Avg distance: ");
    Serial.print(stats.avgDistance, 1);
    Serial.println(" cm");
  }

  Serial.println("==================");
  Serial.println();
}

/**
 * Diagnostic check
 */
void runDiagnostics()
{
  Serial.println("Running diagnostics...");
  Serial.println();

  // Test configuration
  Serial.println("Configuration:");
  Serial.print("  Timeout: ");
  Serial.print(sensor.getTimeout());
  Serial.println(" µs");

  float maxRange = sensor.getTimeout() / 58.8235;
  Serial.print("  Max range: ");
  Serial.print(maxRange, 1);
  Serial.println(" cm");

  Serial.print("  Unit: ");
  switch (sensor.getUnit())
  {
  case MinimalUltrasonic::CM:
    Serial.println("CM");
    break;
  case MinimalUltrasonic::METERS:
    Serial.println("METERS");
    break;
  case MinimalUltrasonic::MM:
    Serial.println("MM");
    break;
  case MinimalUltrasonic::INCHES:
    Serial.println("INCHES");
    break;
  default:
    Serial.println("Other");
  }

  Serial.println();

  // Test readings
  Serial.println("Testing 10 readings:");
  int successCount = 0;

  for (int i = 0; i < 10; i++)
  {
    float dist = sensor.read();

    Serial.print("  ");
    Serial.print(i + 1);
    Serial.print(". ");

    if (dist > 0)
    {
      Serial.print("✓ ");
      Serial.print(dist, 1);
      Serial.println(" cm");
      successCount++;
    }
    else
    {
      Serial.println("✗ Error");
    }

    delay(100);
  }

  Serial.println();
  Serial.print("Success rate: ");
  Serial.print(successCount);
  Serial.println("/10");

  if (successCount >= 8)
  {
    Serial.println("✓ Sensor operating normally");
  }
  else if (successCount >= 5)
  {
    Serial.println("⚠️  Sensor may have issues");
  }
  else
  {
    Serial.println("❌ Sensor appears faulty");
  }

  Serial.println();
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("MinimalUltrasonic - Advanced Example");
  Serial.println("====================================");
  Serial.println();

  // Configure sensor
  sensor.setUnit(MinimalUltrasonic::CM);
  sensor.setTimeout(20000UL);

  // Run diagnostics
  runDiagnostics();

  // Initialize statistics
  stats.startTime = millis();

  Serial.println("Starting monitoring...");
  Serial.println("Type 's' for statistics");
  Serial.println();
}

void loop()
{
  // Get filtered distance
  float distance = getMedianDistance();

  // Update statistics
  updateStatistics(distance);

  // Determine state
  currentState = determineState(distance);

  // Handle state change
  if (currentState != previousState)
  {
    onStateChange(previousState, currentState);
  }
  previousState = currentState;

  // Display reading
  Serial.print(getStateName(currentState));
  Serial.print(": ");

  if (distance > 0)
  {
    Serial.print(distance, 1);
    Serial.println(" cm");
  }
  else
  {
    Serial.println("---");
  }

  // Check for statistics request
  if (Serial.available())
  {
    char cmd = Serial.read();
    if (cmd == 's' || cmd == 'S')
    {
      printStatistics();
    }
  }

  delay(500);
}
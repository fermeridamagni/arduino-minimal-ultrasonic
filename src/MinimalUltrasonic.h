/*
 * @file MinimalUltrasonic.h
 * @brief Minimalist library for ultrasonic ranging sensors (HC-SR04, Ping, Seeed SEN136B5B)
 * @version 2.0.0
 * @date 25 Oct 2025
 * @author fermeridamagni (Magni Development)
 * 
 * @details This library provides a clean, efficient interface for ultrasonic distance sensors.
 *          It supports both 3-pin (Ping, Seeed) and 4-pin (HC-SR04) sensor configurations.
 *          Features include:
 *          - Multiple unit measurements (cm, m, mm, inches, yards, miles)
 *          - Configurable timeout for max range control
 *          - Minimal memory footprint
 *          - Support for multiple sensors
 * 
 * @license MIT License
 * 
 * @example
 * // 4-pin sensor (HC-SR04)
 * MinimalUltrasonic sensor(12, 13);
 * float distance = sensor.read();  // Returns distance in cm (default)
 * 
 * @example
 * // 3-pin sensor (Ping, Seeed)
 * MinimalUltrasonic sensor(13);
 * float distanceInches = sensor.read(MinimalUltrasonic::INCHES);
 */

#ifndef MinimalUltrasonic_h
#define MinimalUltrasonic_h

#include <Arduino.h>

/**
 * @class MinimalUltrasonic
 * @brief Main class for ultrasonic distance measurement
 * 
 * This class provides methods to measure distance using ultrasonic sensors.
 * It works with both 3-pin and 4-pin sensor configurations and supports
 * multiple units of measurement.
 */
class MinimalUltrasonic
{
public:
  /**
   * @enum Unit
   * @brief Available units of measurement for distance readings
   */
  enum Unit : uint8_t
  {
    CM = 0,      ///< Centimeters (default)
    METERS = 1,  ///< Meters
    MM = 2,      ///< Millimeters
    INCHES = 3,  ///< Inches
    YARDS = 4,   ///< Yards
    MILES = 5    ///< Miles
  };

  // Backward compatibility with old defines
  static const uint8_t INC = INCHES;  ///< Legacy support for INC constant

  /**
   * @brief Constructor for 3-pin ultrasonic sensors (Ping, Seeed SEN136B5B)
   * @param sigPin Digital pin number for the signal (combined trigger/echo)
   * 
   * Use this constructor when your sensor has a single signal pin that
   * handles both trigger and echo functions.
   */
  MinimalUltrasonic(uint8_t sigPin);

  /**
   * @brief Constructor for 4-pin ultrasonic sensors (HC-SR04)
   * @param trigPin Digital pin number for the trigger output
   * @param echoPin Digital pin number for the echo input
   * @param timeOut Maximum time to wait for echo response in microseconds (default: 20000µs ≈ 3.4m range)
   * 
   * The timeout value determines the maximum detectable distance:
   * - 20000µs (default) ≈ 3.4 meters
   * - 30000µs ≈ 5.1 meters
   * - 40000µs ≈ 6.8 meters
   */
  MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut = 20000UL);

  /**
   * @brief Read the distance from the ultrasonic sensor
   * @param unit The unit of measurement (default: CM)
   * @return Distance in the specified unit, or 0 if timeout/error
   * 
   * This method triggers the sensor, waits for the echo, and calculates
   * the distance based on the time of flight. Returns 0 if no echo is
   * received within the timeout period.
   * 
   * @example
   * float distCm = sensor.read();                      // Distance in cm
   * float distM = sensor.read(MinimalUltrasonic::METERS);   // Distance in meters
   * float distIn = sensor.read(MinimalUltrasonic::INCHES);  // Distance in inches
   */
  float read(Unit unit = CM) const;

  /**
   * @brief Set the timeout for echo response
   * @param timeOut Maximum time to wait for echo in microseconds
   * 
   * Use this to adjust the maximum detectable range. Longer timeouts
   * allow for greater distances but may slow down readings if no object
   * is detected.
   * 
   * @example
   * sensor.setTimeout(40000UL);  // ~6.8m max range
   */
  void setTimeout(unsigned long timeOut);

  /**
   * @brief Set maximum detection distance (automatically calculates timeout)
   * @param distance Maximum distance in centimeters
   * 
   * This is a convenience method that calculates the appropriate timeout
   * based on the desired maximum distance.
   * 
   * @example
   * sensor.setMaxDistance(500);  // 5 meters max range
   */
  void setMaxDistance(unsigned int distance);

  /**
   * @brief Get the current timeout value
   * @return Current timeout in microseconds
   */
  unsigned long getTimeout() const;

  /**
   * @brief Get the current unit of measurement
   * @return Current default unit
   */
  Unit getUnit() const;

  /**
   * @brief Set the default unit of measurement for future read() calls
   * @param unit The unit to use as default
   * 
   * @example
   * sensor.setUnit(MinimalUltrasonic::METERS);
   * float dist = sensor.read();  // Now returns meters by default
   */
  void setUnit(Unit unit);

private:
  uint8_t _trigPin;              ///< Trigger pin number
  uint8_t _echoPin;              ///< Echo pin number
  bool _isThreePin;              ///< True if using 3-pin sensor configuration
  unsigned long _timeout;        ///< Timeout in microseconds
  Unit _defaultUnit;             ///< Default unit for measurements

  /**
   * @brief Perform the ultrasonic timing measurement
   * @return Time in microseconds for the echo to return, or 0 on timeout
   * 
   * This method sends a trigger pulse and measures the time until the
   * echo is received. It handles both 3-pin and 4-pin configurations.
   */
  unsigned long timing() const;

  /**
   * @brief Convert raw microseconds to the specified unit
   * @param microseconds Time of flight in microseconds
   * @param unit Target unit of measurement
   * @return Distance in the specified unit
   * 
   * Uses the speed of sound (343 m/s at 20°C) to calculate distance.
   * Formula: distance = (time * speed_of_sound) / 2
   */
  float convertToUnit(unsigned long microseconds, Unit unit) const;
};

// Legacy compatibility - Old defines for backward compatibility
#define CM MinimalUltrasonic::CM
#define INC MinimalUltrasonic::INCHES

#endif // MinimalUltrasonic_h

/*
 * @file MinimalUltrasonic.h
 * @brief Library for Ultrasonic Ranging Module in a minimalist way
 * @date 25 Oct 2025
 * @author fermeridamagni (Magni Development)
 */

#ifndef MinimalUltrasonic_h
#define MinimalUltrasonic_h

/*
 * Values of divisors
 */
#define CM 28
#define INC 71

class MinimalUltrasonic
{
public:
  MinimalUltrasonic(uint8_t sigPin) : MinimalUltrasonic(sigPin, sigPin) {}
  MinimalUltrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut = 20000UL);
  unsigned int read(uint8_t und = CM);
  void setTimeout(unsigned long timeOut) { timeout = timeOut; }
  void setMaxDistance(unsigned long dist) { timeout = dist * CM * 2; }

private:
  uint8_t trig;
  uint8_t echo;
  boolean threePins = false;
  unsigned long previousMicros;
  unsigned long timeout;
  unsigned int timing();
};

#endif // MinimalUltrasonic_h

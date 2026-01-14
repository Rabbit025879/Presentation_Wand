#ifndef HAPTICS_H
#define HAPTICS_H

#include <Arduino.h>

#define HAPTICS_DEFAULT_INTENSITY 125
#define HAPTICS_PULSE_END 0 // End marker for pattern
#define UNIT_TIME_MS 100

struct PulsePattern {
  uint8_t duration;   // in units of 100ms
  uint8_t pause;      // in units of 100ms
  uint8_t intensity;   // 0-255

  constexpr PulsePattern() 
    : duration(UNIT_TIME_MS), pause(UNIT_TIME_MS), intensity(HAPTICS_DEFAULT_INTENSITY) {}
    
  constexpr PulsePattern(uint8_t d, uint8_t p, uint8_t i)
    : duration(d), pause(p), intensity(i) {}
};

class Haptics {
public:
  Haptics(uint8_t pin);
  ~Haptics();
  
  bool isBusy();
  
  void startVibrate(uint8_t intensity = HAPTICS_DEFAULT_INTENSITY);
  void stopVibrate();
  
  void shortVibrate(uint8_t cycles = 1, uint8_t intensity = HAPTICS_DEFAULT_INTENSITY);
  void longVibrate(uint8_t cycles = 1, uint8_t intensity = HAPTICS_DEFAULT_INTENSITY);
  // pattern is an array of durations in units of 100ms
  // Positive values: vibrate duration, Negative values: pause duration
  // HAPTICS_PULSE_END(0) is reserved as the end marker
  void emitPulses(PulsePattern* pattern, uint8_t cycles = 1);
  void emitPulses(uint8_t intensity, uint8_t duration = 1, uint8_t cycles = 1, uint8_t pause = 1);
  void emitPulses(uint8_t duration = 1, uint8_t cycles = 1, uint8_t pause = 1);

private:
  uint8_t _pin;
  bool _isVibrating = false;
  bool _isPulsing = false;
  uint32_t _startPulseTime = 0;
  uint8_t _patternIndicator = 0;
  uint8_t _currentCycle = 1;

  // uint8_t _duration = 1;
  // uint8_t _cycles = 1;
  // uint8_t _pause = 1;
  PulsePattern* _pattern = new PulsePattern[1]; 
  bool _pulse(uint32_t startPulseTime, uint8_t intensity, uint8_t duration, uint8_t pause);
};

#endif // HAPTICS_H
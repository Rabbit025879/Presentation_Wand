#ifndef OUTPUTDEVICE_H
#define OUTPUTDEVICE_H

#include <Arduino.h>

#define DEFAULT_INTENSITY 125
#define PULSE_END_MARK 0 // End marker for pattern
#define UNIT_TIME_MS 100

struct PulsePattern {
  uint8_t duration;   // in units of 100ms
  uint8_t pause;      // in units of 100ms
  uint8_t intensity;   // 0-255

  constexpr PulsePattern() 
    : duration(UNIT_TIME_MS), pause(UNIT_TIME_MS), intensity(DEFAULT_INTENSITY) {}
    
  constexpr PulsePattern(uint8_t d, uint8_t p, uint8_t i)
    : duration(d), pause(p), intensity(i) {}
};

class OutputDevice {
public:
  OutputDevice(uint8_t pin);
  ~OutputDevice();
  
  bool isBusy();
  
  void turnOn(uint8_t intensity = DEFAULT_INTENSITY);
  void turnOff();
  
  void shortPulse(uint8_t cycles = 1, uint8_t intensity = DEFAULT_INTENSITY);
  void longPulse(uint8_t cycles = 1, uint8_t intensity = DEFAULT_INTENSITY);
  // pattern is an array of durations in units of 100ms
  // Positive values: vibrate duration, Negative values: pause duration
  // PULSE_END_MARK(0) is reserved as the end marker
  void emitPulses(PulsePattern* pattern, uint8_t cycles = 1);
  void emitPulses(uint8_t intensity, uint8_t duration = 1, uint8_t cycles = 1, uint8_t pause = 1);
  void emitPulses(uint8_t duration = 1, uint8_t cycles = 1, uint8_t pause = 1);

private:
  uint8_t _pin;
  bool _isOn = false;
  bool _isPulsing = false;
  uint32_t _startPulseTime = 0;
  uint8_t _patternIndicator = 0;
  uint8_t _currentCycle = 1;

  PulsePattern* _pattern = new PulsePattern[1]; 
  bool _pulse(uint32_t startPulseTime, uint8_t intensity, uint8_t duration, uint8_t pause);
};

#endif // OUTPUTDEVICE_H
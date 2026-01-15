#include "OutputDevice.h"

OutputDevice::OutputDevice(uint8_t pin) : _pin(pin) {
  pinMode(_pin, OUTPUT);
  analogWrite(_pin, LOW);
}

OutputDevice::~OutputDevice() {
  delete[] _pattern;
}

void OutputDevice::turnOn(uint8_t intensity) {
  analogWrite(_pin, intensity);
  _isOn = true;
}

void OutputDevice::turnOff() {
  analogWrite(_pin, LOW);
  _isOn = false;
}

bool OutputDevice::isBusy() {
  return _isOn ? true : _isPulsing;
}

void OutputDevice::shortPulse(uint8_t cycles, uint8_t intensity) {
  emitPulses(intensity, 2, cycles, 1);
}

void OutputDevice::longPulse(uint8_t cycles, uint8_t intensity) {
  emitPulses(intensity, 5, cycles, 1);
}

void OutputDevice::emitPulses(PulsePattern* pattern, uint8_t cycles) {
  if(!_isPulsing) {
    _startPulseTime = millis();
    _isPulsing = true;
    _pattern = pattern; // Copy pattern to avoid call by reference issues
  } else {
    if(_currentCycle > cycles) {
      // Finished all cycles
      _isPulsing = false;
      _currentCycle = 1;
      return;
    } else if(_currentCycle == cycles) {
      if(_pattern[_patternIndicator+1].duration == PULSE_END_MARK) {
        _pattern[_patternIndicator].pause = 0; // No pause after last pulse
      }
    }
    if(_pulse(_startPulseTime, _pattern[_patternIndicator].intensity, _pattern[_patternIndicator].duration, _pattern[_patternIndicator].pause)) {
      // Move to next pattern element
      _startPulseTime = millis();
      if(_pattern[++_patternIndicator].duration == PULSE_END_MARK) {
        _currentCycle++;
        _patternIndicator = 0; // Reset to start of pattern
      }
    } 
  }
}

void OutputDevice::emitPulses(uint8_t intensity, uint8_t duration, uint8_t cycles, uint8_t pause) {
  PulsePattern* pattern = new PulsePattern[2]{{duration, pause, intensity}, {PULSE_END_MARK, 0, 0}}; // End marker
  emitPulses(pattern, cycles);
}

void OutputDevice::emitPulses(uint8_t duration, uint8_t cycles, uint8_t pause) {
  emitPulses(DEFAULT_INTENSITY, duration, cycles, pause);
}

bool OutputDevice::_pulse(uint32_t startPulseTime, uint8_t intensity, uint8_t duration, uint8_t pause) {
  uint32_t currentTime = millis();
  uint32_t elapsedTime = currentTime - startPulseTime;

  if(elapsedTime < (duration + pause)*UNIT_TIME_MS) {
    if(elapsedTime < duration*UNIT_TIME_MS) {
      // Vibrate
      turnOn(intensity);
    } else {
      // Pause
      turnOff();
    }
  } else {  
    // Finished pulsing
    turnOff();
    return true;
  }
  return false;
}
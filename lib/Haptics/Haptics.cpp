#include "Haptics.h"

Haptics::Haptics(uint8_t pin) : _pin(pin) {
  pinMode(_pin, OUTPUT);
  analogWrite(_pin, LOW);
}

Haptics::~Haptics() {
  delete[] _pattern;
}

void Haptics::startVibrate(uint8_t intensity) {
  analogWrite(_pin, intensity);
  _isVibrating = true;
}

void Haptics::stopVibrate() {
  analogWrite(_pin, LOW);
  _isVibrating = false;
}

bool Haptics::isBusy() {
  return _isVibrating ? true : _isPulsing;
}

void Haptics::shortVibrate(uint8_t cycles, uint8_t intensity) {
  emitPulses(intensity, 2, cycles, 1);
}

void Haptics::longVibrate(uint8_t cycles, uint8_t intensity) {
  emitPulses(intensity, 5, cycles, 1);
}

void Haptics::emitPulses(PulsePattern* pattern, uint8_t cycles) {
#if defined(DEBUG_MODE)    
  if(intensity != _intensity || duration != _duration || cycles != _cycles || pause != _pause) {
    Serial.println("Haptics pulse parameters changed during operation!");
    Serial.println("Last pulse call will take precedence.");
  }
#endif // DEBUG_MODE

  if(!_isPulsing) {
    _startPulseTime = millis();
    _isPulsing = true;
    
    // Copy pattern to avoid changing during pulse
    _pattern = pattern;
    // _cycles = cycles;
  } else {
    if(_currentCycle > cycles) {
      // Finished all cycles
      _isPulsing = false;
      _currentCycle = 1;
      return;
    } else if(_currentCycle == cycles) {
      if(_pattern[_patternIndicator+1].duration == HAPTICS_PULSE_END) {
        _pattern[_patternIndicator].pause = 0; // No pause after last pulse
      }
    }
    if(_pulse(_startPulseTime, _pattern[_patternIndicator].intensity, _pattern[_patternIndicator].duration, _pattern[_patternIndicator].pause)) {
      // Move to next pattern element
      _startPulseTime = millis();
      if(_pattern[++_patternIndicator].duration == HAPTICS_PULSE_END) {
        _currentCycle++;
        _patternIndicator = 0; // Reset to start of pattern
      }
    } 
  }
}

void Haptics::emitPulses(uint8_t intensity, uint8_t duration, uint8_t cycles, uint8_t pause) {
  PulsePattern* pattern = new PulsePattern[2]{{duration, pause, intensity}, {HAPTICS_PULSE_END, 0, 0}}; // End marker
  emitPulses(pattern, cycles);
}

void Haptics::emitPulses(uint8_t duration, uint8_t cycles, uint8_t pause) {
  emitPulses(HAPTICS_DEFAULT_INTENSITY, duration, cycles, pause);
}

bool Haptics::_pulse(uint32_t startPulseTime, uint8_t intensity, uint8_t duration, uint8_t pause) {
  uint32_t currentTime = millis();
  uint32_t elapsedTime = currentTime - startPulseTime;

  if(elapsedTime < (duration + pause)*UNIT_TIME_MS) {
    if(elapsedTime < duration*UNIT_TIME_MS) {
      // Vibrate
      startVibrate(intensity);
    } else {
      // Pause
      stopVibrate();
    }
  } else {  
    // Finished pulsing
    stopVibrate();
    return true;
  }
  return false;
}
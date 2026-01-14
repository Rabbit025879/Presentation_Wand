#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define BUTTON_DEBOUNCE_DEFAULT 50 // milliseconds
#define BUTTON_PRESS_WAIT_TIME 500 // milliseconds
#define BUTTON_SHORT_PRESS_THRESHOLD 200 // milliseconds

enum class ButtonEvent {
  None,
  SingleClick,
  DoubleClick,
  TripleClick,
  LongPress,
  Hold,
  OtherPattern
};

class Button {
public:
  Button(uint8_t pin, uint16_t debounceTime = BUTTON_DEBOUNCE_DEFAULT);
  ButtonEvent getEvent();
  ButtonEvent getEvent(uint8_t* pattern); // TODO: Implement pattern output
  bool _isPressed();
  bool _isReleased();

private:
  uint8_t _pin;
  ButtonEvent _currentEvent = ButtonEvent::None;
  uint32_t _eventTriggerTime = 0;
  void _resetEvent() {
    _currentEvent = ButtonEvent::None;
    _eventTriggerTime = 0;
  }

  uint32_t _pressStartTime = 0;
  bool _completePressing(uint32_t& pressedTime);
  
  bool _isPressedState = false;
  uint16_t _debounceTime; // milliseconds
  uint32_t _lastDebounceTime = 0;
};

#endif // BUTTON_H
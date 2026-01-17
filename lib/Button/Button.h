#ifndef BUTTON_H
#define BUTTON_H

#include "Utils.h"

class Button {
public:
  Button(uint8_t pin, uint16_t debounceTime = BUTTON_DEBOUNCE_DEFAULT);
  ButtonState getState();
  ButtonState getState(uint8_t* pattern); // TODO: Implement pattern output

private:
  uint8_t _pin;
  ButtonState _currentState = ButtonState();
  uint32_t _eventTriggerTime = 0;
  void _resetEvent() {
    _currentState = ButtonState(_isPressedState, ButtonEvent::None);
    _eventTriggerTime = 0;
  }

  uint32_t _pressStartTime = 0;
  /*  Return true if pressing process is complete, false otherwise.
      pressedTime: If complete, returns the total pressed time in milliseconds.
                   If not complete, returns the pressing start time in milliseconds.
  */
  bool _completePressing(uint32_t& pressedTime);
  
  bool _isPressedState = false;
  uint16_t _debounceTime; // milliseconds
  uint32_t _lastDebounceTime = 0;
  bool _fallingEdgeDetected();
  bool _risingEdgeDetected();
};

#endif // BUTTON_H
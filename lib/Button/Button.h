#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define BUTTON_DEBOUNCE_DEFAULT 50 // milliseconds
#define BUTTON_PRESS_WAIT_TIME 700 // milliseconds
#define BUTTON_SHORT_PRESS_THRESHOLD 200 // milliseconds

enum class ButtonEvent {
  None,
  SingleClick,
  DoubleClick,
  TripleClick,
  SingleLongPress,
  DoubleLongPress,
  TripleLongPress,
  Hold,
  OtherPattern
};

struct ButtonState {
  bool isPressed;
  ButtonEvent event;

  ButtonState() : isPressed(false), event(ButtonEvent::None) {}
  ButtonState(bool pressed, ButtonEvent evt) : isPressed(pressed), event(evt) {}
};

class Button {
public:
  Button(uint8_t pin, uint16_t debounceTime = BUTTON_DEBOUNCE_DEFAULT);
  ButtonEvent getEvent();
  ButtonEvent getEvent(uint8_t* pattern); // TODO: Implement pattern output
  bool isPressed() {
    return _isPressedState;
  }

private:
  uint8_t _pin;
  ButtonEvent _currentEvent = ButtonEvent::None;
  uint32_t _eventTriggerTime = 0;
  void _resetEvent() {
    _currentEvent = ButtonEvent::None;
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
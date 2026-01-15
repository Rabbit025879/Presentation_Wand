#include "Button.h"

Button::Button(uint8_t pin, uint16_t debounceTime) : _pin(pin), _debounceTime(debounceTime) {
  pinMode(_pin, INPUT_PULLUP);
}

ButtonEvent Button::getEvent() {
  return getEvent(nullptr);
}

ButtonEvent Button::getEvent(uint8_t* pattern) {
  uint32_t pressedTime = 0;
  if(!_completePressing(pressedTime)) {
    // If still pressing and hold time exceeded, trigger Hold event and return
    if(millis() - pressedTime > BUTTON_PRESS_WAIT_TIME && pressedTime != 0) {
      _currentEvent = ButtonEvent::Hold;
      return _currentEvent;
    }
  } else {
    // If pressing complete, determine event type
    // If already in Hold state, reset to None
    if(_currentEvent == ButtonEvent::Hold) {
      _resetEvent();
      return ButtonEvent::None;
    }
    _eventTriggerTime = millis();
    // Determine single/multi click or long press, but do not return yet
    if(pressedTime <= BUTTON_SHORT_PRESS_THRESHOLD) {
      // Determine click pattern
      switch(_currentEvent) {
        case ButtonEvent::None:
          _currentEvent = ButtonEvent::SingleClick;
          break;
        case ButtonEvent::SingleClick:
          _currentEvent = ButtonEvent::DoubleClick;
          break;
        case ButtonEvent::DoubleClick:
          _currentEvent = ButtonEvent::TripleClick;
          break;
        default:
          _currentEvent = ButtonEvent::OtherPattern;
          break;
      }
    } else {
      if(_currentEvent == ButtonEvent::None)  _currentEvent = ButtonEvent::LongPress;
      else                                    _currentEvent = ButtonEvent::OtherPattern;
    }
  }
  // If wait time exceeded since last event trigger, return the event
  if(millis() - _eventTriggerTime > BUTTON_PRESS_WAIT_TIME && _eventTriggerTime != 0) {
    ButtonEvent eventToReturn = _currentEvent;
    _resetEvent();
    return eventToReturn;
  }

  return ButtonEvent::None;
}

bool Button::_completePressing(uint32_t& pressedTime) {
  if(isReleased() && _pressStartTime != 0) {
    pressedTime = millis() - _pressStartTime; // Pressing process complete time
    _pressStartTime = 0; // Reset press start time
    return true;
  }
  if(isPressed()) {
    _pressStartTime = millis();
  }
  if(_pressStartTime) pressedTime = _pressStartTime;  // Pressing process initiate time
  else pressedTime = 0;
  return false;
}

// Detect the falling edge(pull-up button) of the button press
bool Button::isPressed() {
  // Debounce
  if(_isPressedState == false && digitalRead(_pin) == LOW) {
    if((millis() - _lastDebounceTime) > _debounceTime) {
      _isPressedState = true;
      _lastDebounceTime = millis();
      return true;
    }
  }
  return false;
}

// Detect the rising edge(pull-up button) of the button release
bool Button::isReleased() {
  // Debounce
  if(_isPressedState == true && digitalRead(_pin) == HIGH) {
    if((millis() - _lastDebounceTime) > _debounceTime) {
      _isPressedState = false;
      _lastDebounceTime = millis();
      return true;
    }
  }
  return false;
}
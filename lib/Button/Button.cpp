#include "Button.h"

Button::Button(uint8_t pin, uint16_t debounceTime) : _pin(pin), _debounceTime(debounceTime) {
  pinMode(_pin, INPUT_PULLUP);
}

ButtonState Button::getState() {
  return getState(nullptr);
}

ButtonState Button::getState(uint8_t* pattern) {
  uint32_t pressedTime = 0;
  if(!_completePressing(pressedTime)) {
    // If still pressing and hold time exceeded, trigger Hold event and return
    if(millis() - pressedTime > BUTTON_PRESS_WAIT_TIME && pressedTime != 0) {
      _currentState = ButtonState(_isPressedState, ButtonEvent::Hold);
      return _currentState;
    }
  } else {
    // If pressing complete, determine event type
    // If already in Hold state, reset to None
    if(_currentState.event == ButtonEvent::Hold) {
      _resetEvent();
      return ButtonState(_isPressedState, ButtonEvent::None);
    }
    _eventTriggerTime = millis();
    // Determine single/multi click or long press, but do not return yet
    if(pressedTime <= BUTTON_SHORT_PRESS_THRESHOLD) {
      // Determine click pattern
      switch(_currentState.event) {
        case ButtonEvent::None:
          _currentState.event = ButtonEvent::SingleClick;
          break;
        case ButtonEvent::SingleClick:
          _currentState.event = ButtonEvent::DoubleClick;
          break;
        case ButtonEvent::DoubleClick:
          _currentState.event = ButtonEvent::TripleClick;
          break;
        default:
          _currentState.event = ButtonEvent::OtherPattern;
          break;
      }
    } else {
      switch (_currentState.event) {
      case ButtonEvent::None:
        _currentState.event = ButtonEvent::SingleLongPress;
        break;
      case ButtonEvent::SingleLongPress:
        _currentState.event = ButtonEvent::DoubleLongPress;
        break;
      case ButtonEvent::DoubleLongPress:
        _currentState.event = ButtonEvent::TripleLongPress;
        break;
      default:
        _currentState.event = ButtonEvent::OtherPattern;
        break;
      }
    }
  }
  // If wait time exceeded since last event trigger, return the event
  if(millis() - _eventTriggerTime > BUTTON_PRESS_WAIT_TIME && _eventTriggerTime != 0) {
    ButtonState eventToReturn = ButtonState(_isPressedState, _currentState.event);
    _resetEvent();
    return eventToReturn;
  }

  return ButtonState(_isPressedState, ButtonEvent::None);
}

bool Button::_completePressing(uint32_t& pressedTime) {
  if(_risingEdgeDetected() && _pressStartTime != 0) {
    pressedTime = millis() - _pressStartTime; // Pressing process complete time
    _pressStartTime = 0; // Reset press start time
    return true;
  }
  if(_fallingEdgeDetected()) {
    _pressStartTime = millis();
  }
  if(_pressStartTime) pressedTime = _pressStartTime;  // Pressing process initiate time
  else pressedTime = 0;
  return false;
}

// Detect the falling edge(pull-up button) of the button press
bool Button::_fallingEdgeDetected() {
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
bool Button::_risingEdgeDetected() {
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
#include "MultiButton.h"

MultiButton::MultiButton(uint8_t pointer_pin, uint8_t thumbs_up_pin, uint8_t thumbs_down_pin)
  : pointer_button(pointer_pin),
    thumbs_up_button(thumbs_up_pin),
    thumbs_down_button(thumbs_down_pin),
    last_pointer_state(ButtonState()),
    last_thumbs_up_state(ButtonState()),
    last_thumbs_down_state(ButtonState()) {
}

MultiButtonState MultiButton::getState() {
  ButtonState pointer_state = pointer_button.getState();
  ButtonState thumbs_up_state = thumbs_up_button.getState();
  ButtonState thumbs_down_state = thumbs_down_button.getState();
  
  MultiButtonState event(pointer_state, thumbs_up_state, thumbs_down_state);
  
  last_pointer_state = pointer_state;
  last_thumbs_up_state = thumbs_up_state;
  last_thumbs_down_state = thumbs_down_state;
  
  return event;
}

bool MultiButton::bothThumbButtonsHold() {
  if(last_thumbs_up_state.event == ButtonEvent::Hold &&
     last_thumbs_down_state.event == ButtonEvent::Hold && !_last_both_thumbs_hold) {
    _last_both_thumbs_hold = true;
    return true;
  }
  if(last_thumbs_up_state.event != ButtonEvent::Hold ||
     last_thumbs_down_state.event != ButtonEvent::Hold) {
    _last_both_thumbs_hold = false;
  }
  return false;
}

bool MultiButton::allThreeButtonsHold() {
  if(last_pointer_state.event == ButtonEvent::Hold && 
     last_thumbs_up_state.event == ButtonEvent::Hold && 
     last_thumbs_down_state.event == ButtonEvent::Hold && !_last_all_three_hold) {
    _last_all_three_hold = true;
    return true;
  }
  if(last_pointer_state.event != ButtonEvent::Hold || 
    last_thumbs_up_state.event != ButtonEvent::Hold || 
    last_thumbs_down_state.event != ButtonEvent::Hold) {
    _last_all_three_hold = false;
  }
  return false;
}

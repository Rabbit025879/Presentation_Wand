#ifndef MULTI_BUTTON_H
#define MULTI_BUTTON_H

#include "Button.h"
#include "Utils.h"

class MultiButton {
public:
  MultiButton(uint8_t pointer_pin = POINTER_BUTTON_PIN, 
              uint8_t thumbs_up_pin = THUMBS_UP_BUTTON_PIN,
              uint8_t thumbs_down_pin = THUMBS_DOWN_BUTTON_PIN);
  
  MultiButtonState getState();
  bool bothThumbButtonsHold(); // Check if both thumbs up/down pressed simultaneously
  bool allThreeButtonsHold();  // Check if all three buttons pressed simultaneously

private:
  Button pointer_button;
  Button thumbs_up_button;
  Button thumbs_down_button;
  
  ButtonState last_pointer_state;
  ButtonState last_thumbs_up_state;
  ButtonState last_thumbs_down_state;

  bool _last_both_thumbs_hold = false;
  bool _last_all_three_hold = false;
};

#endif // MULTI_BUTTON_H

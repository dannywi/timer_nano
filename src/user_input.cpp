#include "user_input.hpp"

#include "Arduino.h"
#include "pins.hpp"

namespace tm {
millis_t last_rotary_t{0};
int CLK_curr{0};
int CLK_last{0};

USER_INPUT get_user_input(millis_t millis) {
  USER_INPUT ret{USER_INPUT::NONE};

  int button = digitalRead(ROTARY_SW);
  if (button == LOW) { ret = USER_INPUT::BUTTON_PRESSED; }

  if (ret != USER_INPUT::BUTTON_PRESSED) {
    CLK_curr = digitalRead(ROTARY_CLK);

    // pulse occurred if CLK state changed
    // react to only 1 state change to avoid double counting
    if (CLK_curr != CLK_last && CLK_curr == 1) {
      // todo: account for carry over when time rolls over
      if (last_rotary_t > millis) { last_rotary_t = 0; }

      if (digitalRead(ROTARY_DT) == CLK_curr) {
        ret = USER_INPUT::KNOB_DECREASE;
      } else {
        ret = USER_INPUT::KNOB_INCREASE;
      }
      last_rotary_t = millis;
    }

    CLK_last = CLK_curr;
  }

  return ret;
}

void setup_user_input() {
  pinMode(ROTARY_CLK, INPUT);
  pinMode(ROTARY_DT, INPUT);
  pinMode(ROTARY_SW, INPUT_PULLUP);

  CLK_last = digitalRead(ROTARY_CLK);
}

}  // namespace tm

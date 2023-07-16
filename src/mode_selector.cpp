#include "mode_selector.hpp"

namespace tm {
MODE select_mode(USER_INPUT user_input, MODE curr_mode) {
  MODE ret = curr_mode;

  switch (user_input) {
    case USER_INPUT::BUTTON_PRESSED:
      ret = MODE::CLOCK_SET;
      break;
    case USER_INPUT::KNOB_DECREASE:
    case USER_INPUT::KNOB_INCREASE:
      if (curr_mode == MODE::SLEEP) { ret = MODE::TIMER; }
      break;
    default:
      break;
  }

  return ret;
}

MODE select_mode(ACT_STATUS act_status, MODE curr_mode) {
  return act_status == ACT_STATUS::OFF ? MODE::SLEEP : curr_mode;
}
}  // namespace tm

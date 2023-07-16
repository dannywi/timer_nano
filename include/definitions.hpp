#pragma once

// todo: make a real type to prevent implicit conversion
using millis_t = unsigned long;

using int16_t = int;
using uint16_t = unsigned int;

namespace tm {
enum class MODE {
  SLEEP,
  TIMER,
  CLOCK_SET,
};

enum class USER_INPUT {
  KNOB_INCREASE,
  KNOB_DECREASE,
  BUTTON_PRESSED,
  NONE,
};

}  // namespace tm

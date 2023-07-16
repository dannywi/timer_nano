#pragma once

// todo: make a real type to prevent implicit conversion
using millis_t = unsigned long;

using int16_t = int;
using uint16_t = unsigned int;

using uint8_t = unsigned char;

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

enum class ACT_STATUS {
  OFF,
  ON,
};

// todo: split in/out, to make in const
struct Context {
  struct in_ {
    USER_INPUT user_input;
    millis_t millis;
  } in;
  struct out_ {
    const char* lines[3];
    uint8_t num_lines{0};
    bool update_display{false};
    ACT_STATUS act_status{ACT_STATUS::OFF};
  } out;
};

}  // namespace tm

#include "act_timer.hpp"

#include "Arduino.h"
#include "utils.hpp"

namespace tm {
namespace timer {

constexpr uint8_t TIMER_NUM_LINES = 2;
const char* timer_lines[TIMER_NUM_LINES];

millis_t countdown{0};
millis_t increment{60UL * 1000UL};
millis_t last_millis{0UL};

ACT_STATUS status{ACT_STATUS::OFF};

void act(Context& ctx) {
  //millis_t prev_countdown = countdown;
  bool user_input = true;
  if (ctx.in.user_input == USER_INPUT::KNOB_INCREASE) {
    countdown += increment;
    // Serial.print("INC ");
    // Serial.println(countdown);
  } else if (ctx.in.user_input == USER_INPUT::KNOB_DECREASE) {
    countdown = countdown < increment ? 0 : countdown - increment;
    // Serial.print("DEC ");
    // Serial.println(countdown);
  } else {
    user_input = false;
  }

  // static unsigned long last_wake = 0;
  // if (last_wake + 1000 < millis()) {
  //   Serial.print("timer:in mill ");
  //   Serial.print(ctx.in.millis);
  //   Serial.print(" last mill ");
  //   Serial.print(last_millis);
  //   Serial.print(" countdown: ");
  //   Serial.print(countdown);
  //   Serial.println();
  //   last_wake = millis();
  // }

  // todo: account for rollover
  if (!user_input && countdown > ctx.in.millis - last_millis) {

  }

  bool wakeup = ctx.in.millis - last_millis >= 1000UL;
  bool update_timing = user_input || wakeup;

  if (wakeup && !user_input) {
    millis_t interval = ctx.in.millis - last_millis;
    countdown = countdown > interval ? countdown - interval : 0UL;
  }

  if (update_timing) {
    ctx.out.lines[0] = utils::gen_time_str(utils::TIME_PREFIX::NOW, ctx.in.millis);
    ctx.out.lines[1] = utils::gen_time_str(utils::TIME_PREFIX::TGT, countdown);
    ctx.out.num_lines = 2;
    ctx.out.update_display = true;
    last_millis = ctx.in.millis;
  }

  ctx.out.act_status = countdown > 0 ? ACT_STATUS::ON : ACT_STATUS::OFF;
}
}  // namespace timer
}  // namespace tm

#include "act_sleep.hpp"

#include "Arduino.h"
#include "display.hpp"
#include "utils.hpp"

namespace tm {
namespace sleep {
String clock_line;

constexpr uint8_t SLEEP_NUM_LINES = 1;
const char* sleep_lines[SLEEP_NUM_LINES];

millis_t last_millis{0};

void act(Context& ctx) {
  if (last_millis + 1000 <= ctx.in.millis) {
    last_millis = ctx.in.millis;
    ctx.out.lines[0] = utils::gen_time_str(utils::TIME_PREFIX::NOW, ctx.in.millis);
    ctx.out.num_lines = SLEEP_NUM_LINES;
    ctx.out.update_display = true;
  }

  ctx.out.act_status = ACT_STATUS::OFF;
}
}  // namespace sleep
}  // namespace tm

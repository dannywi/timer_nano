#include "act_sleep.hpp"

#include "Arduino.h"
#include "display.hpp"

namespace tm {
namespace sleep {
String clock_line;

constexpr uint8_t SLEEP_NUM_LINES = 1;
const char* sleep_lines[SLEEP_NUM_LINES];

millis_t sleep_last_millis{0};

void act(Context& ctx) {
  if (sleep_last_millis + 1000 <= ctx.in.millis) {
    sleep_last_millis = ctx.in.millis;

    millis_t secs = ctx.in.millis / 1000UL;

    static constexpr millis_t HOUR = 3600UL;
    static constexpr millis_t MIN = 60UL;

    millis_t cycle_half_day = secs % (HOUR * 12UL);
    millis_t hour = cycle_half_day / HOUR;

    millis_t cycle_hour = secs % HOUR;
    millis_t min = cycle_hour / MIN;

    millis_t cycle_min = secs % MIN;
    millis_t sec = cycle_min;

    auto str_pad = [](millis_t v) { return v < 10 ? String("0") + String(v) : String(v); };
    String hour_s = str_pad(hour);
    String min_s = str_pad(min);
    String sec_s = str_pad(sec);
    clock_line = "NOW: " + hour_s + ":" + min_s + ":" + sec_s;
    ctx.out.lines[0] = clock_line.c_str();
    ctx.out.num_lines = SLEEP_NUM_LINES;
    ctx.out.update_display = true;
  }
}
}  // namespace sleep
}  // namespace tm

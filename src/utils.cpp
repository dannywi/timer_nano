#include "utils.hpp"

#include "Arduino.h"

namespace tm {
namespace utils {

struct TimeStrBuffer {
  const char* const prefix;
  String time_str;
};

// due to g++ 7.3.0 limitation, the enums have to be in numerical order
TimeStrBuffer time_str_buffers[TIME_PREFIX::kMAX] = {
    [TIME_PREFIX::NOW] = {"NOW: "},
    [TIME_PREFIX::TGT] = {"TGT: "},
    [TIME_PREFIX::REM] = {"REM: "},
};

const char* gen_time_str(TIME_PREFIX prefix, millis_t millis) {
  millis_t secs = millis / 1000UL;

  static constexpr millis_t HOUR = 3600UL;
  static constexpr millis_t MIN = 60UL;

  millis_t cycle_half_day = secs % (HOUR * 12UL);
  millis_t hour = cycle_half_day / HOUR;

  millis_t cycle_hour = secs % HOUR;
  millis_t min = cycle_hour / MIN;

  millis_t cycle_min = secs % MIN;
  millis_t sec = cycle_min;

  // todo: don't use String here, just a fixed char[] and change the digits
  static auto str_pad = [](millis_t v) { return v < 10 ? String("0") + String(v) : String(v); };
  String hour_s = str_pad(hour);
  String min_s = str_pad(min);
  String sec_s = str_pad(sec);

  TimeStrBuffer& buffer = time_str_buffers[prefix];
  buffer.time_str = buffer.prefix + hour_s + ":" + min_s + ":" + sec_s;
  return buffer.time_str.c_str();
}
}  // namespace utils
}  // namespace tm

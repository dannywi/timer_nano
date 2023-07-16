#pragma once

#include "definitions.hpp"

namespace tm {
namespace utils {

enum TIME_PREFIX {
  NOW,
  TGT,
  REM,
  kMAX,
};

const char* gen_time_str(TIME_PREFIX prefix, millis_t millis);
}  // namespace utils
}  // namespace tm

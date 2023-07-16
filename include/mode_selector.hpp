#pragma once

#include "definitions.hpp"

namespace tm {
MODE select_mode(USER_INPUT user_input, MODE curr_mode);
MODE select_mode(ACT_STATUS act_status, MODE curr_mode);
}  // namespace tm

#pragma once

#include "definitions.hpp"

namespace tm {
void setup_display();
void update_display(const char* lines[], uint8_t num_lines);
}  // namespace tm

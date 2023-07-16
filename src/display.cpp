#include "display.hpp"

#include <U8x8lib.h>

namespace tm {

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8;

const char* greet[] = {
    "G'DAY MATE!",
    "NICE DAY TODAY!",
};
constexpr uint8_t greet_size = sizeof(greet) / sizeof(char*);

uint8_t MAX_LINES = 4;

void setup_display() {
  u8x8.begin();
  // github.com/olikraus/u8g2/wiki/fntlist8x8
  u8x8.setFont(u8x8_font_victoriabold8_u);
  update_display(greet, greet_size);
}

void update_display(const char* lines[], uint8_t num_lines) {
  u8x8.clearDisplay();
  uint8_t limit = min(num_lines, MAX_LINES);
  for (uint8_t i = 0; i < limit; ++i) { u8x8.drawString(0, i, lines[i]); }
}

}  // namespace tm

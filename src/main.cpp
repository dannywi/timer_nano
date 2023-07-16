#include <Arduino.h>
#include <U8x8lib.h>
#include "definitions.hpp"
#include "user_input.hpp"

// *** temporary ***
template <int PIN, unsigned long INTERVAL>
void process_LED() {
  static unsigned long prev_t = 0;
  static auto curr_state = LOW;
  if (millis() < prev_t) {
    prev_t = 0;  // todo: this would make it out of rythm once, fix
  }

  if (millis() - prev_t > INTERVAL) {
    auto set_v = curr_state == LOW ? HIGH : LOW;
    digitalWrite(PIN, set_v);
    curr_state = set_v;
    prev_t = millis();
  }
}

int counter = 0;

String currDir = "";

// **** DISPLAY ****
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8;

namespace tm {
MODE g_curr_mode{MODE::SLEEP};
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT);

  tm::setup_user_input();

  u8x8.begin();
  // github.com/olikraus/u8g2/wiki/fntlist8x8
  u8x8.setFont(u8x8_font_victoriabold8_u);
}

void loop() {
  process_LED<13, 500>();
  process_LED<5, 1000>();

  bool update_display = false;

  tm::USER_INPUT user_input = tm::get_user_input(millis());
  if (user_input != tm::USER_INPUT::NONE) {
    update_display = true;
  }
  if (user_input == tm::USER_INPUT::KNOB_INCREASE) {
    counter += 1;
    currDir = "INCR";
  } else if(user_input == tm::USER_INPUT::KNOB_DECREASE) {
    counter -= 1;
    currDir = "DECR";
  }

  if (update_display) {
    u8x8.clearDisplay();
    String a = "VAL: ";
    a = a + String(counter);
    u8x8.drawString(0,  1, a.c_str());
    u8x8.drawString(0,  2, currDir.c_str());
  }

  // help debounce reading
  delay(5);
}

/* Timer TODO
- move rotary to lib
  - design the API
- connect display
  - design the display
- add thermometer to the display
  - display when timer is off

SPEC:
- Mode:
  - Timer:
    - when rotary is turned, change to Timer mode
      - if timer is started, stop
    - after no action for 2 secs:
      - if value is > 0, start counting down
        - say somehting on screen
        - display count down
      - if value is 0, reset all timer, go to sleep mode
        - say something on screen
    - one tick one min at start
    - but if scrolled fast (position change > 5 in last 300 millisecs), change to 5 mins step
    - if no operation for 1 sec, go back to 1 min step
  - Sleep:
    - show temperature
      - every minute, change display to 24 hour high/low
  - clock set
    - button pressed on sleep mode, go to set hour 0 - 12
    - button pressed again, go to set minute 0 - 60
    - button pressed again, go to set seconds 0 - 60
    - button pressed again, reset clock, go back to sleep mode

  "thread:"
  - timer

TODO:
DONE - create select_mode func
DONE - add var mode in main
DONE - set curr mode as sleep in setup
- put in loop()

DONE - make user_input module - get_user_input
DONE - in loop(), call this and pass result to select_mode

- action: timer
  - on_user_input
  - process -> display, return MODE?

- action: sleep
  - on_user_input: assert false! shouldn't come here
  - process -> display, return MODE?

- action: clock set
  - on_user_input: button -> move cursor, incr/decr: adjust
  - process -> display (blink target), return MODE?

*/

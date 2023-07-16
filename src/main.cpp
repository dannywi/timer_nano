#include <Arduino.h>

#include "act_sleep.hpp"
#include "definitions.hpp"
#include "display.hpp"
#include "mode_selector.hpp"
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

String currCnt = "";
String currDir = "";

namespace tm {
MODE curr_mode{MODE::SLEEP};

Context context;
}  // namespace tm

void setup() {
  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT);
  // Serial.begin(9600);

  tm::setup_user_input();
  tm::setup_display();
}

void loop() {
  process_LED<13, 500>();
  process_LED<5, 1000>();

  bool update_display = false;

  tm::USER_INPUT user_input = tm::get_user_input(millis());
  tm::MODE tgt_mode = tm::select_mode(user_input, tm::curr_mode);
  tm::context.out.update_display = false;

  // static unsigned long last_wake = 0;
  // if (last_wake + 1000 < millis()) {
  //   Serial.print(millis());
  //   Serial.print(" ");
  //   Serial.print("mode ");
  //   Serial.println((int)tgt_mode);
  //   last_wake = millis();
  // }

  if (tgt_mode == tm::MODE::SLEEP) {
    tm::context.in.millis = millis();
    tm::context.in.user_input = user_input;
    tm::sleep::act(tm::context);
  } else {
    if (user_input != tm::USER_INPUT::NONE) { update_display = true; }

    if (user_input == tm::USER_INPUT::KNOB_INCREASE) {
      counter += 1;
      currDir = "INCR";
    } else if (user_input == tm::USER_INPUT::KNOB_DECREASE) {
      counter -= 1;
      currDir = "DECR";
    }

    if (update_display) {
      // static const char* lines[2];
      currCnt = String("VAL: ") + String(counter);
      tm::context.out.lines[0] = currCnt.c_str();
      tm::context.out.lines[1] = currDir.c_str();
      tm::context.out.num_lines = 2;
      tm::context.out.update_display = true;
    }
  }

  if (tm::context.out.update_display) { tm::update_display(tm::context.out.lines, tm::context.out.num_lines); }

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
- Create Timer


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

#include <Arduino.h>

#include "act_sleep.hpp"
#include "act_timer.hpp"
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
  process_LED<13, 1000>();
  process_LED<5, 2000>();

  // Decide mode from user input (or the lack of it)
  tm::USER_INPUT user_input = tm::get_user_input(millis());
  tm::curr_mode = tm::select_mode(user_input, tm::curr_mode);

  tm::context.in.millis = millis();
  tm::context.in.user_input = user_input;
  tm::context.out.update_display = false;

  // static unsigned long last_wake = 0;
  // if (last_wake + 1000 < millis()) {
  //   Serial.print(millis());
  //   Serial.print(" ");
  //   Serial.print("mode ");
  //   Serial.println((int)tm::curr_mode);
  //   last_wake = millis();
  // }

  // Execute the mode's action taker
  if (tm::curr_mode == tm::MODE::SLEEP) {
    tm::sleep::act(tm::context);
  } else if (tm::curr_mode == tm::MODE::TIMER) {
    tm::timer::act(tm::context);
  }

  // Post processings
  if (tm::context.out.update_display) { tm::update_display(tm::context.out.lines, tm::context.out.num_lines); }
  tm::curr_mode = tm::select_mode(tm::context.out.act_status, tm::curr_mode);

  // help debounce reading
  delay(5);
}

/* Timer TODO
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
DONE - add another select_mode for post act
- Sleep
  DONE - add out param: processing (always DONE)
  - add curr 12h clock as input

- Create Timer
  DONE - create a timer counter, and a start time
  DONE - if it's zero because of incr/decr, UNSET
  - when called
    DONE - compare against last called
      DONE - if 1000 ms passed, or countdown updated, update display
      - if countdown zero (without user input), sound the alarm! and then UNSET
  CXL - immediately OFF - if it's UNSET and time from last user input > 5 secs, go back to sleep
  DONE - output:
    - status: PROCESSING or DONE
    - lines to print
  - account for rolling over

- create a Bell
  - bell::setup + bell::fire + bell::process (this gets called on every iteration)
  - Timer can call fire
  - has its own counter when to start / stop the bell

- create a clock setter
  - output:
    - status: PROCESSING or DONE
    - lines to print
    -
  - input: pointer to current 12h clock (can update!)

- add thermometer to sleep mode (should be transparent to main, only changing the output lines)

OTHERS:
- clean mode to act function mapping
- clean input/output context, if possible make input const
- go through all the main vars that gets mutated inside the actions ...
*/

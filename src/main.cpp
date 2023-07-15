#include <Arduino.h>

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

// **** Rotary ****
#define CLK 2
#define DT 3
#define SW 4

int counter = 0;
int CLK_curr;
int CLK_last;
String currDir = "";
unsigned long last_button_t = 0;
unsigned long last_rotary_t = 0;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  Serial.begin(9600);
  CLK_last = digitalRead(CLK);
}

void loop() {
  process_LED<13, 500>();
  process_LED<5, 1000>();

  CLK_curr = digitalRead(CLK);

  // pulse occurred if CLK state changed
  // react to only 1 state change to avoid double counting
  if (CLK_curr != CLK_last && CLK_curr == 1) {
    if (last_rotary_t > millis()) { last_rotary_t = 0; }

    if (millis() - last_rotary_t > 5000) {
      Serial.println("Reset counter");
      counter = 0;
    }

    if (digitalRead(DT) != CLK_curr) {
      --counter;
      currDir = "CounterClockWise";
    } else {
      ++counter;
      currDir = "ClockWise";
    }

    Serial.print("Direction: ");
    Serial.print(currDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);

    last_rotary_t = millis();
  }

  CLK_last = CLK_curr;
  int button = digitalRead(SW);
  if (button == LOW) {
    // button is pressed
    if (millis() - last_button_t > 50) { Serial.println("Button Pressed"); }
    last_button_t = millis();
  }

  // help debounce reading
  delay(1);
}

/* Timer TODO
- move rotary to lib
  - design the API
- connect display
  - design the display
- add thermometer to the display
  - display when timer is off
*/

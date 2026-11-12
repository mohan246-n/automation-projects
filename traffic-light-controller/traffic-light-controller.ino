/*
  Traffic Light Controller
  ------------------------
  A standard road traffic light sequence - green, yellow,
  red, back to green - with fixed timings, plus an optional
  pedestrian crossing button. Pressing the button asks for a
  crossing phase: the lights finish green safely, go red,
  and the pedestrian "walk" LED lights for a set time before
  traffic resumes. All lights are OFF on startup, then the
  sequence begins with RED (safe default).

  Wiring:
    Red LED     : D8  (through a 220 ohm resistor to GND)
    Yellow LED  : D9  (through a 220 ohm resistor to GND)
    Green LED   : D10 (through a 220 ohm resistor to GND)
    Walk LED    : D11 (through a 220 ohm resistor to GND)
    Button      : D2 (other side to GND, uses internal pull-up)
*/

// ---- Pin definitions ----
const int RED_PIN    = 8;
const int YELLOW_PIN = 9;
const int GREEN_PIN  = 10;
const int WALK_PIN   = 11;
const int BUTTON_PIN = 2;   // Pedestrian button (to GND)

// ---- Phase timings (ms) ----
const unsigned long GREEN_MS  = 5000;
const unsigned long YELLOW_MS = 2000;
const unsigned long RED_MS    = 5000;
const unsigned long WALK_MS   = 4000;  // Part of the red phase

bool pedestrianWaiting = false;

void setLights(bool red, bool yellow, bool green, bool walk) {
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(YELLOW_PIN, yellow ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(WALK_PIN, walk ? HIGH : LOW);
}

void waitCheckingButton(unsigned long ms) {
  // Wait in small steps so a button press is never missed.
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      pedestrianWaiting = true;
    }
    delay(20);
  }
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(WALK_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Safe default: all lights OFF, then start from RED.
  setLights(false, false, false, false);

  Serial.begin(9600);
  Serial.println("Traffic Light Controller ready.");
}

void loop() {
  // RED phase (with pedestrian crossing if requested).
  Serial.println("RED - traffic stop");
  if (pedestrianWaiting) {
    pedestrianWaiting = false;
    Serial.println("Pedestrians: WALK");
    setLights(true, false, false, true);
    waitCheckingButton(WALK_MS);
    setLights(true, false, false, false);
    waitCheckingButton(RED_MS - WALK_MS);
  } else {
    setLights(true, false, false, false);
    waitCheckingButton(RED_MS);
  }

  // GREEN phase.
  Serial.println("GREEN - traffic go");
  setLights(false, false, true, false);
  waitCheckingButton(GREEN_MS);

  // YELLOW phase.
  Serial.println("YELLOW - traffic prepare to stop");
  setLights(false, true, false, false);
  waitCheckingButton(YELLOW_MS);
}

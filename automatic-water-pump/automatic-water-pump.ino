/*
  Automatic Water Pump
  --------------------
  Two float switches watch a sump / storage tank: when water
  reaches the HIGH float the pump relay drains it down to the
  LOW float, then stops. Dry-run protection: a maximum run
  time and a mandatory rest time, and the pump never starts
  unless the HIGH float says there is water. Relay is OFF on
  startup (safe default). For fill (not drain) use, see README.

  Wiring:
    Low float   : D2 (to GND, pull-up; float up = LOW reading)
    High float  : D3 (to GND, pull-up)
    Pump relay  : D7 (active-LOW relay module)
    Status LED  : D13 (through a 220 ohm resistor to GND)
*/

// ---- Pin definitions ----
const int LOW_FLOAT_PIN  = 2;   // Low-level float switch
const int HIGH_FLOAT_PIN = 3;   // High-level float switch
const int RELAY_PIN      = 7;   // Pump relay IN1 (active LOW)
const int STATUS_LED     = 13;  // Built-in / status LED

// ---- Dry-run / motor protection (ms) ----
const unsigned long MAX_RUN_MS  = 10UL * 60UL * 1000UL;  // Max 10 min continuous
const unsigned long REST_MS     = 2UL * 60UL * 1000UL;   // Then rest 2 min

bool pumpOn = false;
unsigned long pumpStarted = 0;
unsigned long pumpStopped = 0;
bool resting = false;

bool floatUp(int pin) {
  // Float switch closes to GND when lifted by water: LOW = up.
  return digitalRead(pin) == LOW;
}

void setPump(bool on) {
  pumpOn = on;
  digitalWrite(RELAY_PIN, on ? LOW : HIGH);  // LOW = relay ON
  digitalWrite(STATUS_LED, on ? HIGH : LOW);
  if (on) {
    pumpStarted = millis();
    Serial.println("Pump ON");
  } else {
    pumpStopped = millis();
    Serial.println("Pump OFF");
  }
}

void setup() {
  pinMode(LOW_FLOAT_PIN, INPUT_PULLUP);
  pinMode(HIGH_FLOAT_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Safe defaults: relay OFF, pump OFF on startup.
  digitalWrite(RELAY_PIN, HIGH);  // HIGH = relay OFF (active LOW)
  digitalWrite(STATUS_LED, LOW);
  pumpStopped = millis();

  Serial.begin(9600);
  Serial.println("Automatic Water Pump ready.");
}

void loop() {
  bool highWater = floatUp(HIGH_FLOAT_PIN);
  bool lowWater  = floatUp(LOW_FLOAT_PIN);

  // Rest period after a long run protects the motor and
  // guarantees the pump cannot run dry continuously.
  if (resting && millis() - pumpStopped > REST_MS) {
    resting = false;
    Serial.println("Rest over - pump available again.");
  }

  if (pumpOn) {
    if (!lowWater) {
      setPump(false);  // Drained down to the low float: stop
      resting = true;
    } else if (millis() - pumpStarted > MAX_RUN_MS) {
      setPump(false);  // Safety cut-off: something is wrong
      resting = true;
      Serial.println("WARNING: Max run time reached - check tank / floats.");
    }
  } else if (!resting && highWater) {
    setPump(true);  // Water is high: start draining
  }

  Serial.print("High float: ");
  Serial.print(highWater ? "UP" : "down");
  Serial.print("  Low float: ");
  Serial.print(lowWater ? "UP" : "down");
  Serial.print("  Pump: ");
  Serial.println(pumpOn ? "ON" : "OFF");

  delay(500);  // Check twice a second
}

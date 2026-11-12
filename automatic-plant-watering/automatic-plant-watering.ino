/*
  Automatic Plant Watering
  ------------------------
  A capacitive / resistive soil moisture sensor watches the
  soil. When it gets dry the pump relay runs a small water
  pump for a short burst, then waits and checks again, so the
  plant is watered little and often and never flooded. A
  maximum run time stops the pump if the sensor fails. Relay
  is OFF on startup (safe default).

  Wiring:
    Moisture A0 : A0 (VCC 5V, GND GND)
    Pump relay  : D7 (active-LOW relay module)
    Status LED  : D13 (through a 220 ohm resistor to GND)
*/

// ---- Pin definitions ----
const int MOISTURE_PIN = A0;  // Soil moisture analog output
const int RELAY_PIN    = 7;   // Pump relay IN1 (active LOW)
const int STATUS_LED   = 13;  // Built-in / status LED

// ---- Thresholds (0-1023). Calibrate for YOUR sensor/soil. ----
// Typical resistive sensor: HIGH reading = DRY soil.
// Water when reading rises above DRY_THRESHOLD,
// soil is wet enough below WET_THRESHOLD.
const int DRY_THRESHOLD = 600;
const int WET_THRESHOLD = 450;

const unsigned long WATER_RUN_MS   = 5000;    // Pump burst: 5 s
const unsigned long REST_MS        = 60000;   // Wait 1 min between checks after watering
const unsigned long CHECK_MS       = 2000;    // Normal check interval

bool soilDry = false;

void setPump(bool on) {
  digitalWrite(RELAY_PIN, on ? LOW : HIGH);  // LOW = relay ON
  digitalWrite(STATUS_LED, on ? HIGH : LOW);
  Serial.println(on ? "Pump ON - watering" : "Pump OFF");
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Safe defaults: relay OFF, pump OFF on startup.
  digitalWrite(RELAY_PIN, HIGH);  // HIGH = relay OFF (active LOW)
  digitalWrite(STATUS_LED, LOW);

  Serial.begin(9600);
  Serial.println("Automatic Plant Watering ready.");
  Serial.println("Let the sensor settle in the soil before trusting readings.");
}

void loop() {
  int moisture = analogRead(MOISTURE_PIN);
  Serial.print("Soil moisture: ");
  Serial.println(moisture);

  if (!soilDry && moisture > DRY_THRESHOLD) {
    soilDry = true;
  } else if (soilDry && moisture < WET_THRESHOLD) {
    soilDry = false;
    Serial.println("Soil wet enough - no watering needed.");
  }

  if (soilDry) {
    // Water in a short burst, then let it soak in before
    // re-checking, so the sensor sees the real new level.
    setPump(true);
    delay(WATER_RUN_MS);  // Hard limit: pump never runs longer per burst
    setPump(false);
    delay(REST_MS);
  } else {
    delay(CHECK_MS);
  }
}

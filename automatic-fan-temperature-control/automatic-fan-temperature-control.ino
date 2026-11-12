/*
  Automatic Fan Temperature Control
  ---------------------------------
  An LM35 temperature sensor (analog, no library needed)
  watches the room temperature. When it gets hot the fan
  relay turns the fan ON; when it cools down the fan turns
  OFF. A hysteresis gap stops the fan switching rapidly at
  the threshold. Relay is OFF on startup (safe default).

  Wiring:
    LM35 OUT    : A0 (Vs 5V, GND GND, flat side facing you:
                      left = Vs, middle = OUT, right = GND)
    Fan relay   : D7 (active-LOW relay module)
    Status LED  : D13 (through a 220 ohm resistor to GND)
*/

// ---- Pin definitions ----
const int LM35_PIN   = A0;  // LM35 analog output (10 mV / deg C)
const int RELAY_PIN  = 7;   // Fan relay IN1 (active LOW)
const int STATUS_LED = 13;  // Built-in / status LED

// ---- Temperature thresholds (degrees C) ----
// Fan ON above FAN_ON_TEMP, OFF below FAN_OFF_TEMP.
const float FAN_ON_TEMP  = 30.0;
const float FAN_OFF_TEMP = 28.0;

bool fanOn = false;

float readTemperatureC() {
  // Average a few readings for a steady value.
  long total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(LM35_PIN);
    delay(5);
  }
  float average = total / 10.0;
  float millivolts = average * 5000.0 / 1023.0;
  return millivolts / 10.0;  // LM35: 10 mV per degree C
}

void setFan(bool on) {
  fanOn = on;
  digitalWrite(RELAY_PIN, on ? LOW : HIGH);  // LOW = relay ON
  digitalWrite(STATUS_LED, on ? HIGH : LOW);
  Serial.println(on ? "Fan ON" : "Fan OFF");
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Safe defaults: relay OFF, fan OFF on startup.
  digitalWrite(RELAY_PIN, HIGH);  // HIGH = relay OFF (active LOW)
  digitalWrite(STATUS_LED, LOW);

  Serial.begin(9600);
  Serial.println("Automatic Fan Temperature Control ready.");
}

void loop() {
  float tempC = readTemperatureC();
  Serial.print("Temperature: ");
  Serial.print(tempC, 1);
  Serial.println(" C");

  if (!fanOn && tempC > FAN_ON_TEMP) {
    setFan(true);
  } else if (fanOn && tempC < FAN_OFF_TEMP) {
    setFan(false);
  }

  delay(1000);  // Check once a second
}

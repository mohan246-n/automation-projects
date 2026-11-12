/*
  Automatic Gate Opener
  ---------------------
  An HC-SR04 ultrasonic sensor watches for a vehicle or person
  approaching the gate. When something gets close, a servo
  swings a small model gate open (use a relay / motor driver
  for a real gate - see README). The gate stays open while the
  way is occupied, then closes after a clear delay. An
  obstacle while closing re-opens the gate (safety stop). The
  servo starts in the CLOSED position (safe default).

  Wiring:
    HC-SR04 TRIG: D9, ECHO: D10, VCC 5V, GND GND
    Servo signal: D6 (servo power from a separate 5V supply
                      for anything larger than a micro servo)
    Status LED  : D13 (through a 220 ohm resistor to GND)
*/

#include <Servo.h>

// ---- Pin definitions ----
const int SERVO_PIN  = 6;   // Servo / gate motor control
const int TRIG_PIN   = 9;   // HC-SR04 trigger
const int ECHO_PIN   = 10;  // HC-SR04 echo
const int STATUS_LED = 13;  // Built-in / status LED

// ---- Gate settings ----
const int OPEN_ANGLE   = 90;   // Servo angle: gate open
const int CLOSED_ANGLE = 0;    // Servo angle: gate closed
const int DETECT_DISTANCE_CM = 50;   // Approach distance: open the gate
const unsigned long CLOSE_DELAY_MS = 5000;  // Close 5 s after way is clear

Servo gateServo;
bool gateOpen = false;
unsigned long wayClearSince = 0;

int readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return -1;  // No echo: treat as clear, never as blocked
  return (int)(duration * 0.0343 / 2.0);
}

void setGate(bool open) {
  gateOpen = open;
  gateServo.write(open ? OPEN_ANGLE : CLOSED_ANGLE);
  digitalWrite(STATUS_LED, open ? HIGH : LOW);
  Serial.println(open ? "Gate OPENING" : "Gate CLOSING");
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Safe default: gate CLOSED on startup.
  gateServo.attach(SERVO_PIN);
  gateServo.write(CLOSED_ANGLE);
  digitalWrite(STATUS_LED, LOW);
  wayClearSince = millis();

  Serial.begin(9600);
  Serial.println("Automatic Gate Opener ready. Gate is closed.");
}

void loop() {
  int distance = readDistanceCm();
  bool occupied = (distance > 0 && distance < DETECT_DISTANCE_CM);

  Serial.print("Distance: ");
  if (distance < 0) Serial.println("clear (no echo)");
  else { Serial.print(distance); Serial.println(" cm"); }

  if (occupied) {
    wayClearSince = millis();
    if (!gateOpen) setGate(true);  // Also re-opens if closing on an obstacle
  } else if (gateOpen && millis() - wayClearSince > CLOSE_DELAY_MS) {
    setGate(false);
  }

  delay(300);
}

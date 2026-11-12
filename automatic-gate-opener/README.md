# Automatic Gate Opener

A gate that opens when a vehicle or person approaches and
closes by itself once the way is clear, built around an
ultrasonic sensor and a servo for a model / small gate.

## How it works

The HC-SR04 measures the distance in front of the gate. When
something comes closer than the detection distance, the
Arduino drives the servo from the closed angle to the open
angle and lights the status LED. The gate stays open while
anything is inside detection range — so it can never close on
a vehicle or person in the way — and closes 5 seconds after
the way becomes clear. Because the "occupied" check runs
continuously, an obstacle appearing while the gate would be
closing immediately keeps / re-opens it: that is the safety
stop. A missing echo is treated as "clear", never as blocked,
so a sensor glitch does not hold the gate open forever — but
it also means a dead sensor cannot protect the way, so test
the sensor regularly.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Ultrasonic sensor | HC-SR04 | 1 |
| Servo motor | SG90 (model gate) or high-torque servo | 1 |
| Gate model / linkage | cardboard / small hinged gate to suit the servo | 1 |
| Resistor | 220 Ω (status LED) | 1 |
| LED | 5 mm, status (gate open) | 1 |
| Separate 5 V supply | for larger servos (recommended) | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
HC-SR04 VCC ── Arduino 5V
HC-SR04 GND ── Arduino GND
HC-SR04 TRIG ─ Arduino D9
HC-SR04 ECHO ─ Arduino D10

Servo signal ─ Arduino D6
Servo power ── 5V (separate supply for large servos,
               GND common with Arduino GND)

Arduino D13 ── 220Ω ── Status LED ── GND
```

The gate starts in the CLOSED position. The code uses the
standard Arduino `Servo` library, which is included with the
Arduino IDE — no extra install is needed.

## Wiring steps

1. Mount the HC-SR04 at gate height, pointing at the approach
   path, clear of the gate's own swing.
2. Connect the sensor: VCC → 5 V, GND → GND, TRIG → D9,
   ECHO → D10.
3. Attach the servo to the gate hinge / linkage, connect its
   signal wire to D6, and power it (a micro servo can share
   the Arduino 5 V; anything larger needs its own 5 V supply
   with a common ground).
4. Connect the status LED: D13 → 220 Ω → LED anode, cathode → GND.
5. Upload `automatic-gate-opener.ino`, open the Serial Monitor
   (9600 baud), and walk up to the sensor to test.
6. Adjust the servo horn position so 0° is fully closed and
   90° is fully open; change `CLOSED_ANGLE` / `OPEN_ANGLE` if
   your linkage needs different angles.

## Calibration

Set `DETECT_DISTANCE_CM` (default 50) to the approach distance
you want — measure it with the Serial distance readings.
Lengthen `CLOSE_DELAY_MS` if the gate closes too soon behind
slow vehicles. For a **real** gate, replace the servo with a
proper gate motor and driver / relay board rated for it, and
keep all of this logic — detection, hold-open, safety re-open —
exactly the same.

## Safety notes

- This is a model / learning build. A real vehicle gate is
  heavy machinery: it needs a rated motor, mechanical stops,
  and a proper safety edge / photocell system fitted by a
  qualified installer. Do not scale this servo build up
  directly to a full-size gate.
- The gate closes on a timer after the way looks clear —
  never rely on it where children or animals cross unobserved.
- The servo is driven to CLOSED on startup, so a reset does
  not leave the gate standing open.
- Use a separate, fused supply for larger motors / servos so
  a stall cannot brown-out and reset the Arduino.

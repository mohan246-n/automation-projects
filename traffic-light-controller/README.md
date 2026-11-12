# Traffic Light Controller

A working model traffic light with the standard red → green →
yellow sequence, plus a pedestrian crossing button with a
"walk" light — the classic first automation project, done
with clean, easy-to-change timings.

## How it works

The Arduino runs the lights as a simple repeating sequence of
phases: red, green, then yellow, each for a fixed time set by
named constants at the top of the code. While any phase is
timing, the code checks the pedestrian button every 20 ms, so
a press is never missed. If a pedestrian is waiting when the
red phase starts, the walk LED joins the red light for the
first part of that phase, giving people a set time to cross
before traffic moves again — just like a real junction, the
lights never jump straight from green to a crossing phase.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| LED | 5 mm red | 1 |
| LED | 5 mm yellow | 1 |
| LED | 5 mm green | 1 |
| LED | 5 mm white / green (pedestrian "walk") | 1 |
| Resistors | 220 Ω (one per LED) | 4 |
| Push button | momentary (pedestrian button) | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
Arduino D8 ── 220Ω ── Red LED ── GND
Arduino D9 ── 220Ω ── Yellow LED ── GND
Arduino D10 ─ 220Ω ── Green LED ── GND
Arduino D11 ─ 220Ω ── Walk LED ── GND

Button ── Arduino D2 and GND (no resistor needed, uses
          the internal pull-up; pressed = LOW)
```

All lights are OFF on startup, and the sequence begins with RED.

## Wiring steps

1. Connect each LED in a row on the breadboard: Arduino pin →
   220 Ω resistor → LED anode (long leg), LED cathode → GND.
   Use D8 red, D9 yellow, D10 green, D11 walk.
2. Connect the pedestrian button between D2 and GND.
3. Upload `traffic-light-controller.ino` and open the Serial
   Monitor (9600 baud) to follow the phases.
4. Watch a full cycle: red, green, yellow, red again.
5. Press the button during green — nothing changes
   immediately (that is correct and safe); the walk light
   comes on with the next red phase.

## Calibration

There is nothing to calibrate — just change the phase timing
constants to taste: `GREEN_MS`, `YELLOW_MS`, `RED_MS`, and
`WALK_MS` (defaults 5000 / 2000 / 5000 / 4000 ms). Real
junctions keep yellow short (about 3 s at scale) and never
skip it — keep a yellow phase in your model too. For a bigger
display, drive each output through a transistor or relay and
use the same code.

## Safety notes

- This is a low-voltage LED model only. Never connect it to
  real traffic signals or any mains wiring.
- All lights OFF on startup, then red first — a reset never
  shows a sudden green, mirroring real fail-safe practice.
- Use one resistor per LED; LEDs wired directly to a pin
  without a resistor can damage both the LED and the Arduino.
- A pedestrian request is served only at the next red phase
  by design — explain that to anyone "crossing" your model,
  so they wait for the walk light.

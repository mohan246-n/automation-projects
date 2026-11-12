# Automatic Water Pump

Two float switches and a pump relay keep a sump or storage
tank drained automatically: the pump starts when the water
gets high, stops when it gets low, and protects itself from
running dry.

## How it works

A float switch floats on the water and closes its contact
when the water lifts it. One float sits at the low level and
one at the high level. When the water rises enough to lift
the HIGH float, the Arduino turns the pump relay on. The pump
runs until the water falls enough for the LOW float to drop —
using two floats, not one, means the pump runs in proper
cycles instead of switching rapidly at a single level.
Dry-run protection is built in two ways: the pump can only
start when the HIGH float says there is real water to move,
and a maximum run time (10 minutes) followed by a mandatory
rest stops the pump if a float sticks or the outlet is
blocked. This code is set up to *drain* a sump / tank; to
*fill* a tank instead, mount the same floats and simply treat
"high = stop, low = start" — swap the start/stop conditions
in the loop, the protection logic stays the same.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Float switches | side / vertical mount, normally-open contact | 2 |
| Relay module | 5 V, 1-channel, opto-isolated (rated for the pump) | 1 |
| Water pump | sump / transfer pump to suit the tank | 1 |
| Resistor | 220 Ω (status LED) | 1 |
| LED | 5 mm, status (pump running) | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
Low float switch ── Arduino D2 and GND (no resistor needed,
                    uses the internal pull-up;
                    float UP = contact closed = LOW)
High float switch ─ Arduino D3 and GND (same wiring)

Arduino D7 ── Relay IN1
Relay COM ─── Pump ─── Pump power supply
Relay NO ──── Power return
             (pump runs only when relay is energised)

Arduino D13 ── 220Ω ── Status LED ── GND
Arduino 5V/GND ── Relay VCC / GND
```

Relay logic: the code uses an **active-LOW** relay module (LOW =
relay ON). The relay is OFF on startup.

## Wiring steps

1. Mount the float switches in the tank: LOW float just above
   the pump's minimum safe water level, HIGH float at the
   level where you want pumping to start. Make sure each
   float can swing / slide freely without touching the walls.
2. Connect the low float between D2 and GND, and the high
   float between D3 and GND.
3. Connect the relay module: VCC → 5 V, GND → GND, IN1 → D7.
4. Connect the pump through the relay's COM and NO terminals.
   **If the pump is mains-powered, get a qualified electrician
   to wire the AC side. For testing, use a small 12 V DC pump.**
5. Connect the status LED: D13 → 220 Ω → LED anode, cathode → GND.
6. Upload `automatic-water-pump.ino`, open the Serial Monitor
   (9600 baud), and test by lifting each float by hand: the
   pump should start on the high float and stop when you
   release the low float.

## Calibration

Float *position* is the calibration here — move the high float
up for a higher start level, and the low float down for a
deeper drain, but never below the pump maker's minimum level.
Set `MAX_RUN_MS` (default 10 minutes) a little longer than a
normal drain cycle for your tank; if the safety cut-off trips
in normal use, check for a blocked outlet before extending it.
`REST_MS` (default 2 minutes) gives the motor a cooling break
after a long run.

## Safety notes

- The relay is OFF on startup, and the pump can never start
  unless the high float is up, so a reset or a dry tank
  cannot leave the pump running dry.
- Water and mains electricity are a dangerous mix. Keep the
  Arduino and relay dry and enclosed, and have a qualified
  electrician wire any mains pump.
- Use a relay (and fuse) rated for your pump's current —
  pump motors draw a high starting current.
- Fit a manual isolator switch, and check the floats monthly
  — a float jammed by debris defeats every protection here.

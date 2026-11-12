# Automatic Fan Temperature Control

A fan that switches itself on when the room gets hot and off
when it cools down — no reaching for the switch, and no fan
left running all night by mistake.

## How it works

An LM35 sensor outputs 10 mV per degree Celsius, so the
Arduino reads it as a plain analog voltage — no library
needed. The reading is averaged over 10 samples for a steady
value. Above the on-threshold the fan relay switches the fan
on; below the off-threshold it switches off again. The 2 °C
gap between the thresholds (hysteresis) stops the fan clicking
on and off rapidly when the temperature hovers at the setting.

A DHT11 can be used instead of the LM35 if you also want
humidity — that needs the DHT sensor library, and the LM35
wiring/code here would change, so the LM35 version is the
default because it compiles with no extra libraries.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Temperature sensor | LM35 (analog) | 1 |
| Relay module | 5 V, 1-channel, opto-isolated | 1 |
| Fan | 12 V DC fan (safe option) or 230 V AC fan via relay | 1 |
| Resistor | 220 Ω (status LED) | 1 |
| LED | 5 mm, status (fan running) | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
LM35 (flat side facing you)
  Vs (left)   ── Arduino 5V
  OUT (middle) ─ Arduino A0
  GND (right) ── Arduino GND

Arduino D7 ── Relay IN1
Relay COM ─── Fan ─── Power (12 V DC, or AC live via relay)
Relay NO ──── Power return / AC neutral side

Arduino D13 ── 220Ω ── Status LED ── GND
Arduino 5V/GND ── Relay VCC / GND
```

Relay logic: the code uses an **active-LOW** relay module (LOW =
relay ON). The relay is OFF on startup.

## Wiring steps

1. Connect the LM35 carefully — it is easy to reverse. With
   the flat side facing you: left pin to 5 V, middle pin to
   A0, right pin to GND. **A reversed LM35 gets hot quickly;
   if it does, disconnect power and check.**
2. Place the sensor away from direct sunlight, the fan's own
   airflow, and warm equipment, so it reads room temperature.
3. Connect the relay module: VCC → 5 V, GND → GND, IN1 → D7.
4. Connect the fan to the relay's COM and NO terminals.
   **For a 230 V AC fan, get a qualified electrician to wire
   the AC side. For testing, use a 12 V DC fan.**
5. Connect the status LED: D13 → 220 Ω → LED anode, cathode → GND.
6. Upload `automatic-fan-temperature-control.ino` and open
   the Serial Monitor (9600 baud) to see the temperature.

## Calibration

Check the Serial temperature against a room thermometer. The
LM35 is factory-calibrated, so readings should be within
about 1 °C. Set `FAN_ON_TEMP` (default 30.0) to the temperature
where you want cooling and `FAN_OFF_TEMP` (default 28.0) a
degree or two below it. Keep the gap at least 1 °C so the fan
does not cycle rapidly.

## Safety notes

- Relays switch the fan, not the Arduino. Never let mains
  wiring touch the Arduino or the breadboard.
- Relay is OFF on startup, so the fan stays off if the
  Arduino resets.
- If using a 230 V AC fan, keep the relay module enclosed
  and unplug it before touching any AC wiring.
- Do not use this as the only cooling control for animals,
  medicine storage, or anything else where overheating while
  unattended would cause harm.

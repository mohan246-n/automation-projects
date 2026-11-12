# Automatic Plant Watering

A plant that waters itself: a soil moisture sensor checks the
soil, and when it gets dry a small pump runs for a few seconds
to water it, then waits while the water soaks in.

## How it works

The moisture sensor's analog output changes with how wet the
soil is. The Arduino reads it continuously and compares it
with dry and wet thresholds. Above the dry threshold it runs
the pump relay for one short burst (5 seconds by default),
then rests for a minute so the water can soak in before it
checks again. Two thresholds, not one, give hysteresis — it
does not keep topping up soil that is already wet enough, and
the fixed burst length means even a failed sensor cannot leave
the pump running and flood the plant.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Soil moisture sensor | capacitive preferred (does not corrode) | 1 |
| Relay module | 5 V, 1-channel, opto-isolated | 1 |
| Water pump | small 5 V / 12 V submersible or diaphragm pump | 1 |
| Silicone tube + water container | fits the pump | 1 |
| Resistor | 220 Ω (status LED) | 1 |
| LED | 5 mm, status (pump running) | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
Moisture VCC ── Arduino 5V
Moisture GND ── Arduino GND
Moisture A0 ─── Arduino A0 (analog soil level)

Arduino D7 ── Relay IN1
Relay COM ─── Pump ─── Pump power supply (5 V / 12 V DC)
Relay NO ──── Power return
             (pump runs only when relay is energised)

Arduino D13 ── 220Ω ── Status LED ── GND
Arduino 5V/GND ── Relay VCC / GND
```

Relay logic: the code uses an **active-LOW** relay module (LOW =
relay ON). The relay is OFF on startup.

## Wiring steps

1. Push the moisture sensor into the plant's soil, up to the
   mark on the probe, away from the stem.
2. Connect the sensor: VCC → 5 V, GND → GND, A0 → A0.
3. Put the pump in the water container, with its tube running
   to the plant pot. **Test it by hand first** so you know how
   fast it delivers water.
4. Connect the relay module: VCC → 5 V, GND → GND, IN1 → D7,
   and the pump through the relay's COM and NO terminals.
5. Connect the status LED: D13 → 220 Ω → LED anode, cathode → GND.
6. Upload `automatic-plant-watering.ino` and open the Serial
   Monitor (9600 baud) to see the moisture readings.

## Calibration

Moisture sensors vary a lot, so calibrate for yours. Note the
Serial reading with the sensor in dry soil and in freshly
watered soil. Set `DRY_THRESHOLD` (default 600) between them,
closer to the dry reading, and `WET_THRESHOLD` (default 450)
below it. Note that most resistive sensors read high when dry
— if yours reads the opposite way, swap the two threshold
meanings. Adjust `WATER_RUN_MS` for pot size: small pots need
only 2–3 seconds per burst.

## Safety notes

- The pump only ever runs in short, fixed bursts, and the
  relay is OFF on startup, so a reset cannot flood the plant.
- Keep the Arduino and relay board away from water splashes;
  only the pump and sensor go near the soil and container.
- Use a low-voltage DC pump. Never run mains wiring to a
  plant pot or water container.
- Check the water container regularly — the system cannot
  water from an empty container, and a pump run dry wears out.

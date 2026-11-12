# Automation Projects

A collection of 5 practical, beginner-friendly Arduino-based automation
projects, built for learning, home use, and portfolio / college work.
Each project lives in its own folder with its own README, working
Arduino code (`.ino`), and a text circuit diagram.

## Projects

| # | Project | What it does |
|---|---------|--------------|
| 1 | [automatic-plant-watering](automatic-plant-watering/) | Soil moisture sensor runs a small pump in short bursts when the soil gets dry, and never floods the plant. |
| 2 | [automatic-fan-temperature-control](automatic-fan-temperature-control/) | LM35 temperature sensor switches a fan on when the room gets hot and off when it cools down. |
| 3 | [automatic-gate-opener](automatic-gate-opener/) | Ultrasonic sensor detects an approaching vehicle or person and opens the gate, closing it once the way is clear. |
| 4 | [traffic-light-controller](traffic-light-controller/) | Red / yellow / green light sequence with a pedestrian crossing button and walk light. |
| 5 | [automatic-water-pump](automatic-water-pump/) | Two float switches run a pump between high and low water levels, with dry-run and max-run protection. |

## How to use

1. Pick a project folder and read its `README.md` first — it lists the
   exact components, wiring, and safety notes for that project.
2. Open the `.ino` file in the Arduino IDE (or Arduino CLI / VS Code
   with the Arduino extension).
3. Select your board (Arduino Uno for all 5 projects in this repo) and
   the correct port, then upload.
4. Wire the circuit exactly as described in the project README.
   **Do not power the circuit until you have double-checked the wiring.**
5. Open the Serial Monitor at 9600 baud (where the project uses it)
   to see readings and status messages.

## Common components and tools

- Arduino Uno (all 5 projects)
- Breadboard, jumper wires, USB cable
- Resistors: 10 kΩ, 220 Ω (see each project for exact values)
- 5 V relay module(s), LEDs, push buttons / switches
- Soil moisture sensor, LM35 temperature sensor, HC-SR04 ultrasonic
  sensor, float switches, servo motor (SG90)
- Small DC water pumps (projects 1 and 5)
- Multimeter, wire stripper, screwdriver, soldering iron (optional)

## ⚠️ Safety warnings — read before building anything

- **Mains voltage (230 V AC in India / 110–240 V AC elsewhere) can kill.**
  Projects in this repo switch mains loads only through an enclosed
  relay module, and the low-voltage Arduino side must be kept fully
  separate from the mains side. If you are not confident working with
  mains wiring, get a qualified electrician to wire the AC side, or
  build the project with a low-voltage DC load (e.g. a 12 V fan or
  pump) instead.
- Never touch the relay module's screw terminals or mains wires while
  the circuit is powered. Always unplug / switch off at the wall
  before changing wiring.
- Check every connection with a multimeter before first power-up.
- Keep water away from electronics: for the watering and pump
  projects, keep the Arduino and relay dry and outside the tank / pot.
- A pump / fan relay and fuse must be rated for the motor's current —
  motors draw a high starting current.
- The gate opener is a model / learning build. A real vehicle gate
  needs a rated motor and proper safety systems fitted by a qualified
  installer — do not scale the servo build up directly.

## Repo layout

```
automation-projects/
├── README.md
├── LICENSE
├── .gitignore
├── automatic-plant-watering/
├── automatic-fan-temperature-control/
├── automatic-gate-opener/
├── traffic-light-controller/
└── automatic-water-pump/
```

## Pushing to GitHub

```bash
git init
git add .
git commit -m "Add 5 Arduino automation projects"
git branch -M main
git remote add origin https://github.com/<your-username>/automation-projects.git
git push -u origin main
```

## License

MIT — see [LICENSE](LICENSE).

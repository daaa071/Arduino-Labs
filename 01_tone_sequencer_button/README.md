# 🎵 Tone Sequencer with Button & LEDs

This Arduino project plays a sequence of musical notes (C4–B4) with synchronized LED lights when a button is pressed. It also demonstrates passive buzzer control and LED sequencing.

---

## 🔧 Components Used

| Component              | Quantity |
|------------------------|----------|
| Arduino Uno / Leonardo | 1        |
| Breadboard             | 1        |
| Red LEDs               | 7        |
| 220Ω Resistors         | 7        |
| 10kΩ Resistors         | 1        |
| Passive Buzzer         | 1        |
| Push Button            | 1        |
| Jumper Wires           | ~20      |

---

## 📷 Circuit Diagram

![Circuit Diagram](01_tone_sequencer_button/01_tone_sequencer_button_circuit.png)

---

## 🧠 How It Works

- When the button is pressed:
  - A tone is played on the buzzer from C4 to B4.
  - A corresponding LED lights up for each tone.
- Each note lasts **1 second**, followed by a **brief pause**.
- Button press is stabilized with **debounce logic**.

---

## 💡 Highlights

- ✅ Use of `tone()` and `noTone()`
- ✅ LED sequencing with arrays
- ✅ Debouncing button inputs
- ✅ Clean code with `constexpr` and loops

---

## 🧪 Planned Improvements

- Add more tones or chords.
- Create a long-press to play in reverse.
- Add a second mode with a toggle switch.

---

> Made with ❤️ using [Fritzing](https://fritzing.org/) and Arduino IDE.

#Note for README

The content of this README was written entirely by me (Zoraiz). Grok AI was used solely to refine the language, eliminate grammatical and spelling errors, and enhance readability.

# EMG & Joystick Controlled Wheelchair

Low-cost powered wheelchair retrofit with two control modes:

- Joystick (manual) mode
- Hands-free EMG / brainwave (neural) mode using a head-mounted sensor

## Features

- Toggle between modes with one push button
- Joystick: forward / backward / left / right with variable speed
- Neural mode: tense facial/head muscles to select direction (auto-cycling commands)
- Stops automatically on poor sensor signal
- Direction LEDs show current/active command
- Bluetooth wireless link between headband and wheelchair

## Hardware Needed

- Arduino Uno
- 2× DC geared motors + IBT-2 (or similar high-current) motor driver
- Analog joystick module (with button)
- Brainwave/EMG sensor module
- 2× HC-05 Bluetooth modules
- 12V battery + 5V buck converter
- 9V battery (for headband)
- 4× direction LEDs + 1 mode LED

## How to Upload & Run the Code

1. Connect your **Arduino Uno** to your computer via USB
2. Open **Arduino IDE** (download from arduino.cc if needed)
3. File → Open → select `Wheel_Chair.ino`
4. In **Tools → Board** → select "Arduino Uno"
5. In **Tools → Port** → choose the port your Arduino is connected to
6. Click the **Upload** button (→ arrow icon)
7. Wait for "Done uploading" message

After upload:
- Power the wheelchair electronics (12V battery)
- Power the headband (9V battery)
- Press the mode button to switch between joystick and neural mode

**Note:** The Bluetooth modules must be pre-paired before first use (usually HC-05 default password: 1234 or 0000).

## Important

- Always test in a safe, open space
- Neural mode is sensitive to sensor placement & skin contact
- The chair stops immediately if signal quality is poor (safety feature)

Made for accessibility.  
— Zoraiz

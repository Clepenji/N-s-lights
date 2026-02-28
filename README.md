# N's Smart Fairy Lights 🎁

A birthday gift project that transforms ordinary fairy lights into smart, automatic mood lighting for a teenager's room!

## What It Does

This Arduino-powered controller makes fairy lights smarter by:
- **Automatically turning on** when the room gets dark 🌙
- **Automatically turning off** when it gets bright again 🌅
- **Manual control** with a simple button press
- **Smart timeout features** to prevent lights from staying on all night

Perfect for creating the perfect room ambiance without having to remember to turn the lights on and off!

## Hardware Requirements

- Arduino board (Uno, Nano, or similar)
- Light sensor (connected to pin A2)
- Push button (connected to pin 4)
- Relay or transistor to control fairy lights (connected to pin 10)
- Fairy lights (obviously! ✨)

## How to Use

1. Upload `smart_fairy-lights_refactored.ino` to your Arduino
2. Wire up the components according to the pin configuration
3. Hang your fairy lights and enjoy automatic lighting!

**Button Controls:**
- **Single press:** Toggle lights on/off manually
- The lights will automatically turn on when it gets dark
- Auto-on lights will turn off after 30 minutes (or when it gets bright again)

## Features

- **Ambient light sensing** with debouncing to prevent flickering
- **Multiple operating modes** (manual on, auto on, disabled)
- **Smart timeouts** (20-minute disable period, 30-minute auto-off)
- **State machine design** for reliable operation

---

*Made with 💙 for a friend's birthday*
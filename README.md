# Custom-Beatmania-IIDX-Controller

This repository contains the firmware and build notes for a DIY **Beatmania IIDX-style arcade controller** built around an **Arduino Leonardo** acting as a **USB HID keyboard**.

## Features
- **Arcade button inputs (IIDX-style layout)** mapped to keyboard keys (plug-and-play, no drivers).
- **Per-button LED feedback** driven by Arduino GPIO pins.
- **Stable input handling** suitable for fast rhythm gameplay:
  - Debounced press/release logic to avoid accidental double triggers
  - Encoder sensitivity tuned via constants (threshold / release timing)
- **Easy customization**: key mappings and pin assignments are defined in arrays/constants for quick rebinding.

## Hardware Overview
- **Arduino Leonardo** (native USB HID keyboard support)
- **Samducksa arcade microswitch buttons**
- **Rotary encoder (600 P/R)** for turntable
- Button **LEDs** for visual feedback
- Common ground wiring for switches + LEDs (all grounds tied to Arduino GND)

## Wiring Notes
- Buttons are configured with `INPUT_PULLUP`, meaning:
  - **Idle** reads `HIGH`
  - **Pressed** reads `LOW` (switch connects signal to GND)
- LEDs are configured as `OUTPUT` pins. Depending on how your LED modules are wired, they may be:
  - **Active-high**: `HIGH` turns on
  - **Active-low**: `LOW` turns on  

## Credits / Acknowledgements
Huge credit to **thomasstar** and the project below:
- **GJ-Tho-Modular-V2** (CAD + modular controller design reference)  
  https://github.com/thomasstar/GJ-Tho-Modular-V2

I used the **CAD files** from thomasstar’s repository as the basis for the physical design/case of this controller.

## License
This repository contains my firmware/build notes for the controller.  
For CAD/design licensing and usage terms, please refer to the original project linked above (thomasstar / GJ-Tho-Modular-V2).

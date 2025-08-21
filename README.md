# BuzzWire Game - Embedded Systems Project

An Arduino-based BuzzWire game where players navigate a wire loop along a twisted path without making contact. Built for embedded systems coursework demonstrating timers, interrupts, pointers, and memory allocation.

## Hardware Components
- ATmega328P (Arduino Uno)
- 4-digit 7-segment display with shift register
- 3 push buttons (start/stop/unused)
- LED for visual feedback
- Passive buzzer for audio feedback
- Wire detection circuit with pull-up resistor
- Wooden game structure with copper wires

## Features
- **Real-time timer display** in MM:SS format
- **Contact counting** with "C###" display format
- **Statistical analysis** of game performance
- **Interrupt-driven wire detection** for immediate response
- **Dynamic memory allocation** for contact timestamps
- **Audio/visual feedback** on wire contact (880Hz buzzer + LED)
- **Serial monitor statistics** with detailed game analysis

## Technical Implementation

### Embedded Systems Requirements Demonstrated:
1. **Timer Usage**: TIMER0 for millisecond timing system
2. **Interrupt Usage**: PCINT1 for wire detection, TIMER0_COMPA for timing
3. **Pointer Usage**: Dynamic memory allocation and external buffer references
4. **Memory Allocation**: malloc/free for contact timestamp array

### Pin Configuration:
- PC5: Wire detection (interrupt-driven)
- PC0: LED output
- PD3: Buzzer output
- PC1: Left button (start game)
- PC3: Right button (stop game)
- Display: Shift register communication (LATCH/CLK/DATA)

## Game States
1. **IDLE**: Display shows "RDY", waiting for start button
2. **RUNNING**: Timer active, contact detection enabled
3. **FINISHED**: Display shows "DONE", statistics printed

## Building and Running
1. Open project in PlatformIO
2. Connect Arduino Uno
3. Upload code to microcontroller
4. Open Serial Monitor for game statistics
5. Press left button to start game

## Project Structure
```
lib/                    # Hardware abstraction libraries
├── buttons/           # Button input handling
├── buzzer/           # Audio feedback system
├── display/          # 7-segment display control
├── game_logic/       # Core game state machine
├── leds/             # LED control
├── millis/           # Millisecond timing system
├── score_logic/      # Game scoring and statistics
├── timer/            # Alternative timer system
├── usart/            # Serial communication
└── wire_interrupt/   # Interrupt-driven wire detection

BuzzW/                 # Main project folder
├── src/main.c        # Main game loop
├── platformio.ini    # PlatformIO configuration
└── include/          # Project headers
```

## Course Requirements Met
- ✅ Timer implementation (TIMER0 CTC mode)
- ✅ Interrupt service routines (PCINT1_vect, TIMER0_COMPA_vect)
- ✅ Pointer usage (dynamic arrays, external references)
- ✅ Memory allocation (malloc/free with error handling)
- ✅ Modular architecture with reusable libraries
- ✅ Real-time embedded system operation

## Game Statistics Output Example
```
=== Game Statistics: ==
Total time: 45 s
Contacts: 3
Penalty: 30 (10s per contact)
Final score: 15
Touch 1: 12 s
Touch 2: 28 s
Touch 3: 41 s
Min interval: 13 s
Max interval: 16 s
Avg interval: 14 s
```

## Author
Kristopher Montgomery - Embedded Systems Course Project

# CHIP-8 Emulator

## Overview

The CHIP-8 Emulator is a project designed to emulate the functionality of the CHIP-8, a simple interpreted programming language developed in the 1970s. This emulator runs classic CHIP-8 programs and games, replicating the behavior of the original virtual machine.

## Features

- Emulates all 35 CHIP-8 instructions.
- Supports original CHIP-8 resolution (64x32) and modern extensions (e.g., Super-CHIP).
- Implements a virtual keypad for input.
- Accurate timing to match the original CHIP-8 clock speed.
- Cross-platform support with a user-friendly interface.

## Requirements

- C++11 or newer.
- SDL2 (Simple DirectMedia Layer) for graphics and input.
- A terminal or console for debugging output (optional).

## Installation

### Prerequisites

Ensure the following dependencies are installed on your system:

1. **SDL2 Library**:

   - On Ubuntu/Debian:
     ```bash
     sudo apt install libsdl2-dev
     ```


2. **C++ Compiler**:

   - GCC (g++) or Clang for Linux/MacOS.
   - MSVC for Windows.

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/SMA-Codes/CHIP8
   cd chip8-emulator
   ```
2. Build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
3. Run the emulator:
   ```bash
   ./chip8-emulator path/to/rom
   ```

## Usage

1. Download a CHIP-8 ROM file (e.g., Tetris, Pong).
2. Launch the emulator and provide the ROM path as an argument:
   ```bash
   ./chip8-emulator roms/tetris.ch8
   ```

   - Mapped to your keyboard:
     ```
     1 2 3 4
     Q W E R
     A S D F
     Z X C V
     ```



## Features in Detail

### Instruction Set

The emulator supports all 35 CHIP-8 instructions, including:

- Arithmetic and bitwise operations.
- Control flow (jumps, calls, etc.).
- Drawing graphics.
- Handling user input.

### Graphics

- Implements the CHIP-8 display resolution (64x32).
- Pixel rendering using SDL2.

### Sound

- Emulates the original CHIP-8 sound timer, producing a beep when the timer reaches zero.

### Timing

- Uses a cycle-accurate clock to emulate the 500 Hz speed of the original CHIP-8 interpreter.

## Future Enhancements

- Add support for Super-CHIP extensions.
- Improve UI/UX for selecting ROMs and configuring settings.
- Add debugging tools (e.g., instruction logging, memory inspection).

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with detailed information about the changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Thanks to the CHIP-8 community and existing resources that have inspired this project.
- Special credit to [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM).


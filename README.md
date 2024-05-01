Sure, here's a sample README for your project:

---

# Simple 8-bit CPU Emulator

This project implements a simple 8-bit CPU emulator in C. The emulator is capable of compiling and executing programs written in a custom assembly-like language, as well as displaying output on a simulated screen.

## Features

- **CPU Emulation**: The emulator simulates the behavior of an 8-bit CPU, including fetching, decoding, and executing instructions.
- **Instruction Set**: Supports a basic instruction set including MOV, STOR, LOAD, INC, ADD, SUB, OR, AND, CMP, JMP, FLG, and RSTF instructions.
- **Program Compilation**: Compiles programs written in a custom assembly-like language into binary format for execution by the CPU.
- **Screen Output**: Simulates a screen where the CPU can output characters.

## Getting Started

### Prerequisites

- C compiler (e.g., GCC)
- Make (optional but recommended)

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/Brulans4/8bits-cpu.git
   ```

2. Navigate to the project directory:

   ```bash
   cd 8bits-cpu

   ```
3. Compile the project:

   ```bash
   make all
   ```

### Usage

1. Compile a program:

   ```bash
   ./main <program.asm>
   ```

   This will generate a binary file named `prog.bin`.
   It will then load and execute the compiled program.
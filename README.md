Sure, here's a sample README for your project:

---

# Simple 8-bit CPU Emulator

This project implements a simple 8-bit CPU emulator in C. The emulator is capable of compiling and executing programs written in a custom assembly-like language, as well as displaying output on a simulated screen.

## Features

- **CPU Emulation**: The emulator simulates the behavior of an 8-bit CPU, including fetching, decoding, and executing instructions.
- **Instruction Set**: Supports a basic instruction set including MOV, STOR, LOAD, INC, ADD, SUB, OR, AND, CMP, JMP, FLG, and RSTF instructions.
- **Program Compilation**: Compiles programs written in a custom assembly-like language into binary format for execution by the CPU.
- **Screen Output**: Simulates a screen where the CPU can output characters.

**Register Information**:
  
| REGISTER | CODE VALUE | 
|----------|------------|
| AL       | 01         |
| BL       | 10         | 
| CL       | 11         |

**Instruction set**:

| OP CODE | Binary | Instruction | REG1 (2 bits) | REG2 (2 bits) | Immediate value (8 bits) | Total (8/16 bits) | Operation | Note |
|---------|--------|-------------|---------------|---------------|--------------------------|-------------------|-----------|------|
| 0       | 0000   | NOP         |               |               |                          | 8                 | NOP       |                              |
| 1       | 0001   | MOV         | X             | X             |                          | 8                 | MOV AL, BL | AL = BL                      |
| 2       | 0010   | STOR        | X             |               | X                        | 16                | STOR 15, AL | [15 + 192] = AL              |
| 3       | 0011   | LOAD        | X             |               | X                        | 16                | LOAD AL, 5 | AL = 5                       |
| 4       | 0100   | INC         | X             | X             |                          | 8                 | INC AL, AL | AL = AL + 1                  |
| 5       | 0101   | ADD         | X             | X             |                          | 8                 | ADD AL, BL | AL = AL + BL                 |
| 6       | 0110   | ADDIV       | X             |               | X                        | 16                | ADDIV AL, 12 | AL = AL + 12               |
| 7       | 0111   | DEC         | X             | X             |                          | 8                 | DEC AL, AL | AL = AL - 1                  |
| 8       | 1000   | SUB         | X             | X             |                          | 8                 | SUB AL, BL | AL = AL - BL                 |
| 9       | 1001   | SUBIV       | X             |               | X                        | 16                | SUBIV AL, 12 | AL = AL - 12               |
| A       | 1010   | OR          | X             | X             |                          | 8                 | OR AL, BL  | AL = OR AL with BL          |
| B       | 1011   | AND         | X             | X             |                          | 8                 | AND AL, BL | AL = AND AL with BL         |
| C       | 1100   | CMP         | X             | X             |                          | 8                 | CMP AL, BL | AL - BL == 0                 |
| D       | 1101   | JMP         | X             |               | X                        | 16                | JMP 5      | PC = 5                       |
| E       | 1110   | FLG         | X             |               | X                        | 16                | FLG 5      | if(fc == 1 OR fz == 0) PC = 5 |
| F       | 1111   | RSTFG       | X             |               |                          | 8                 | RSTF       | fc = 0, fz = 0               |


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
---

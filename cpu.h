#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>
#include <stdbool.h>

#define MEMORY_SIZE 256
#define VIDEO_RAM 64
#define CHAR_MEM 32

/**
 * The CPU struct.
 *
 * This struct represents the CPU of the 8-bit computer.
 */
typedef struct
{
    uint8_t pc;

    bool carryFlag;
    bool zeroFlag;

    uint8_t instructionRegister;

    uint8_t *regs;

    uint8_t *ram;
} CPU;

/**
 * Initializes a CPU object.
 *
 * @return A pointer to the initialized CPU object.
 */
CPU *cpuInit();

/**
 * Frees the memory allocated for a CPU object.
 *
 * @param cpu The CPU object to be freed.
 */
void cpuFree(CPU *cpu);

/**
 * Loads the program into the CPU.
 */
void loadProgram(CPU *cpu);

/**
 * Executes the program loaded into the CPU.
 *
 * @param cpu The CPU object to execute the program on.
 */
void executProgram(CPU *cpu);

/**
 * Fetches the next instruction from the RAM.
 *
 * @param cpu The CPU object to fetch the instruction for.
 */
void fetchInstruction(CPU *cpu);

/**
 * Decodes the instruction fetched from the RAM.
 *
 * @param cpu The CPU object to decode the instruction for.
 * @param inedexRegWrite The index of the register to write to.
 * @param inedexRegRead The index of the register to read from.
 *
 * @return The opcode of the instruction.
 */
int decodeInstruction(CPU *cpu, uint8_t *inedexRegWrite, uint8_t *inedexRegRead);

/**
 * Executes the instruction.
 *
 * @param cpu The CPU object to execute the instruction on.
 * @param opCode The opcode of the instruction.
 * @param inedexRegWrite The index of the register to write to.
 * @param inedexRegRead The index of the register to read from.
 */
void executeInstruction(CPU *cpu, uint8_t opCode, uint8_t inedexRegWrite, uint8_t inedexRegRead);

/**
 * Prints the screen of the CPU.
 *
 * @param cpu The CPU object to print the screen for.
 */
void printScreen(CPU *cpu);

/**
 * Prints the state of the CPU.
 *
 * @param cpu A pointer to the CPU object.
 */
void printCPU(CPU *cpu);

/**
 * Prints the decoded instruction.
 *
 * @param cpu The CPU object to print the decoded instruction for.
 * @param opCode The opcode of the instruction.
 * @param inedexRegWrite The index of the register to write to.
 * @param inedexRegRead The index of the register to read from.
 */
void printDecodedInstruction(CPU *cpu, uint8_t opCode, uint8_t *inedexRegWrite, uint8_t *inedexRegRead);

#endif // _CPU_H_
#ifndef COMP_H
#define COMP_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Compiles the given program and loads it into the CPU.
 *
 * @param cpu The CPU object to load the program into.
 * @param program The program to compile and load into the CPU.
 */
void compileProgram(char *program);

/**
 * Gets the opcode of the given keyword.
 *
 * @param keyWord The keyword to get the opcode for.
 *
 * @return The opcode of the keyword.
 */
int getOpcode(char *keyWord);

/**
 * Converts the given argument to an integer.
 *
 * @param argument The argument to convert.
 *
 * @return The integer representation of the argument.
 */
int convertArgument(char *argument, uint8_t *imediateValue);

#endif // COMP_H
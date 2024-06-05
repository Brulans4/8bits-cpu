#include <stdio.h>
#include <stdlib.h>

#include "comp.h"
#include "cpu.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("missing argument <file to compile>\n");
        exit(EXIT_FAILURE);
    }
    if (argc > 2)
    {
        printf("too many arguments\n");
        exit(EXIT_FAILURE);
    }
    compileProgram(argv[1]);

    CPU *cpu = cpuInit();

    loadProgram(cpu);
    executProgram(cpu);
    printScreen(cpu);

    cpuFree(cpu);
}
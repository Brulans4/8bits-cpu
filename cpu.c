#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "cpu.h"
#include "utils.h"

CPU *cpuInit()
{
    CPU *cpu = smalloc(sizeof(CPU));

    cpu->pc = 0;

    cpu->carryFlag = false;
    cpu->zeroFlag = false;

    cpu->instructionRegister = 0;

    cpu->regs = smalloc(sizeof(uint8_t) * 3);
    for (size_t i = 0; i < 3; i++)
    {
        cpu->regs[i] = 0;
    }

    cpu->ram = smalloc(sizeof(uint8_t) * MEMORY_SIZE);
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        cpu->ram[i] = 0;
    }

    cpu->ram[160] = ' ';
    for (size_t i = 0; i < 26; i++)
    {
        cpu->ram[i + 161] = 'A' + i;
    }
    cpu->ram[187] = '.';
    cpu->ram[188] = ',';
    cpu->ram[189] = '!';
    cpu->ram[190] = '?';
    cpu->ram[191] = ':';

    return cpu;
}

void loadProgram(CPU *cpu)
{

    char line[3] = {0};
    FILE *progFile = fopen("prog.bin", "r");
    if (progFile == NULL)
    {
        printf("Error opening file!\n");
        cpuFree(cpu);
        exit(EXIT_FAILURE);
    }

    int index = 0;
    while (fgets(line, sizeof(line), progFile) != NULL)
    {
        int value = strtol(line, NULL, 16);

        cpu->ram[index] = value;
        index++;

        if (index == MEMORY_SIZE - (VIDEO_RAM + CHAR_MEM))
        {
            printf("Program too big !\n");
            cpuFree(cpu);
            exit(EXIT_FAILURE);
        }
    }
    fclose(progFile);
}

void executProgram(CPU *cpu)
{
    while (true)
    {
        fetchInstruction(cpu);

        uint8_t inedexRegWrite;
        uint8_t inedexRegRead;
        uint8_t opCode = decodeInstruction(cpu, &inedexRegWrite, &inedexRegRead);

        if (opCode == 0)
        {
            break;
        }

        executeInstruction(cpu, opCode, inedexRegWrite, inedexRegRead);
    }
}

void fetchInstruction(CPU *cpu)
{
    cpu->instructionRegister = cpu->ram[cpu->pc];
}

int decodeInstruction(CPU *cpu, uint8_t *inedexRegWrite, uint8_t *inedexRegRead)
{
    uint8_t opCode = cpu->instructionRegister >> 4;
    *inedexRegWrite = (cpu->instructionRegister - (opCode << 4)) >> 2;
    *inedexRegRead = (cpu->instructionRegister - (opCode << 4)) - (*inedexRegWrite << 2);

    return opCode;
}

void executeInstruction(CPU *cpu, uint8_t opCode, uint8_t inedexRegWrite, uint8_t inedexRegRead)
{
    inedexRegWrite--;
    inedexRegRead--;

    switch (opCode)
    {
    case 0: // NOP
        cpu->pc++;
        break;
    case 1: // MOV
        cpu->regs[inedexRegWrite] = cpu->regs[inedexRegRead];
        if (cpu->regs[inedexRegWrite] == 0)
        {
            cpu->zeroFlag = true;
        }

        cpu->pc++;
        break;
    case 2: // STOR
        cpu->ram[cpu->ram[cpu->pc + 1] + (MEMORY_SIZE - VIDEO_RAM)] = cpu->regs[inedexRegRead];

        cpu->pc += 2;
        return;
    case 3: // LOAD
        cpu->regs[inedexRegWrite] = cpu->ram[cpu->pc + 1];

        cpu->pc += 2;
        break;
    case 4: // INC
        if (cpu->regs[inedexRegRead] == 255)
        {
            cpu->carryFlag = true;
        }

        cpu->regs[inedexRegWrite] = cpu->regs[inedexRegRead] + 1;
        if (cpu->regs[inedexRegWrite] == 0)
        {
            cpu->zeroFlag = true;
        }

        cpu->pc++;
        break;
    case 5: // ADD
        if (cpu->regs[inedexRegRead] > 255 - cpu->regs[inedexRegWrite])
        {
            cpu->carryFlag = true;
        }

        cpu->regs[inedexRegWrite] = cpu->regs[inedexRegWrite] + cpu->regs[inedexRegRead];
        if (cpu->regs[inedexRegWrite] == 0)
        {
            cpu->zeroFlag = true;
        }

        cpu->pc++;
        break;
    case 6: // ADDIV
        if (cpu->regs[inedexRegRead] > 255 - cpu->regs[inedexRegWrite])
        {
            cpu->carryFlag = true;
        }

        cpu->regs[inedexRegWrite] = cpu->regs[inedexRegWrite] + cpu->ram[cpu->pc + 1];
        if (cpu->regs[inedexRegWrite] == 0)
        {
            cpu->zeroFlag = true;
        }

        cpu->pc += 2;
        break;
    case 7: // DEC
        cpu->regs[inedexRegWrite] = cpu->regs[inedexRegRead] - 1;
        if (cpu->regs[inedexRegWrite] == 0)
        {
            cpu->zeroFlag = true;
        }

        cpu->pc++;
        break;
    case 8: // SUB
        if (cpu->regs[inedexRegRead] > cpu->regs[inedexRegWrite])
        {
            cpu->carryFlag = true;
        }
        cpu->regs[inedexRegWrite] = cpu->regs[inedexRegWrite] - cpu->regs[inedexRegRead];
        if (cpu->regs[inedexRegWrite] == 0)
        {
            cpu->zeroFlag = true;
        }

        cpu->pc++;
        break;
    case 9: // SUBIV
        if (cpu->ram[cpu->pc + 1] > cpu->regs[inedexRegWrite])
        {
            cpu->carryFlag = true;
        }
        cpu->regs[inedexRegWrite] = cpu->regs[inedexRegWrite] - cpu->ram[cpu->pc + 1];
        if (cpu->regs[inedexRegWrite] == 0)
        {
            cpu->zeroFlag = true;
        }

        cpu->pc += 2;
        break;
    case 10: // OR
        cpu->regs[inedexRegWrite] = cpu->regs[inedexRegWrite] | cpu->regs[inedexRegRead];
        if (cpu->regs[inedexRegWrite] == 0)
        {
            cpu->zeroFlag = true;
        }

        cpu->pc++;
        break;
    case 11: // AND
        cpu->regs[inedexRegWrite] = cpu->regs[inedexRegWrite] & cpu->regs[inedexRegRead];
        if (cpu->regs[inedexRegWrite] == 0)
        {
            cpu->zeroFlag = true;
        }

        cpu->pc++;
        break;
    case 12: // CMP
        cpu->carryFlag = cpu->regs[inedexRegWrite] > cpu->regs[inedexRegRead];

        cpu->pc++;
        break;
    case 13: // JMP
        cpu->pc = cpu->ram[cpu->pc + 1];
        break;
    case 14: // FLG
        if (cpu->carryFlag || cpu->zeroFlag)
        {
            cpu->pc = cpu->ram[cpu->pc + 1];
        }
        else
        {
            cpu->pc += 2;
        }
        break;
    case 15: // RSTFG
        cpu->carryFlag = false;
        cpu->zeroFlag = false;

        cpu->pc++;
        break;
    }
}

void printScreen(CPU *cpu)
{
    printf("|========|\n");
    printf("| SCREEN |\n");
    printf("|========|\n|");
    for (int i = 0; i < 64; i++)
    {
        printf("%c", cpu->ram[cpu->ram[i + 192] + 160]);
        if (i % 8 == 7)
        {
            printf("|\n|");
        }
    }
    printf("========|\n");
}

void cpuFree(CPU *cpu)
{
    free(cpu->regs);
    free(cpu->ram);

    free(cpu);
}

void printCPU(CPU *cpu)
{
    printf("==================================CPU==================================\n");
    printf("\n");
    printf("PC: %x\n", cpu->pc);
    printf("\n");
    printf("Carry Flag: %s\n", cpu->carryFlag ? "true" : "false");
    printf("Zero Flag: %s\n", cpu->zeroFlag ? "true" : "false");
    printf("\n");
    printf("IN: %x\n", cpu->instructionRegister);
    printf("AL: %x\n", cpu->regs[0]);
    printf("BL: %x\n", cpu->regs[1]);
    printf("CL: %x\n", cpu->regs[2]);
    printf("\n");
    printf("RAM:\n");
    printf("--------------------------------------------------------------------------------------\n");

    printf("|   |");
    for (int i = 0; i < 16; i++)
    {
        printf("| 0%x ", i);
    }
    printf("|\n");
    printf("======================================================================================\n");
    for (int i = 0; i < 16; i++)
    {
        printf("| %x |", i);
        for (int j = 0; j < 16; j++)
        {
            uint8_t value = cpu->ram[i * 16 + j];
            if (value < 16)
            {
                printf("| 0%x ", value);
            }
            else
            {
                printf("| %x ", value);
            }
        }
        printf("|\n");
    }
    printf("--------------------------------------------------------------------------------------\n");
    printf("\n");
    printf("======================================================================================\n");
    printf("\n");
}

void printDecodedInstruction(CPU *cpu, uint8_t opCode, uint8_t *inedexRegWrite, uint8_t *inedexRegRead)
{
    switch (opCode)
    {
    case 0:
        printf("opCode: NOP\n");
        break;

    case 1:
        printf("opCode: MOV\n");
        break;
    case 2:
        printf("opCode: STOR\n");
        break;
    case 3:
        printf("opCode: LOAD\n");
        break;
    case 4:
        printf("opCode: INC\n");
        break;
    case 5:
        printf("opCode: ADD\n");
        break;
    case 6:
        printf("opCode: ADDIV\n");
        break;
    case 7:
        printf("opCode: DEC\n");
        break;
    case 8:
        printf("opCode: SUB\n");
        break;
    case 9:
        printf("opCode: SUBIV\n");
        break;
    case 10:
        printf("opCode: OR\n");
        break;
    case 11:
        printf("opCode: AND\n");
        break;
    case 12:
        printf("opCode: CMP\n");
        break;
    case 13:
        printf("opCode: JMP\n");
        break;
    case 14:
        printf("opCode: FLG\n");
        break;
    case 15:
        printf("opCode: RSTFG\n");
        break;
    }

    if (*inedexRegWrite == 1)
    {
        printf("Write: AL\n");
    }
    else if (*inedexRegWrite == 2)
    {
        printf("Write: BL\n");
    }
    else if (*inedexRegWrite == 3)
    {
        printf("Write: CL\n");
    }
    else
    {
        printf("Write: RAM[%x]\n", cpu->pc + 1);
    }

    if (*inedexRegRead == 1)
    {
        printf("Read: AL\n");
    }
    else if (*inedexRegRead == 2)
    {
        printf("Read: BL\n");
    }
    else if (*inedexRegRead == 3)
    {
        printf("Read: CL\n");
    }
    else
    {
        printf("Read: RAM[%x]\n", cpu->pc + 1);
    }
    printf("\n");
}
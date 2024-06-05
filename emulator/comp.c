#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comp.h"
#include "utils.h"

void compileProgram(char *pathProgram)
{
    char line[32] = {0};
    FILE *progFile = fopen(pathProgram, "r");
    if (progFile == NULL)
    {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    FILE *binFile = fopen("prog.bin", "w");
    if (binFile == NULL)
    {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), progFile) != NULL)
    {
        char keyWord[8] = {0};
        int j = 0;
        int i = 0;

        for (; i < 16; i++)
        {
            if ('A' <= line[i] && line[i] <= 'Z')
            {
                keyWord[j] = line[i];
                j++;
            }
            else
            {
                break;
            }
        }

        uint8_t opcode = getOpcode(keyWord);
        uint8_t instruction = (opcode << 4);
        if (opcode == 0)
        {
            fprintf(binFile, "0%x", instruction);
            continue;
        }
        if (opcode == 15)
        {
            fprintf(binFile, "%x", instruction);
            continue;
        }

        char arg1[8] = {0};
        char arg2[8] = {0};

        j = 0;
        int isArg1 = 1;

        for (; i < 16; i++)
        {
            if (line[i] == ';')
            {
                break;
            }

            if (line[i] == ',')
            {
                isArg1 = 0;
                j = 0;
                continue;
            }

            if (('A' <= line[i] && line[i] <= 'Z') || ('0' <= line[i] && line[i] <= '9'))
            {
                if (isArg1)
                {
                    arg1[j] = line[i];
                    j++;
                }
                else
                {
                    arg2[j] = line[i];
                    j++;
                }
            }
        }

        if (opcode == 13 || opcode == 14)
        {
            fprintf(binFile, "%x", instruction);
            uint8_t imediateValue = atoi(arg1);
            if (imediateValue < 16)
            {
                fprintf(binFile, "0%x", imediateValue);
            }
            else
            {
                fprintf(binFile, "%x", imediateValue);
            }
            continue;
        }
        else
        {
            uint8_t imediateValue = 0;
            uint8_t reg1 = convertArgument(arg1, &imediateValue);
            uint8_t reg2 = convertArgument(arg2, &imediateValue);

            if (reg1 != 0)
            {
                instruction += (reg1 << 2);
            }
            if (reg2 != 0)
            {
                instruction += reg2;
            }

            fprintf(binFile, "%x", instruction);
            if (reg1 == 0 || reg2 == 0)
            {
                if (imediateValue < 16)
                {
                    fprintf(binFile, "0%x", imediateValue);
                }
                else
                {
                    fprintf(binFile, "%x", imediateValue);
                }
            }
        }
    }
    fclose(binFile);
    fclose(progFile);
}

int getOpcode(char *keyWord)
{
    for (int i = 0; i < 16; i++)
    {
        if (strcmp(keyWord, instructionSet[i]) == 0)
        {
            return i;
        }
    }
    printf("Error: Invalid keyword\n");
    exit(EXIT_FAILURE);
}

int convertArgument(char *arg, uint8_t *imediateValue)
{
    if (strcmp(arg, "AL") == 0)
    {
        return 1;
    }
    if (strcmp(arg, "BL") == 0)
    {
        return 2;
    }
    if (strcmp(arg, "CL") == 0)
    {
        return 3;
    }
    *imediateValue = atoi(arg);
    return 0;
}
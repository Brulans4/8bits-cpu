#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "utils.h"

const char *instructionSet[] = {
    "NOP",
    "MOV",
    "STOR",
    "LOAD",
    "INC",
    "ADD",
    "ADDIV",
    "DEC",
    "SUB",
    "SUBIV",
    "OR",
    "AND",
    "CMP",
    "JMP",
    "FLG",
    "RSTF",
};

void *smalloc(size_t size)
{
  void *res = malloc(size);
  checkCond(size != 0 && res == NULL, "ERROR MALLOC");
  return res;
}

void checkCond(bool cond, char *msg)
{
  if (cond)
  {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

void checkNeg(int res, char *msg)
{
  checkCond(res < 0, msg);
}

void checkNull(void *res, char *msg)
{
  checkCond(res == NULL, msg);
}
#ifndef DICO_INSTRUCT_H
#define DICO_INSTRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

typedef struct
{
    char id[5];
    char type;
    char typeNumber;
} INSTRUCTION;

extern int sizeDico;

INSTRUCTION* AllocateDico(FILE *file, int* sizeDico);

INSTRUCTION* InitializeDicoInstruct(char* nameDicoFile);

int IndexInstruction(INSTRUCTION* dictionary, char *instructionName);

#endif
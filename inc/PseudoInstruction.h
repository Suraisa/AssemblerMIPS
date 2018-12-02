#ifndef PSEUDO_INSTRUCTION_H
#define PSEUDO_INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>

#include "Utils.h"
#include "Lexeme.h"
#include "FSMLexeme.h"
typedef struct
{
    char id[30];
    char operands[3];
    int typeNumber;
} PSEUDO_INSTRUCTION;

extern int sizePseudoDico;

#include "Section.h"

PSEUDO_INSTRUCTION* AllocatePseudoDico(FILE *file, int* sizeDico);

PSEUDO_INSTRUCTION* InitializePseudoDicoInstruct(char *nameDicoFile);

int FindPseudoInstruction(char* nameToFind, FILE** dicoPseudoInstruction, SECTION* section);

LIST_DOUBLE CreateNewInstruction(FILE** dicoPseudoInstruction, SECTION* section);

int IndexPseudoInstruction(PSEUDO_INSTRUCTION* dictionary, char *instructionName);

int CreateNewListLexeme(FILE** readingFile, QUEUE_DOUBLE* lexemeQueue, SECTION* operands);

#endif
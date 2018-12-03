#ifndef DICO_INSTRUCT_H
#define DICO_INSTRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

typedef struct{
unsigned int func:6,
sa:5,
rd:5 ,
rt:5,
rs:5,
opcode:6;
} R;

typedef struct{
unsigned int immediate:16,
rt:5,
rs:5,
opcode:6;
} I;

typedef struct{
unsigned int target:26,
opcode:6;
} J;

typedef union {
  struct R r_inst ;
  struct I i_inst ;
  struct J j_inst ;
} BIT_FIELD;

typedef struct{
  char[4] order;
  char modificable;
  char type;
} DETAILS_INSTRUCT;

typedef struct
{
    char id[30];
    char operands[3];
    int hasPseudoInstruction;
    int typeNumber;
    BIT_FIELD bitField;
    DETAILS_INSTRUCT details;
} INSTRUCTION;

/**
 * @brief Contain the size of the instruction dictionary.
 *
 */
extern int sizeDico;

/**
 * @param file FILE* who contain the pointeur on the file that you want to us to construction the dictionary
 * @param sizeDico int* to receive the size of the dictionary.
 * @brief Allocate the instruction's dictionary.
 * @return The allocated dictionary.
 *
 */
INSTRUCTION* AllocateDico(FILE *file, int* sizeDico);

/**
 * @param nameDicoFile char* which correspond to the file you want to open to initialize the dictionary.
 * @brief Initialiaze the instruction's dictionary.
 * @return The initialized dictionary.
 *
 */
INSTRUCTION* InitializeDicoInstruct(char* nameDicoFile);

/**
 * @param dictionary INSTRUCTION* which is a instructions' dictionary.
 * @param instructionName char* which the instruction to find in the dictionary.
 * @brief Find an instruction in the dictionary.
 * @return The index of the instructionName in the dictionary or -1 if is not inside.
 *
 */
int IndexInstruction(INSTRUCTION* dictionary, char *instructionName);

#endif

#ifndef BIT_TREATMENT_H
#define BIT_TREATMENT_H

#include "DicoInstruct.h"
#include "FSMCollection.h"
#include "DoubleListGeneric.h"
#include "Section.h"

typedef enum
{
    rs = 's',
    rt = 't',
    rd = 'd',
    sa = 'a'
} REGISTER_ORDER;

typedef struct
{
    BIT_FIELD* bitField;
    unsigned long int size;
} SECTION_FIELD;

SECTION_FIELD CreateSectionField(unsigned long int size);

unsigned long int BitBssTreatment(LIST_DOUBLE bss);

SECTION_FIELD BitInstructionTreatment(INSTRUCTION* instruction, LIST_DOUBLE instructions);

#endif
#ifndef BIT_TREATMENT_H
#define BIT_TREATMENT_H

#include "DicoInstruct.h"
#include "FSMCollection.h"
#include "DoubleListGeneric.h"
#include "Section.h"

typedef struct
{
    BIT_FIELD* bitField;
    unsigned long int size;
} SECTION_FIELD;

SECTION_FIELD CreateSectionField(unsigned long int size);

SECTION_FIELD BitInstructionTreatment(INSTRUCTION* instruction, LIST_DOUBLE instructions);

#endif
#ifndef SECTION_H
#define SECTION_H

#include "ListGeneric.h"

typedef struct
{
    int operandNumber;
    unsigned long int shift;
    LIST lexemeList[3];
} SECTION;

#endif
#ifndef PSEUDO_INSTRUCTION_H
#define PSEUDO_INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>

#include "Utils.h"
#include "Section.h"


LIST_DOUBLE FindPseudoInstruction(char* nameToFind, FILE* dicoPseudoInstruction);

#endif
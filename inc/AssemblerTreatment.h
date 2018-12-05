#ifndef ASSEMBLER_TREATMENT_H
#define ASSEMBLER_TREATMENT_H

#include <stdio.h>
#include <stdlib.h>

#include "Global.h"
#include "DoubleListGeneric.h"
#include "FSMLexeme.h"
#include "Section.h"
#include "Lexeme.h"

void OpenAssembleeFile(int argc, char *argv[], FILE** readingFile);

int LexemePass(FILE** readingFile, QUEUE_DOUBLE* lexemeQueue);

int LabelTreatment(QUEUE_DOUBLE* lexemeQueue, INSTRUCTION* dicoInstruct, LIST_DOUBLE* hash);

#endif
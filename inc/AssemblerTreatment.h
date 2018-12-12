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

int CollectionPass(INSTRUCTION** dictionary, PSEUDO_INSTRUCTION** pseudoDictionary, LIST_DOUBLE* lexemeQueue, COLLECTION_FSM* collectionStateMachine, COLLECTION_LISTS* collections, LIST_DOUBLE* allSymbol);

int InstructLabelTreatment(QUEUE_DOUBLE* lexemeQueue, INSTRUCTION* dicoInstruct, LIST_DOUBLE* hash);

void DataLabelTreatment(QUEUE_DOUBLE* lexemeQueue, INSTRUCTION* dicoInstruct, LIST_DOUBLE* hash);

int LabelTreatment(COLLECTION_LISTS* lexemeQueue, INSTRUCTION* dicoInstruct);

#endif
#ifndef SECTION_H
#define SECTION_H

#include "DoubleListGeneric.h"
#include "DoubleQueueGeneric.h"

#ifndef ENUM_COLLECTION
#define ENUM_COLLECTION
typedef enum
{
    TEXT,
    DATA,
    BSS
} COLLECTIONS;
#endif

typedef enum
{
    INST,
    DIR,
    LABEL
} COLLECTION_DATA_TYPE;

typedef struct
{
    char* name;
    int operandNumber;
    unsigned long int lineNumber;
    QUEUE_DOUBLE lexemeList[3];
} INSTRUCTION_DATA;

typedef struct
{
    COLLECTIONS section;
    QUEUE_DOUBLE lexemeList;
} LABEL_DATA;

typedef union
{
    INSTRUCTION_DATA instruction;
    LIST_DOUBLE directiveValue;
    LABEL_DATA label;
} COLLECTION_DATA;

typedef struct
{
    QUEUE_DOUBLE collection[3];
    LIST_DOUBLE* labelTable;
} COLLECTION_LISTS;

#include "FSMCollection.h"

typedef struct
{
    COLLECTION_STATE state;
    unsigned long int shift;
    char dataType;
    COLLECTION_DATA data;
} SECTION;

extern char* collectionType[8];

extern char* collectionSection[3];

#include "HashTable.h"

void InitializeCollectionLists(COLLECTION_LISTS* collections);

SECTION* CreateInstructionSection(COLLECTION_STATE state, unsigned long int shift, char* instructionName, int operandNumber, unsigned long int lineNumber);

SECTION* CreateDirectiveSection(COLLECTION_STATE state, unsigned long int shift, LIST_DOUBLE* lexemeList);

SECTION* CreateLabelSection(COLLECTION_FSM stateMachine, LIST_DOUBLE* lexemeList);

int NumberLexemeOperand(LIST_DOUBLE lexemeList);

int AddOperand(COLLECTION_FSM* stateMachine, SECTION* section, LIST_DOUBLE* lexemeList);

void DisplaySection(void* value);

void DisplayCollectionLists(COLLECTION_LISTS collections);

void ErasedSection(void* value);

void ErasedCollectionLists(void* value);

#endif
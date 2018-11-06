#ifndef SECTION_H
#define SECTION_H

#include "ListGeneric.h"
#include "QueueGeneric.h"

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
    QUEUE lexemeList[3];
} INSTRUCTION_DATA;

typedef struct
{
    COLLECTIONS section;
    QUEUE lexemeList;
} LABEL_DATA;

typedef union
{
    INSTRUCTION_DATA instruction;
    LIST directiveValue;
    LABEL_DATA label;
} COLLECTION_DATA;

typedef struct
{
    QUEUE collection[3];
    LIST* labelTable;
} COLLECTION_LISTS;

#include "FSMCollection.h"

typedef struct
{
    COLLECTION_STATE state;
    unsigned long int shift;
    char dataType;
    COLLECTION_DATA data;
} SECTION;

extern char* collectionType[11];

extern char* collectionSection[3];

#include "HashTable.h"

void InitializeCollectionLists(COLLECTION_LISTS* collections);

SECTION* CreateInstructionSection(COLLECTION_STATE state, unsigned long int shift, char* instructionName, int operandNumber, unsigned long int lineNumber);

SECTION* CreateDirectiveSection(COLLECTION_STATE state, unsigned long int shift, LIST* lexemeList);

SECTION* CreateLabelSection(COLLECTION_FSM stateMachine, LIST* lexemeList);

int NumberLexemeOperand(LIST lexemeList);

int AddOperand(COLLECTION_FSM* stateMachine, SECTION* section, LIST* lexemeList);

void DisplaySection(void* value);

void DisplayCollectionLists(COLLECTION_LISTS collections);

void ErasedSection(void* value);

void ErasedCollectionLists(void* value);

#endif
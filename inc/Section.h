#ifndef SECTION_H
#define SECTION_H

#include "ListGeneric.h"
#include "QueueGeneric.h"
#include "HashTable.h"

typedef enum
{
    TEXT,
    DATA,
    BSS
} COLLECTIONS;

typedef struct
{
    int operandNumber;
    QUEUE lexemeList[3];
} INSTRUCTION_DATA;

typedef union
{
    INSTRUCTION_DATA instruction;
    LIST directiveValue;
    COLLECTIONS section;
} COLLECTION_DATA;

typedef struct
{
    QUEUE collection[3];
    unsigned long int shift[3];
    LIST labelTable[HASHLENGTH];
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

void InitializeCollectionLists(COLLECTION_LISTS* collections);

SECTION* CreateDirectiveSection(COLLECTION_STATE state, unsigned long int shift, LIST lexemeList);

void DisplaySection(void* value);

void ErasedSection(void* value);

void ErasedCollectionLists(void* value);

#endif
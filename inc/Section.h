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

typedef union
{
    QUEUE lexemeList[3];
    COLLECTIONS section;
}COLLECTION_DATA;

typedef struct
{
    QUEUE collection[3];
    LIST* labelTable;
} COLLECTION_LISTS;

#include "FSMCollection.h"

typedef struct
{
    COLLECTION_STATE state;
    int operandNumber;
    unsigned long int shift;
    char dataType;
    COLLECTION_DATA data;
} SECTION;

void InitializeCollectionLists(COLLECTION_LISTS* collections);

// void PushCollections(COLLECTION_LISTS* collection, );

#endif
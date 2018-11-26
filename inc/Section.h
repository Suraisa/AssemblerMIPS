#ifndef SECTION_H
#define SECTION_H

#include "DoubleListGeneric.h"
#include "DoubleQueueGeneric.h"
#include "FSMOperand.h"
#include "DicoInstruct.h"

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
    int dicoIndex;
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

/**
 * @brief Table of collection's Type equivalent to the COLLECTION_STATE.
 *
 */
extern char* collectionType[8];

/**
 * @brief Table of collection's section equivalent to the COLLECTIONS.
 *
 */
extern char* collectionSection[3];

#include "HashTable.h"

/**
 * @param collections COLLECTION_LISTS* which is the collection list to initialize.
 * @brief Table of collection's section equivalent to the COLLECTIONS.
 *
 */
void InitializeCollectionLists(COLLECTION_LISTS* collections);

/**
 * @param collections COLLECTION_LISTS* which is the collection list to initialize.
 * @brief Table of collection's section equivalent to the COLLECTIONS.
 * @return an instruction SECTION*
 */
SECTION* CreateInstructionSection(COLLECTION_STATE state, unsigned long int shift, char* instructionName, int dicoIndex, unsigned long int lineNumber);

SECTION* CreateDirectiveSection(COLLECTION_STATE state, unsigned long int shift, LIST_DOUBLE* lexemeList);

SECTION* CreateLabelSection(COLLECTION_FSM stateMachine, LIST_DOUBLE* lexemeList);

int NumberLexemeOperand(LIST_DOUBLE lexemeList);

int AddOperand(COLLECTION_FSM* stateMachine, SECTION* section, LIST_DOUBLE* lexemeList, INSTRUCTION* instructionDictionary);

void DisplaySection(void* value);

void DisplayCollectionLists(COLLECTION_LISTS collections);

void ErasedSection(void* value);

void ErasedCollectionLists(void* value);

#endif
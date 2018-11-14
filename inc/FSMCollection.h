#ifndef FSM_COLLECTION_H
#define FSM_COLLECTION_H

#include "Lexeme.h"
#include "DicoInstruct.h"
#include "DoubleListGeneric.h"
#include "DoubleQueueGeneric.h"
#include "Utils.h"
#include "limits.h"

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
    DIR0,
    ASCIZZ,
    SPACE,
    WORD,
    BYTE,
    SET_NOREORDER,
    INSTRUCTION0,
    INSTRUCTION1,
    INIT_COLLECTION
} COLLECTION_STATE;

/*
Needed variables for the state machine
*/
typedef struct
{
    COLLECTIONS actualCollection;
    COLLECTION_STATE previousState;
    COLLECTION_STATE currentState;
    char error;
    char inState;
    unsigned long int shift[3];
    unsigned long int nextShift[3];
} COLLECTION_FSM;

#include "Section.h"

int InitializationCollection(COLLECTION_LISTS* collectionLists);

/**
 * @param stateMachine Pointeur on the stateMachine to initialize.
 * @brief Initialize the stateMachine.
 *
 */
void InitializationCollectionFsm(COLLECTION_FSM* stateMachine);

void InitCollectionFsm(COLLECTION_FSM *stateMachine, QUEUE_DOUBLE *lexemeQueue);

void CollectionFsm(COLLECTION_FSM *stateMachine, QUEUE_DOUBLE *lexemeQueue, COLLECTION_LISTS* collections, INSTRUCTION* instructionDictionary);

void PrintErrorCollection(COLLECTION_FSM* stateMachine, unsigned long int lineNumber, char *problem, char* why, char* type);

#endif
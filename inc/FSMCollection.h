#ifndef FSM_COLLECTION_H
#define FSM_COLLECTION_H

#include "Lexeme.h"
#include "ListGeneric.h"
#include "QueueGeneric.h"
#include "Utils.h"
#include "limits.h"

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
    INSTRUCTION2,
    INSTRUCTION3,
    INSTRUCTION4,
    INIT_COLLECTION
} COLLECTION_STATE;

#include "Section.h"

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


/**
 * @param stateMachine Pointeur on the stateMachine to initialize.
 * @brief Initialize the stateMachine.
 *
 */
void InitializationCollectionFsm(COLLECTION_FSM* stateMachine);

void InitCollectionFsm(COLLECTION_FSM *stateMachine, QUEUE *lexemeQueue);

void CollectionFsm(COLLECTION_FSM *stateMachine, QUEUE *lexemeQueue, COLLECTION_LISTS* collections);

#endif
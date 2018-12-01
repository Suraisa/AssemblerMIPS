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

#ifndef ENUM_COLLECTION_STATE
#define ENUM_COLLECTION_STATE
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
#endif

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
#include "PseudoInstruction.h"

int InitializationCollection(COLLECTION_LISTS* collectionLists);

/**
 * @param stateMachine Pointeur on the stateMachine to initialize.
 * @brief Initialize the stateMachine.
 *
 */
void InitializationCollectionFsm(COLLECTION_FSM* stateMachine);

/**
 * @param stateMachine collection_list* to fill the error part or other value inside.
 * @param lexemeQueue QUEUE_DOUBLE* the queue of lexeme to treat grammatically.
 * @brief It's the Init stateMachine which change the stateMachine's state to the right one.
 * 
 */
void InitCollectionFsm(COLLECTION_FSM *stateMachine, QUEUE_DOUBLE *lexemeQueue);

/**
 * @param stateMachine collection_list* to fill the error part or other value inside.
 * @param lexemeQueue QUEUE_DOUBLE* the queue of lexeme to treat grammatically.
 * @param collections COLLECTION_LISTS* to fill in.
 * @param instructionDictionary INSTRUCTION* the instructions' dictionary to test the instruction in label.
 * @brief It's the Init stateMachine which change the stateMachine's state to the right one.
 * 
 */
void CollectionFsm(COLLECTION_FSM *stateMachine, QUEUE_DOUBLE *lexemeQueue, COLLECTION_LISTS* collections, INSTRUCTION* instructionDictionary, PSEUDO_INSTRUCTION* pseudoDictionary);

/**
 * @param stateMachine COLLECTION_FSM* to change the error state and the inState state.
 * @param lineNumber unsigned long int to show the line number.
 * @param problem char* to explain the problem.
 * @param type char* to show where do we have the problem.
 * @brief Print an the error of the stateMachine in color.
 *
 */
void PrintErrorCollection(COLLECTION_FSM* stateMachine, unsigned long int lineNumber, char *problem, char* why, char* type);

#endif
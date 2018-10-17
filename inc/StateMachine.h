/**
 * @file StateMachine.h
 * @brief StateMachine-related stuff.
 *
 * Contains the stateMachine structure definition and the associated prototypes.
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdio.h>

#include "ListGeneric.h"
#include "QueueGeneric.h"
#include "DisplayType.h"

typedef enum
{
    PARENTHESISLEFT,
    PARENTHESISRIGHT,
    COMMA,
    REGISTER,
    DIRECTIVE,
    COMMENT,
    NUMBER,
    HEXADECIMAL,
    DECIMAL,
    STRING,
    SYMBOL,
    RETURN,
    COLON,
    OPERATOR,
    INIT
} STATE;

/*
Needed variables for the state machine
*/
typedef struct
{
    STATE currentState;
    char error;
    char inState;
} STATEMACHINE;

/**
 * @param stateMachine Pointeur on the stateMachine to initialize.
 * @brief Initialize the stateMachine.
 *
 */
void Initialization(STATEMACHINE* stateMachine);

/**
 * @param readingChar Char which send you to the correct state.
 * @param stateMachine Pointeur on the stateMachine to change the state or add an error.
 * @param lineNumber Unsigned long int represent the line position in the file to decrypt.
 * @brief It's the Init stateMachine which change the stateMachine's state to the right one.
 *
 */
void InitStateMachine(char readingChar, STATEMACHINE *stateMachine, unsigned long int lineNumber);

/**
 * @param readingChar Pointer on the char currently read in the file to decrypt.
 * @param lexemeQueue Pointeur on a QUEUE of Lexemes it updates it during the process of lexeme decrypting.
 * @param readingValue Pointeur on a LIST, to stock values which don't have lexeme for now (this function will use it, use just an empty list).
 * @param stateMachine Pointeur on the stateMachine. To know in which STATE are the treatment or to add an error.
 * @param lineNumber Pointer on an unsigned long int represent the line position in the file to decrypt. It will be incrementated during the process.
 * @param finishedFile Integer to know if the file is ended or not. 
 * @brief It's the brain of the stateMachine which sends the stateMachine to the right state and do a process according of this state.
 *
 */
void StateMachine(char *readingChar, QUEUE *lexemeQueue, LIST *readingValue, STATEMACHINE *stateMachine, unsigned long int *lineNumber, int finishedFile);

/**
 * @param lexemeQueue Pointeur on a QUEUE of Lexemes that you want to update.
 * @param readingValue Pointeur on a LIST of the lexeme value's information.
 * @param state STATE represent the state of the lexeme to add.
 * @param lineNumber Unsigned long int represent the line position of this lexeme.
 * @param finishedFile Integer to know if the file is ended or not. 
 * @brief Add a Lexeme in the lexemeQueue according of the state, the list of value read and the number of line.
 *
 */
void LexemeTreatment(QUEUE* lexemeQueue, STATE state, LIST* readingValue, unsigned long int lineNumber);

/**
 * @param lineNumber Unsigned long int represent the line position of the error. 
 * @param problem String to represent the problem.
 * @param wrongValue Char which create an error.
 * @param state String to represent in which state we have an error.
 * @brief Print an the error of the stateMachine in color.
 *
 */
void PrintError(unsigned long int lineNumber, char* problem, char wrongValue, char* state);

#include "Lexeme.h"

#endif
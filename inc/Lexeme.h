/**
 * @file Lexeme.h
 * @brief Lexem-related stuff.
 *
 * Contains the lexem structure definition and the associated prototypes.
 */

#ifndef LEXEME_H
#define LEXEME_H

#include "ListGeneric.h"
#include "Utils.h"
#include "StateMachine.h"

typedef struct
{
    char *type;
    unsigned long int lineNumber;
    void *value;
    STATE state;
} LEXEME;

/**
 * @brief String equivalent to state machine's states.
 *
 */
extern char *definedType[15];

/**
 * @param state Integer which is defined by a enumerator by the state machine.
 * @brief This function creates the lexeme type thanks to the equivalent state.
 *
 */
char *CreateType(STATE state);

/**
 * @param list of char, integer or one string.
 * @brief This function concatenate the list to send a (void*) pointer of the value.
 *
 */
void *CreateValue(LIST list);

/**
 * @param list of char, integer or one string.
 * @param state Integer which is defined by a enumerator by the state machine.
 * @param line Unsigned long int which define the lineNumber of this lexeme.
 * @brief This function sends a Lexeme with the information sent.
 *
 */
LEXEME CreateLexeme(STATE state, LIST list, unsigned long int line);

/**
 * @param lexeme pointeur of the lexeme where you want to delete the value.
 * @brief This function erases the value of the lexeme.
 *
 */
void ErasedValueLexeme(void* lexeme);

/**
 * @param value Void* represent the pointer on a Lexeme.
 * @brief This function aim to show the lexeme sent properly.
 *
 */
void DisplayLexeme(void *value);

#endif
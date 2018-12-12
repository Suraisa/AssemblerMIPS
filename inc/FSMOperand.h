#ifndef FSM_OPERAND_H
#define FSM_OPERAND_H

#include <stdio.h>
#include <limits.h>
#include "DoubleListGeneric.h"
#include "Section.h"
#include "Lexeme.h"

#define RELATIVE_MAX 131072
#define RELATIVE_MIN (-RELATIVE_MAX-1)

#define ABSOLUTE_MAX 134217728
#define ABSOLUTE_MIN (-ABSOLUTE_MAX-1)

typedef enum
{
    ABSOLUTE = 'A',
    REG = '$',
    BASE_OFFSET = 'B',
    IMMEDIAT = 'I',
    SHIFT_AMOUNT = 'S',
    RELATIVE = 'R'
}OPERAND_TYPE;

typedef struct
{
    int error;
    OPERAND_TYPE type;
} FSM_STATE_OPERAND;

/**
 * @param fsm FSM_STATE_OPERAND* which is the fsm to initialize. 
 * @param operandType OPERAND_TYPE which is the type that we expected for the fsm.
 * @brief Initialize the operand fsm.
 *
 */
void InitializationOperandFsm(FSM_STATE_OPERAND* fsm, OPERAND_TYPE operandType);

/**
 * @param listLexeme LIST_DOUBLE* which is the list to test.
 * @brief Test if the lexemes' list is an immediat operand.
 *
 */
int IsImmediat(LIST_DOUBLE* listLexeme);

/**
 * @param listLexeme LIST_DOUBLE* which is the list to test.
 * @brief Test if the lexemes' list is an absolute operand.
 *
 */
int IsAbsolute(LIST_DOUBLE* listLexeme);

/**
 * @param listLexeme LIST_DOUBLE* which is the list to test.
 * @brief Test if the lexemes' list is a relative operand.
 *
 */
int IsRelative(LIST_DOUBLE* listLexeme);

/**
 * @param listLexeme LIST_DOUBLE* which is the list to test.
 * @brief Test if the lexemes' list is a baseOffset operand.
 *
 */
int IsBaseOffset(LIST_DOUBLE* listLexeme);

/**
 * @param listLexeme LIST_DOUBLE* which is the list to test.
 * @brief Test if the lexemes' list is a shiftAmount operand.
 *
 */
int IsShiftAmount(LIST_DOUBLE* listLexeme);

/**
 * @param fsm FSM_STATE_OPERAND* fsm operand to use.
 * @param listLexeme LIST_DOUBLE* which is the list to test.
 * @brief Test if the operand is the one defined in the fsm.
 *
 */
void OperandFSM(FSM_STATE_OPERAND* fsm, LIST_DOUBLE* listOperand);

void CreateNewBaseOffset(QUEUE_DOUBLE* lexemeQueue);

#endif
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

void InitializationOperandFsm(FSM_STATE_OPERAND* fsm, OPERAND_TYPE operandType);

int IsImmediat(LIST_DOUBLE* listLexeme);

int IsAbsolute(LIST_DOUBLE* listLexeme);

int IsRelative(LIST_DOUBLE* listLexeme);

int IsBaseOffset(LIST_DOUBLE* listLexeme);

int IsShiftAmount(LIST_DOUBLE* listLexeme);

void OperandFSM(FSM_STATE_OPERAND* fsm, LIST_DOUBLE* listOperand);

#endif
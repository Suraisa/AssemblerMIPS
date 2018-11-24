#include "FSMOperand.h"

void InitializationOperandFsm(FSM_STATE_OPERAND* fsm, OPERAND_TYPE operandType)
{
    fsm->error = 0;
    fsm->type = operandType;
}

void OperandFSM(FSM_STATE_OPERAND* fsm, LIST_DOUBLE* listOperand)
{
    switch(fsm->type)
    {
        case REG:
        {
            if ((*listOperand)->next != (*listOperand) && ((LEXEME *)(*listOperand)->data)->state == REGISTER)
            {
                fsm->error = 1;
                return;
            }
            fsm->type = 0;
            break;
        }
        case IMMEDIAT:
        {
            fsm->error = !IsImmediat(listOperand);
            break;
        }
        case BASE_OFFSET:
        {
            fsm->error = !IsBaseOffset(listOperand);
            break;
        }
        case ABSOLUTE:
        {
            fsm->error = !IsRelativeAbsolute(listOperand, ABSOLUTE_MAX, ABSOLUTE_MIN);
            break;
        }
        case RELATIVE:
        {
            fsm->error = !IsRelativeAbsolute(listOperand, RELATIVE_MAX, RELATIVE_MIN);
            break;
        }
    }
}

int IsImmediat(LIST_DOUBLE* listLexeme)
{
    LIST_DOUBLE firstNode = *listLexeme;
    LIST_DOUBLE listOperand = *listLexeme;
    int sign = 1;
    int IsImmediat = 0;
    if(SizeListDouble(listOperand) > 2)
        return 0;

    do
    {
        LEXEME lexeme = *((LEXEME *)listOperand->data);
        switch (lexeme.state)
        {
            default:
                return 0;

            case OPERATOR:
            {
                if(((LEXEME *)listOperand->prev->data)->state == OPERATOR)
                    return 0;

                if(*(char*)lexeme.value == '-')
                {
                    sign = -1;
                }
            }
            case HEXADECIMAL:
            case DECIMAL:
            {
                if(sign == 1)
                {
                    if(*(int*)lexeme.value > SHRT_MAX)
                        return 0;
                }
                else if(sign == -1)
                {
                    if(*(int*)lexeme.value > SCHAR_MIN)
                        return 0;
                }
                *(int*)((LEXEME *)listOperand->prev->data)->value *= sign;
                IsImmediat = 1;
                break;
            }
        }
        listOperand = listOperand->next;
    }while (firstNode != listOperand);
    
    return IsImmediat;
}

int IsShiftAmount(LIST_DOUBLE* listLexeme)
{
    LEXEME lexeme = *((LEXEME *)(*listLexeme)->data);
    if(SizeListDouble(*listLexeme) > 1)
        return 0;
    
    if(lexeme.state != HEXADECIMAL || lexeme.state != DECIMAL)
        return 0;

    if(*(int*)lexeme.value > 31)
        return 0;

    return 1;
}

int IsRelativeAbsolute(LIST_DOUBLE* listLexeme, int valueMax, int valueMin)
{
    LIST_DOUBLE firstNode = *listLexeme;
    LIST_DOUBLE listOperand = *listLexeme;
    int sign = 1;
    int IsImmediat = 0;
    if(SizeListDouble(listOperand) > 2)
        return 0;

    do
    {
        LEXEME lexeme = *((LEXEME *)listOperand->data);
        switch (lexeme.state)
        {
            default:
                return 0;

            case OPERATOR:
            {
                if(((LEXEME *)listOperand->prev->data)->state == OPERATOR)
                    return 0;

                if(*(char*)lexeme.value == '-')
                {
                    sign = -1;
                }
            }
            case HEXADECIMAL:
            case DECIMAL:
            {
                if(sign == 1)
                {
                    if(*(int*)lexeme.value > valueMax)
                        return 0;
                }
                else if(sign == -1)
                {
                    if(*(int*)lexeme.value > -valueMin)
                        return 0;
                }
                IsImmediat = (*(int*)lexeme.value)%4 == 0 ? 1 : 0;
                break;
            }
        }
        listOperand = listOperand->next;
    }while (firstNode != listOperand);
    *(int*)((LEXEME *)listOperand->prev->data)->value *= sign;
    *(int*)((LEXEME *)listOperand->prev->data)->value %= 4;
    return IsImmediat;
}

int IsBaseOffset(LIST_DOUBLE* listLexeme)
{
    LIST_DOUBLE firstNode = *listLexeme;
    LIST_DOUBLE listOperand = *listLexeme;
    int sign = 1;
    int IsImmediat = 0;
    if(SizeListDouble(listOperand) > 2)
        return 0;

    do
    {
        LEXEME lexeme = *((LEXEME *)listOperand->data);
        switch (lexeme.state)
        {
            default:
                return 0;

            case OPERATOR:
            {
                if(((LEXEME *)listOperand->prev->data)->state != PARENTHESISRIGHT)
                    return 0;

                if(*(char*)lexeme.value == '-')
                {
                    sign = -1;
                }
            }
            case HEXADECIMAL:
            case DECIMAL:
            {
                if(((LEXEME *)listOperand->prev->data)->state != PARENTHESISRIGHT || ((LEXEME *)listOperand->prev->data)->state != OPERATOR)
                    return 0;
                
                if(sign == 1)
                {
                    if(*(int*)lexeme.value > SHRT_MAX)
                        return 0;
                }
                else if(sign == -1)
                {
                    if(*(int*)lexeme.value > -SHRT_MIN)
                        return 0;
                }
                IsImmediat = (*(int*)lexeme.value)%4 == 0 ? 1 : 0;
                break;
            }
            case PARENTHESISLEFT:
            {
                if (((LEXEME *)listOperand->prev->data)->state != HEXADECIMAL || ((LEXEME *)listOperand->prev->data)->state != DECIMAL)
                    return 0;
                break;
            }
            case REGISTER:
            {
                if(((LEXEME *)listOperand->prev->data)->state != PARENTHESISLEFT)
                    return 0;
                break;
            }
            case PARENTHESISRIGHT:
            {
                if(((LEXEME *)listOperand->prev->data)->state != REGISTER)
                    return 0;
                break;
            }
        }
        listOperand = listOperand->next;
    }while (firstNode != listOperand);
    *(int*)((LEXEME *)listOperand->prev->data)->value *= sign;
    return IsImmediat;
}
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
            if ((*listOperand)->next != (*listOperand) || ((LEXEME *)(*listOperand)->data)->state != REGISTER)
            {
                fsm->error = 1;
                break;
            }
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
            fsm->error = !IsAbsolute(listOperand);
            break;
        }
        case RELATIVE:
        {
            fsm->error = !IsRelative(listOperand);
            break;
        }
        case SHIFT_AMOUNT:
        {
            fsm->error = !IsShiftAmount(listOperand);
            break;
        }
        default:
        {
            fsm->error = 1;
            break;
        }
    }
}

int IsImmediat(LIST_DOUBLE* listLexeme)
{
    LIST_DOUBLE firstNode = *listLexeme;
    LIST_DOUBLE listOperand = *listLexeme;
    LEXEME lexeme;
    int sign = 1;
    int isCorrect = 0;
    int findSign = 0;
    if(SizeListDouble(listOperand) > 2)
        return 0;

    do
    {
        if(((LEXEME *)listOperand->prev->data)->state == OPERATOR)
        {
            firstNode = listOperand;
            ErasedInFrontDouble(listLexeme);
        }
        lexeme = *((LEXEME *)listOperand->data);
        switch (lexeme.state)
        {
            default:
                return 0;

            case SYMBOL:
            {
                if (findSign)
                    return 0;

                if((*listLexeme)->next != *listLexeme)    
                    return 0;
                
                isCorrect = 1;
                break;
            }
            case OPERATOR:
            {
                if(lexeme.state == ((LEXEME *)listOperand->next->data)->state)
                    return 0;

                if(*(char*)lexeme.value == '-')
                {
                    sign = -1;
                }
                findSign = 1;
                break;
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
                    if(*(int*)lexeme.value > -SHRT_MIN)
                        return 0;
                }
                isCorrect = 1;
                break;
            }
        }
        listOperand = listOperand->next;
    }while (firstNode != listOperand);
    if(lexeme.state != SYMBOL)
    {
        *(long int*)((LEXEME *)(*listLexeme)->data)->value *= sign;
    }
    return isCorrect;
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

int IsAbsolute(LIST_DOUBLE* listLexeme)
{
    LIST_DOUBLE firstNode = *listLexeme;
    LIST_DOUBLE listOperand = *listLexeme;
    LEXEME lexeme;
    int sign = 1;
    int isCorrect = 0;
    int findSign = 0;
    if(SizeListDouble(listOperand) > 2)
        return 0;

    do
    {
        if(((LEXEME *)listOperand->prev->data)->state == OPERATOR)
        {
            firstNode = listOperand;
            ErasedInFrontDouble(listLexeme);
        }
        lexeme = *((LEXEME *)listOperand->data);
        switch (lexeme.state)
        {
            default:
                return 0;

            case SYMBOL:
            {
                if (SizeListDouble(listOperand)>1)
                    return 0;

                if(findSign)
                    return 0;

                isCorrect = 1;
                break;
            }
            case OPERATOR:
            {
                if(lexeme.state == ((LEXEME *)listOperand->next->data)->state)
                    return 0;

                if(*(char*)lexeme.value == '-')
                {
                    sign = -1;
                }
                findSign = 1;
                break;
            }
            case HEXADECIMAL:
            case DECIMAL:
            {
                if(sign == 1)
                {
                    if(*(int*)lexeme.value > RELATIVE_MAX)
                        return 0;
                }
                else if(sign == -1)
                {
                    if(*(int*)lexeme.value > -RELATIVE_MIN)
                        return 0;
                }
                isCorrect = (*(int*)lexeme.value)%4 == 0 ? 1 : 0;
                break;
            }
        }
        listOperand = listOperand->next;
    }while (firstNode != listOperand);
    if(lexeme.state != SYMBOL)
    {
        *(long int*)((LEXEME *)(*listLexeme)->data)->value *= sign;
        *(long int*)((LEXEME *)(*listLexeme)->data)->value >>= 2;
    }
    return isCorrect;
}

int IsRelative(LIST_DOUBLE* listLexeme)
{
    LIST_DOUBLE firstNode = *listLexeme;
    LIST_DOUBLE listOperand = *listLexeme;
    LEXEME lexeme;
    int sign = 1;
    int findSign = 0;
    int isCorrect = 0;
    if(SizeListDouble(listOperand) > 2)
        return 0;

    do
    {
        if(((LEXEME *)listOperand->prev->data)->state == OPERATOR)
        {
            firstNode = listOperand;
            ErasedInFrontDouble(listLexeme);
        }
        lexeme = *((LEXEME *)listOperand->data);
        switch (lexeme.state)
        {
            default:
                return 0;

            case SYMBOL:
            {
                if(firstNode != listOperand)
                    return 0;

                if(findSign)
                    return 0;
                
                isCorrect = 1;
                break;
            }
            case OPERATOR:
            {
                if(lexeme.state == ((LEXEME *)listOperand->next->data)->state)
                    return 0;

                if(*(char*)lexeme.value == '-')
                {
                    sign = -1;
                }
                findSign = 1;
                break;
            }
            case HEXADECIMAL:
            case DECIMAL:
            {
                if(sign == 1)
                {
                    if(*(int*)lexeme.value > RELATIVE_MAX)
                        return 0;
                }
                else if(sign == -1)
                {
                    if(*(int*)lexeme.value > -RELATIVE_MIN)
                        return 0;
                }
                isCorrect = (*(int*)lexeme.value)%4 == 0 ? 1 : 0;
                break;
            }
        }
        listOperand = listOperand->next;
    }while (firstNode != listOperand);
    
    if(lexeme.state!= SYMBOL)
    {
        *(long int*)((LEXEME *)(*listLexeme)->data)->value *= sign;
        *(long int*)((LEXEME *)(*listLexeme)->data)->value >>= 2;
    }
    return isCorrect;
}

int IsBaseOffset(LIST_DOUBLE* listLexeme)
{
    LIST_DOUBLE firstNode = *listLexeme;
    LIST_DOUBLE listOperand = *listLexeme;
    int findSign = 0;
    int sign = 1;
    int isCorrect = 0;

    if(SizeListDouble(listOperand) > 5)
        return 0;

    do
    {
        if(findSign && ((LEXEME *)listOperand->prev->data)->state == OPERATOR)
        {
            firstNode = listOperand;
            ErasedInFrontDouble(listLexeme);
        }
        LEXEME lexeme = *((LEXEME *)listOperand->data);
        switch (lexeme.state)
        {
            default:
                return 0;

            case SYMBOL:
            {
                if(firstNode != listOperand)
                    return 0;

                if(findSign)
                    return 0;

                break;
            }
            case OPERATOR:
            {
                if(findSign)
                    return 0;

                if(((LEXEME *)listOperand->prev->data)->state != PARENTHESISRIGHT)
                    return 0;

                if(*(char*)lexeme.value == '-')
                {
                    sign = -1;
                }
                findSign = 1;
                break;
            }
            case HEXADECIMAL:
            case DECIMAL:
            {
                if(((LEXEME *)listOperand->prev->data)->state != PARENTHESISRIGHT && !findSign)
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
                break;
            }
            case PARENTHESISLEFT:
            {
                if (((LEXEME *)listOperand->prev->data)->state != HEXADECIMAL && ((LEXEME *)listOperand->prev->data)->state != DECIMAL && ((LEXEME *)listOperand->prev->data)->state != SYMBOL)
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
                
                isCorrect = 1;
                break;
            }
        }
        listOperand = listOperand->next;
    }while (firstNode != listOperand);
    *(long int*)((LEXEME *)(*listLexeme)->data)->value *= sign;
    return isCorrect;
}
#include "FSMLexeme.h"

void InitLexemeFsm(char readingChar, LEXEME_FSM *stateMachine, unsigned long int lineNumber)
{
    switch (readingChar)
    {
        case ' ':
        case '\t':
            break;
        case '"':
        {
            stateMachine->currentState = STRING;
        }
        break;
        case '#':
        {
            stateMachine->currentState = COMMENT;
        }
        break;
        case '.':
        {
            stateMachine->currentState = DIRECTIVE;
        }
        break;
        case '$':
        {
            stateMachine->currentState = REGISTER;
        }
        break;
        case ',':
        {
            stateMachine->currentState = COMMA;
        }
        break;
        case '(':
        {
            stateMachine->currentState = PARENTHESISLEFT;
        }
        break;
        case ')':
        {
            stateMachine->currentState = PARENTHESISRIGHT;
        }
        break;
        case '\n':
        {
            stateMachine->currentState = RETURN;
        }
        break;
        case ':':
        {
            stateMachine->currentState = COLON;
        }
        break;
        case '-':
        case '+':
        {
            stateMachine->currentState = OPERATOR;
        }
        break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            stateMachine->currentState = DECIMAL;
        }
        break;
        default:
        {
            if (CharIsNumberLetter(readingChar))
            {
                stateMachine->currentState = SYMBOL;
            }
            else
            {
                PrintError(stateMachine, lineNumber, "Invalid symbol", readingChar, "");
                break;
            }
        }
        break;
    }
}

void LexemeFsm(char *readingChar, QUEUE_DOUBLE *lexemeQueue, LIST_DOUBLE *readingValue, LEXEME_FSM *stateMachine, unsigned long int *lineNumber, int finishedFile)
{
    if (finishedFile && IsEmptyDouble(*readingValue))
        return;

    if (finishedFile)
    {
        if(stateMachine->currentState == HEXADECIMAL)
        {
            HexadecimalTreatment(lexemeQueue, stateMachine, readingValue, *lineNumber);            
        }
        LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, *lineNumber);
        return;
    }

    switch (stateMachine->currentState)
    {
    case INIT:
    {
        InitLexemeFsm(*readingChar, stateMachine, *lineNumber);
        if (stateMachine->currentState != INIT)
        {
            LexemeFsm(readingChar, lexemeQueue, readingValue, stateMachine, lineNumber, 0);
            break;
        }
        break;
    }
    case STRING:
    {
        if (*readingChar == '\n')
        {
            PrintError(stateMachine, *lineNumber, "Return in", '\0', definedType[stateMachine->currentState]);
            (*lineNumber)++;
        }
        else if (*readingChar == '"' && !stateMachine->inState)
        {
            stateMachine->inState = !stateMachine->inState;
        }
        else if (*readingChar == '"' && stateMachine->inState)
        {
            if(*(char*)(*readingValue)->data != 0x5c)
            {
                stateMachine->inState = !stateMachine->inState;
            }
            else
            {
                ErasedInFrontDouble(readingValue);
                AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
            }
        }
        else
        {
            AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
        }
        if (!stateMachine->inState)
        {
            LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, *lineNumber);
            stateMachine->currentState = INIT;
        }
        break;
    }
    case COMMENT:
    {
        if (*readingChar != '\n')
            AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
        else
        {
            LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, *lineNumber);
            stateMachine->currentState = INIT;
            LexemeFsm(readingChar, lexemeQueue, readingValue, stateMachine, lineNumber, 0);
        }
        break;
    }
    case OPERATOR:
    case COMMA:
    case PARENTHESISLEFT:
    case COLON:
    case PARENTHESISRIGHT:
    {
        if(!IsEmptyDouble(*lexemeQueue) && (((LEXEME*)((*lexemeQueue)->prev)->data)->state) == stateMachine->currentState)
        {
            PrintError(stateMachine, *lineNumber, "Tow times same punctuation", '\0', definedType[stateMachine->currentState]);
        }
        else
        {
            AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
            LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, *lineNumber);
        }
        stateMachine->currentState = INIT;
        break;
    }
    case REGISTER:
    {
        if (*readingChar == '$' && !stateMachine->inState)
        {
            stateMachine->inState = !stateMachine->inState;
            AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
        }
        else if (CharIsLowerLetter(*readingChar) || CharIsNumber(*readingChar))
        {
            AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
        }
        else
        {
            if (*readingChar == '\n' || *readingChar == '\t' || *readingChar == ' ' || *readingChar == ',' || *readingChar == ')')
            {
                LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, *lineNumber);
                stateMachine->currentState = INIT;
                stateMachine->inState = !stateMachine->inState;
                LexemeFsm(readingChar, lexemeQueue, readingValue, stateMachine, lineNumber, 0);
            }
            else
            {
                PrintError(stateMachine, *lineNumber, "Invalid symbol", *readingChar, definedType[stateMachine->currentState]);
                break;
            }
        }
        break;
    }
    case DIRECTIVE:
    {
        if (*readingChar == '.' && !stateMachine->inState)
        {
            stateMachine->inState = !stateMachine->inState;
            AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
        }
        else if (CharIsLetter(*readingChar))
        {
            AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
        }
        else
        {
            if (*readingChar == '\n' || *readingChar == '\t' || *readingChar == ' ')
            {
                LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, *lineNumber);
                stateMachine->currentState = INIT;
                stateMachine->inState = !stateMachine->inState;
                LexemeFsm(readingChar, lexemeQueue, readingValue, stateMachine, lineNumber, 0);
            }
            else
            {
                PrintError(stateMachine, *lineNumber, "Invalid symbol", *readingChar, definedType[stateMachine->currentState]);
                break;
            }
        }

        break;
    }
    case RETURN:
    {
        char *value = "New Line";
        AddInFrontDouble(readingValue, &value, &DisplayString, NULL, sizeof(char *));
        LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, *lineNumber);
        (*lineNumber)++;
        stateMachine->currentState = INIT;
        break;
    }
    case DECIMAL:
    {
        if(!IsEmptyDouble(*readingValue) && (*readingChar == ' ' || *readingChar == '\t' || *readingChar == '\n' || *readingChar == ',' || *readingChar == '('))
        {
            LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, *lineNumber);
            stateMachine->currentState = INIT;
            LexemeFsm(readingChar, lexemeQueue, readingValue, stateMachine, lineNumber, 0);
            break;
        }   
        if (SizeListDouble(*readingValue) == 1)
        {
            if (*(int *)((*readingValue)->data) == 0 && (*readingChar == 0x58 || *readingChar == 0x78))
            {
                ErasedListDouble(readingValue);
                stateMachine->currentState = HEXADECIMAL;
                break;
            }
        }
        if ((0x2F < *readingChar && *readingChar < 0x3A))
        {
            int* readingInt = malloc(sizeof(int));

            if (readingInt == NULL)
                return;

            *readingInt = (int)strtol(readingChar, NULL, 10);
            AddInFrontDouble(readingValue, readingInt, &DisplayInt, NULL, sizeof(int));
            free(readingInt);
        }
        else
        {
            PrintError(stateMachine, *lineNumber, "Invalid symbol", *readingChar, definedType[stateMachine->currentState]);
        }
        break;
    }
    case HEXADECIMAL:
    {
        if(!IsEmptyDouble(*readingValue) && (*readingChar == ' ' || *readingChar == '\t' || *readingChar == '\n' || *readingChar == ',' || *readingChar == '('))
        {
            HexadecimalTreatment(lexemeQueue, stateMachine, readingValue, *lineNumber);
            stateMachine->currentState = INIT;
            LexemeFsm(readingChar, lexemeQueue, readingValue, stateMachine, lineNumber, 0);
            break;
        }
        if (CharIsHexadecimal(*readingChar))
        {
            AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
        }
        else
        {
            PrintError(stateMachine, *lineNumber, "Invalid symbol", *readingChar, definedType[stateMachine->currentState]);
        }
        break;
    }
    default:
    {
        if (CharIsNumberLetter(*readingChar))
        {
            AddInFrontDouble(readingValue, readingChar, &DisplayChar, NULL, sizeof(char));
        }
        else
        {
            if (*readingChar == '\n' || *readingChar == '\t' || *readingChar == ' ' || *readingChar == ':')
            {
                LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, *lineNumber);
                stateMachine->currentState = INIT;
                LexemeFsm(readingChar, lexemeQueue, readingValue, stateMachine, lineNumber, 0);
            }
            else
            {
                PrintError(stateMachine, *lineNumber, "Invalid symbol", *readingChar, "\0");
                break;
            }
        }
        break;
    }
    }
}

void HexadecimalTreatment(QUEUE_DOUBLE* lexemeQueue, LEXEME_FSM *stateMachine, LIST_DOUBLE* readingValue, unsigned long int lineNumber)
{
    char *stringValue = ConcatenateCharListDouble(*readingValue);
    ErasedListDouble(readingValue);
    long int* value = malloc(sizeof(*value));
    *value = StringHexToDecimal(stringValue);
    free (stringValue);
    AddInFrontDouble(readingValue, value, &DisplayInt, NULL, sizeof(long int));
    LexemeTreatment(lexemeQueue, stateMachine->currentState, readingValue, lineNumber);
    free(value);
}
void LexemeTreatment(QUEUE_DOUBLE* lexemeQueue, LEXEME_STATE state, LIST_DOUBLE* readingValue, unsigned long int lineNumber)
{
    LEXEME lexemeToAdd = CreateLexeme(state, *readingValue, lineNumber);
    PushQueueDouble(lexemeQueue, &lexemeToAdd, DisplayLexeme, ErasedValueLexeme, sizeof(LEXEME));
    ErasedListDouble(readingValue);
}

void PrintError(LEXEME_FSM *stateMachine, unsigned long int lineNumber, char* problem, char wrongValue, char* state)
{
    if (wrongValue != '\0')
    {
        printf("\x1b[31m" "\nERROR" "\x1b[0m" " in line" "\x1b[36m" " %lu " "\x1b[0m" ":\n""%s " "'%c'" "\x1b[35m" " %s" "\x1b[0m" "\n", lineNumber, problem, wrongValue, state);
    }
    else
    {
        printf("\x1b[31m" "\nERROR" "\x1b[0m" " in line" "\x1b[36m" " %lu " "\x1b[0m" ":\n""%s" "\x1b[35m" " %s" "\x1b[0m" "\n", lineNumber, problem, state);
    }
    stateMachine->error = 1;
}

void InitializationLexemeFsm(LEXEME_FSM* stateMachine)
{
    stateMachine->currentState = INIT;
    stateMachine->error = 0;
    stateMachine->inState = 0;
}
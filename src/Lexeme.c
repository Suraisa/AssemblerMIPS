#include "Lexeme.h"

char *definedType[15] =
    {"PARENTHESISLEFT",
     "PARENTHESISRIGHT",
     "COMMA",
     "REGISTER",
     "DIRECTIVE",
     "COMMENT",
     "NUMBER",
     "HEXADECIMAL",
     "DECIMAL",
     "STRING",
     "SYMBOL",
     "RETURN",
     "COLON",
     "OPERATOR",
     "INIT"};

char *CreateType(STATE state)
{
    return definedType[state];
}

void *CreateValue(LIST list)
{
    if (IsEmpty(list))
        return NULL;

    if (list->display == &DisplayInt)
    {
        long int *value = malloc(sizeof(long int));

        if (value == NULL)
            return NULL;

        *value = ConcatenateIntList(list);
        return value;
    }
    else if (list->display == &DisplayString)
    {
        char **value = malloc(sizeof(char *));

        if (value == NULL)
            return NULL;

        *value = *(char **)list->data;
        return value;
    }
    else
    {
        char *value = ConcatenateCharList(list);
        return value;
    }
}

LEXEME CreateLexeme(STATE state, LIST list, unsigned long int lineNumber)
{
    LEXEME lexeme;
    lexeme.type = CreateType(state);
    lexeme.lineNumber = lineNumber;
    lexeme.value = CreateValue(list);
    lexeme.state = state;
    return lexeme;
}

void ErasedValueLexeme(void* lexeme)
{
    free(((LEXEME*)lexeme)->value);
}

void DisplayLexeme(void *value)
{
    char *separator = "--------------------------------";
    printf("\n");
    printf("%s\n", separator);
    printf("Type: %26s\n", ((LEXEME *)value)->type);
    printf("Line number: %19lu\n", (unsigned long int)((LEXEME *)value)->lineNumber);

    if (((LEXEME *)value)->state == DECIMAL)
    {
        printf("Value: %25ld", *(long int *)((LEXEME *)value)->value);  
    }
    else if (((LEXEME *)value)->state == RETURN)
    {
        printf("Value: %25s", *(char **)((LEXEME *)value)->value);
    }
    else
    {
        printf("Value: %25s", (char *)((LEXEME *)value)->value);
    }

    printf("\n%s", separator);
    printf("\n");
}
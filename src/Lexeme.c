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

char *CreateType(LEXEME_STATE state)
{
    return definedType[state];
}

void *CreateValue(LIST_DOUBLE list)
{
    if (IsEmptyDouble(list))
        return NULL;

    if (list->display == &DisplayInt)
    {
        long int *value = malloc(sizeof(long int));

        if (value == NULL)
            return NULL;

        *value = ConcatenateIntListDouble(list);
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
        char *value = ConcatenateCharListDouble(list);
        return value;
    }
}

LEXEME CreateLexeme(LEXEME_STATE state, LIST_DOUBLE list, unsigned long int lineNumber)
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

    if (((LEXEME *)value)->state == DECIMAL || ((LEXEME *)value)->state == HEXADECIMAL || ((LEXEME *)value)->state == REGISTER)
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

LIST_DOUBLE CopyListLexeme(LIST_DOUBLE* list)
{
    if (IsEmptyDouble(*list))
        return NULL;

    LIST_DOUBLE firstNode = *list;
    LIST_DOUBLE slider = firstNode;
    LIST_DOUBLE copy = CreateListDouble();
    LEXEME value;
    do
    {
        value.lineNumber = ((LEXEME *)slider->data)->lineNumber;
        value.state = ((LEXEME *)slider->data)->state;
        value.type = ((LEXEME *)slider->data)->type;
        if(((LEXEME *)slider->data)->state == DECIMAL || ((LEXEME *)slider->data)->state == HEXADECIMAL || ((LEXEME *)slider->data)->state == REGISTER)
        {
            value.value = malloc(sizeof(long int));
            memmove(value.value, ((LEXEME*)slider->data)->value, sizeof(long int));
        }
        else if(((LEXEME *)slider->data)->state != RETURN)
        {
            value.value = malloc(strlen(((LEXEME *)slider->data)->value)+1);
            memmove(value.value, ((LEXEME*)slider->data)->value, strlen(((LEXEME *)slider->data)->value)+1);
        }
        else
        {
            value.value = malloc(1);
            memmove(value.value, ((LEXEME*)slider->data)->value, 1);
        }
        AddAtLastDouble(&copy, &value, slider->display, slider->erasedDataValue, sizeof(LEXEME));

        slider = slider->next;
    }while(slider != firstNode);
    return copy;
}
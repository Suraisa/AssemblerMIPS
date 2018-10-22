#include <stdio.h>

#include "ListGeneric.h"
#include "DisplayType.h"
#include "Lexeme.h"
#include "FSMLexeme.h"


int main()
{
    LIST list = CreateList();

    char b = 'a';

    AddInFront(&list, &b, &DisplayChar, NULL, sizeof(char));
    b = 'b';
    AddInFront(&list, &b, &DisplayChar, NULL, sizeof(char));
    b = 'c';
    AddInFront(&list, &b, &DisplayChar, NULL, sizeof(char));
    
    LEXEME lexeme = CreateLexeme(STRING, list, 5);

    ErasedList(&list);

    AddInFront(&list, &lexeme, &DisplayLexeme, ErasedValueLexeme, sizeof(LEXEME));
    AddInFront(&list, &lexeme, &DisplayLexeme, ErasedValueLexeme, sizeof(LEXEME));
    AddInFront(&list, &lexeme, &DisplayLexeme, ErasedValueLexeme, sizeof(LEXEME));

    Display(list);

    ErasedList(&list);

    return 0;
}
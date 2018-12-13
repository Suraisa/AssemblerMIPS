#include <stdio.h>

#include "DoubleListGeneric.h"
#include "DisplayType.h"
#include "Utils.h"

int main()
{
    LIST_DOUBLE list = CreateListDouble();

    int a = 7;

    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 6;
    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 3;
    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 2;
    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));

    printf("\nConcatenation of int: %ld\n",ConcatenateIntListDouble(list));

    ErasedListDouble(&list);


    char b = 'a';

    AddInFrontDouble(&list, &b, &DisplayChar, NULL, sizeof(char));
    b = 'b';
    AddInFrontDouble(&list, &b, &DisplayChar, NULL, sizeof(char));
    b = 'c';
    AddInFrontDouble(&list, &b, &DisplayChar, NULL, sizeof(char));

    printf("\nConcatenation of char: %s\nSize of the string: %d",ConcatenateCharListDouble(list), StringSize(ConcatenateCharListDouble(list)));

    ErasedListDouble(&list);

    char* value = malloc(3*sizeof(*value));
    int* intValue;
    strcpy(value, "33");
    printf("\n%s\n", value);
    a = IsAvailableRegister(value, &intValue);
    printf("\n%d\n", *intValue);

    free(value);
    free(intValue);

    char* string = malloc(10);
    strcpy(string,"TheString");

    ReplaceString(&string, "test", 5, 5);
    printf("\n%s\n", string);

    free(string);

    return 0;
}
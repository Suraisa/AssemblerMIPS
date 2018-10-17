#include <stdio.h>

#include "ListGeneric.h"
#include "DisplayType.h"
#include "Utils.h"

int main()
{
    LIST list = CreateList();

    int a = 7;

    AddInFront(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 6;
    AddInFront(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 3;
    AddInFront(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 2;
    AddInFront(&list, &a, &DisplayInt, NULL, sizeof(long int));

    printf("\nConcatenation of int: %ld\n",ConcatenateIntList(list));

    ErasedList(&list);


    char b = 'a';

    AddInFront(&list, &b, &DisplayChar, NULL, sizeof(char));
    b = 'b';
    AddInFront(&list, &b, &DisplayChar, NULL, sizeof(char));
    b = 'c';
    AddInFront(&list, &b, &DisplayChar, NULL, sizeof(char));

    printf("\nConcatanation of char: %s\nSize of the string: %d",ConcatenateCharList(list), StringSize(ConcatenateCharList(list)));

    ErasedList(&list);

    return 0;
}
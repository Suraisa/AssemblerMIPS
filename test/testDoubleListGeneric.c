#include <stdio.h>

#include "DoubleListGeneric.h"
#include "DisplayType.h"
#include "Lexeme.h"
#include "Section.h"
#include "DicoInstruct.h"

int main()
{
    LIST_DOUBLE list = CreateListDouble();
    LIST_DOUBLE list1 = CreateListDouble();
    long int a = 0;
    float b = 5.8;
    double c = 8.2;
    char d = 'x';
    char *e = "test";

    AddAtLastDouble(&list, &a, &DisplayInt, NULL, sizeof(int));
    AddAtLastDouble(&list, &b, &DisplayFloat, NULL, sizeof(float));
    AddAtLastDouble(&list, &c, &DisplayDouble, NULL, sizeof(double));
    AddAtLastDouble(&list, &d, &DisplayChar, NULL, sizeof(char));
    AddAtLastDouble(&list, &e, &DisplayString, NULL, sizeof(char*));

    IsEmptyDouble(list) ? printf("\nThe list is empty.\n") : printf("\nThe list is not empty.\n");

    DisplayDoubleList(list);
    ErasedListDouble(&list);
    DisplayDoubleList(list);

    IsEmptyDouble(list) ? printf("\nThe list is empty.\n") : printf("\nThe list is not empty.\n");

    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 6;
    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 3;
    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 1;
    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));

    DisplayDoubleList(list);

    LIST_DOUBLE popedList = PopInFrontDouble(&list, 2);

    printf("\nPoped list :\n");

    DisplayDoubleList(popedList);

    ErasedListDouble(&popedList);

    popedList = PopInFrontDouble(&list, 1);

    printf("\nPoped list :\n");

    DisplayDoubleList(popedList);

    ErasedListDouble(&popedList);

    popedList = PopInFrontDouble(&list, 1);

    printf("\nPoped list :\n");

    DisplayDoubleList(popedList);

    ErasedListDouble(&popedList);

    printf("\nList :\n");

    DisplayDoubleList(list);

    ErasedListDouble(&list);
    ErasedListDouble(&popedList);

    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 6;
    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 3;
    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 2;
    AddInFrontDouble(&list, &a, &DisplayInt, NULL, sizeof(long int));

    ErasedAtLastDouble(&list);
    ErasedAtLastDouble(&list);

    printf("\nList with 2 elements:\n");

    DisplayDoubleList(list);

    a = 6;
    AddInFrontDouble(&list1, &a, &DisplayInt, NULL, sizeof(long int));
    AddInFrontDouble(&list1, &a, &DisplayInt, NULL, sizeof(long int));

    ErasedListDouble(&list);

    printf("\nList erased:\n");

    DisplayDoubleList(list);

    printf("\nList copied:\n");

    ErasedListDouble(&list1);

    AddInFrontDouble(&list1, &a, &DisplayInt, NULL, sizeof(long int));

    DisplayDoubleList(list1);

    ErasedListDouble(&list1);

    return 0;
}

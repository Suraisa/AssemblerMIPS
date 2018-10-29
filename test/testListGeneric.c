#include <stdio.h>

#include "ListGeneric.h"
#include "DisplayType.h"

int main()
{
    LIST list = CreateList();
    long int a = 0;
    float b = 5.8;
    double c = 8.2;
    char d = 'x';
    char *e = "test";

    AddAtLast(&list, &a, &DisplayInt, NULL, sizeof(int));
    AddAtLast(&list, &b, &DisplayFloat, NULL, sizeof(float));
    AddAtLast(&list, &c, &DisplayDouble, NULL, sizeof(double));
    AddAtLast(&list, &d, &DisplayChar, NULL, sizeof(char));
    AddAtLast(&list, &e, &DisplayString, NULL, sizeof(char*));

    IsEmpty(list) ? printf("\nThe list is empty.\n") : printf("\nThe list is not empty.\n");

    Display(list);
    ErasedList(&list);
    Display(list);

    IsEmpty(list) ? printf("\nThe list is empty.\n") : printf("\nThe list is not empty.\n");

    AddInFront(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 6;
    AddInFront(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 3;
    AddInFront(&list, &a, &DisplayInt, NULL, sizeof(long int));
    a = 2;
    AddInFront(&list, &a, &DisplayInt, NULL, sizeof(long int));

    LIST popedList = PopInFront(&list, 2);

    printf("\nPoped list :\n");

    Display(popedList);

    printf("\nList :\n");

    Display(list);
    
    ErasedList(&list);
    ErasedList(&popedList);

    return 0;
}

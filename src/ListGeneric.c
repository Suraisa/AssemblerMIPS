#include "ListGeneric.h"

LIST CreateList()
{
    return NULL;
}

int IsEmpty(LIST list)
{
    return !list;
}

void AddInFront(LIST *list, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void *), size_t size)
{
    LIST newNode = CreateList();
    newNode = (LIST)calloc(1, sizeof(*newNode));
    newNode->data = malloc(size);

    if (newNode->data == NULL)
        return;

    memmove(newNode->data, dataToAdd, size);
    newNode->next = *list;
    newNode->display = display;
    newNode->erasedDataValue = erasedDataValue;
    *list = newNode;
}

void AddAtLast(LIST *list, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void *), size_t size)
{
    LIST copy = *list;

    LIST newNode = CreateList();
    newNode = (LIST)calloc(1, sizeof(*newNode));
    newNode->data = malloc(size);

    if (newNode->data == NULL)
        return;

    memmove(newNode->data, dataToAdd, size);
    newNode->display = display;
    newNode->erasedDataValue = erasedDataValue;

    if (!IsEmpty(copy))
    {
        while (!IsEmpty(copy->next))
        {
            copy = copy->next;
        }
        copy->next = newNode;
    }
    else
    {
        *list = newNode;
    }
}

void ErasedInFront(LIST *list)
{
    if (IsEmpty(*list))
        return;

    LIST copy = *list;
    (*list) = (*list)->next;
    if (copy->erasedDataValue != NULL)
    {
        (copy->erasedDataValue)(copy->data);
    }
    free(copy->data);
    free(copy);
}

void ErasedAtLast(LIST *list)
{
    if (IsEmpty(*list))
        return;

    LIST previousCopy = (*list);

    if (IsEmpty(previousCopy->next))
    {
        if (previousCopy->erasedDataValue != NULL)
        {
            (previousCopy->erasedDataValue)(previousCopy->data);
        }
        free(previousCopy->data);
        free(previousCopy);
        *list = NULL;
    }
    else
    {
        LIST copy = previousCopy->next;
        while (!IsEmpty(copy->next))
        {
            copy = copy->next;
            previousCopy = previousCopy->next;
        }
        if (copy->erasedDataValue != NULL)
        {
            (copy->erasedDataValue)(copy->data);
        }
        free(copy->data);
        free(copy);
        previousCopy->next = NULL;
    }
}

void ErasedList(LIST *list)
{
    LIST nodeI;
    LIST nodeISuiv;

    for (nodeI = *list; !IsEmpty(nodeI); nodeI = nodeISuiv)
    {
        nodeISuiv = nodeI->next;
        if (nodeI->erasedDataValue != NULL)
        {
            (nodeI->erasedDataValue)(nodeI->data);
        }
        free(nodeI->data);
        free(nodeI);
    }
    *list = NULL;
}

LIST PopInFront(LIST *list, unsigned long int number)
{
    if(number<1)
        return NULL;
        
    LIST popedList = *list;
    LIST copy = *list;
    int counter = 1;
    while (!IsEmpty(copy->next) && counter < number)
    {
        copy = copy->next;
        counter++;
    }
    *list = copy->next;
    copy->next = NULL;
    return popedList;
}

void Display(LIST list)
{
    LIST nodeI;
    for (nodeI = list; !IsEmpty(nodeI); nodeI = nodeI->next)
    {
        nodeI->display(nodeI->data);
    }
}

int SizeList(LIST list)
{
    if (IsEmpty(list))
        return 0;

    int counter = 0;
    LIST slider = list;

    while (!IsEmpty(slider))
    {
        counter++;
        slider = slider->next;
    }

    return counter;
}
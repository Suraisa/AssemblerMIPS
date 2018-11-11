#include "DoubleListGeneric.h"

LIST_DOUBLE CreateListDouble()
{
    return NULL;
}

int IsEmptyDouble(LIST_DOUBLE list)
{
    return !list;
}

void AddInFrontDouble(LIST_DOUBLE *list, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void *), size_t size)
{
    LIST_DOUBLE newNode = CreateListDouble();
    newNode = (LIST_DOUBLE)calloc(1, sizeof(*newNode));
    newNode->data = malloc(size);

    if (newNode->data == NULL)
        return;

    memmove(newNode->data, dataToAdd, size);
    newNode->prev = IsEmptyDouble(*list) ? newNode : (*list)->prev;
    newNode->next = IsEmptyDouble(*list) ? newNode : *list;
    if(!IsEmptyDouble(*list))
    {
        newNode->prev->next = newNode;
        newNode->next->prev = newNode;

    }
    newNode->display = display;
    newNode->erasedDataValue = erasedDataValue;
    *list = newNode;
}

void AddAtLastDouble(LIST_DOUBLE *list, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void *), size_t size)
{
    if(IsEmptyDouble(*list))
    {
        AddInFrontDouble(list, dataToAdd, display, erasedDataValue, size);
        return;
    }
    AddInFrontDouble(&((*list)->prev), dataToAdd, display, erasedDataValue, size);
}

void ErasedInFrontDouble(LIST_DOUBLE *list)
{
    if (IsEmptyDouble(*list))
        return;

    LIST_DOUBLE copy = *list;
    (*list)->prev->next = (*list)->next;
    (*list)->next->prev = (*list)->prev;
    (*list) = (*list)->next;
    if (copy->erasedDataValue != NULL)
    {
        (copy->erasedDataValue)(copy->data);
    }
    free(copy->data);
    free(copy);
}

void ErasedAtLastDouble(LIST_DOUBLE *list)
{
    if(IsEmptyDouble(*list))
        return;

    ErasedInFrontDouble(&((*list)->prev));    
}

void ErasedListDouble(LIST_DOUBLE *list)
{
    LIST_DOUBLE nodeI;
    LIST_DOUBLE nodeISuiv;

    for (nodeI = *list; !IsEmptyDouble(nodeI); nodeI = nodeISuiv)
    {
        if(!IsEmptyDouble(nodeI->prev))
        {
            nodeI->prev->next = NULL;
        }
        nodeI->prev = NULL;
        nodeISuiv = nodeI->next;
        if (nodeI->erasedDataValue != NULL)
        {
            (nodeI->erasedDataValue)(nodeI->data);
        }
        nodeI->next = NULL;
        free(nodeI->data);
        free(nodeI);
    }
    *list = NULL;
}

LIST_DOUBLE PopInFrontDouble(LIST_DOUBLE *list, unsigned long int number)
{
    if(number<1)
        return NULL;
        
    LIST_DOUBLE popedList = *list;
    LIST_DOUBLE copy = *list;
    int counter = 1;
    int sizeList = SizeListDouble(*list);
    if(sizeList <= number)
    {
        counter = sizeList;
        return *list;
    }
    else
    {
        do
        {
            copy = copy->next;
            counter++;
        }while (copy != *list && counter < number);
    }
    copy->next->prev = (*list)->prev;
    (*list)->prev->next = copy->next;
    *list = copy->next;
    copy->next = popedList;
    popedList->prev = copy;
    return popedList;
}

void DisplayDoubleList(LIST_DOUBLE list)
{
    if(IsEmptyDouble(list))
        return;

    LIST_DOUBLE firstNode = list;
    LIST_DOUBLE slider = list;
    do
    {
        slider->display(slider->data);
        slider = slider->next;
    }while(slider != firstNode);
}

int SizeListDouble(LIST_DOUBLE list)
{
    if (IsEmptyDouble(list))
        return 0;

    int counter = 1;
    LIST_DOUBLE firstNode = list;
    LIST_DOUBLE slider = list->next;

    while (slider != firstNode)
    {
        counter++;
        slider = slider->next;
    }

    return counter;
}
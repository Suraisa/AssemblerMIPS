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
    *list = newNode->next;
}

void ErasedInFrontDouble(LIST_DOUBLE *list)
{
    if (IsEmptyDouble(*list))
        return;

    LIST_DOUBLE copy = *list;
    if((*list)->next != (*list))
    {
        (*list)->prev->next = (*list)->next;
        (*list)->next->prev = (*list)->prev;
        (*list) = (*list)->next;
    }
    else
    {
        (*list) = NULL;
    }
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

    LIST_DOUBLE copy = (*list)->prev;
    if((*list)->next != (*list))
    {
        (*list)->prev->prev->next = (*list)->prev->next;
        (*list)->prev = (*list)->prev->prev;
    }
    else
    {
        (*list) = NULL;
    }
    if (copy->erasedDataValue != NULL)
    {
        (copy->erasedDataValue)(copy->data);
    }
    free(copy->data);
    free(copy);
}

void ErasedListDouble(LIST_DOUBLE *list)
{
    while(!IsEmptyDouble(*list))
    {
        ErasedInFrontDouble(list);
    }
    *list = NULL;
}

LIST_DOUBLE PopInFrontDouble(LIST_DOUBLE *list, unsigned long int number)
{
    if(number<1)
        return NULL;
        
    LIST_DOUBLE popedList = *list;
    LIST_DOUBLE copy = *list;
    int counter = 0;
    int sizeList = SizeListDouble(*list);
    if(sizeList <= number)
    {
        *list = NULL;
        return popedList;
    }
    else
    {
        do
        {
            copy = copy->next;
            counter++;
        }while (copy != *list && counter < number);
    }
    LIST_DOUBLE LastNode = popedList->prev;
    copy->prev->next = popedList;
    popedList->prev = copy->prev;
    copy->prev = LastNode;
    copy->prev->next = copy;
    *list = copy;
    return popedList;
}

void ConcatenateListDouble(LIST_DOUBLE* list1, LIST_DOUBLE* list2)
{
    if(IsEmptyDouble(*list1))
    {
        *list1 = *list2;
        return;
    }
    LIST_DOUBLE list1End = (*list1)->prev;
    
    (*list1)->prev = (*list2)->prev;
    list1End->next = (*list2);
    (*list2)->prev->next = (*list1);
    (*list2)->prev = list1End;
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
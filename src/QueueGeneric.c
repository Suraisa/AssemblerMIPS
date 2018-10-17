#include "QueueGeneric.h"

QUEUE CreateQueue()
{
    return CreateList();
}

void PushQueue(QUEUE *queue, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void*), size_t size)
{
    AddAtLast(queue, dataToAdd, display, erasedDataValue, size);
}

void ErasedQueue(QUEUE *queue)
{
    ErasedList(queue);
}

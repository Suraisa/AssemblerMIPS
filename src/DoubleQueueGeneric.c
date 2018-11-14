#include "DoubleQueueGeneric.h"

QUEUE_DOUBLE CreateQueueDouble()
{
    return CreateListDouble();
}

void PushQueueDouble(QUEUE_DOUBLE *queue, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void*), size_t size)
{
    AddAtLastDouble(queue, dataToAdd, display, erasedDataValue, size);
}

void ErasedQueueDouble(QUEUE_DOUBLE *queue)
{
    ErasedListDouble(queue);
}

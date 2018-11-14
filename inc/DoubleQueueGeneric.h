/**
 * @file QueueGeneric.h
 * @brief Queue-related stuff.
 *
 * Contains the Queue definition and the associated prototypes to have multiple types inside the queue.
 */

#ifndef DOUBLE_QUEUEGENERIC_H
#define DOUBLE_QUEUEGENERIC_H

#include "DoubleListGeneric.h"

#define QUEUE_DOUBLE LIST_DOUBLE

QUEUE_DOUBLE CreateQueueDouble();

/**
 * @param list Pointer on a list to add an element on it.
 * @param dataToAdd Pointer of the value to add.
 * @param display Pointer of a function which take a void* on parameter.\nIt's use to recognize the value type (generally use a display for the value added).
 * @param erasedDataValue Pointer of a function which take a void* on parameter.\nIt's use to desallocate the data inside the value.
 * @param size size_t represente the size of the dataToAdd.
 * @brief Add a value on the last position of the list.
 *
 */
void PushQueueDouble(QUEUE_DOUBLE*, void*, void (*display)(void*), void (*erasedDataValue)(void*), size_t);

/**
 * @param list Pointer on the list.
 * @brief Desallocate all elements of the list.
 *
 */
void ErasedQueueDouble(QUEUE_DOUBLE*);

#endif
/**
 * @file ListGeneric.h
 * @brief List-related stuff.
 *
 * Contains the List structure definition and the associated prototypes to have multiple types inside the list.
 */

#ifndef LISTGENERIC_H
#define LISTGENERIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NODE
{
    void* data;
    void (*display)();
    void (*erasedDataValue)();
    struct NODE* next;
}* LIST;

/**
 * @brief Create an empty list.
 *
 */
LIST CreateList();

/**
 * @param list LIST to test.
 * @return -#1 if the list is empty.\n-#0 if it's not
 * @brief Test if the list is empty.
 *
 */
int IsEmpty(LIST list);

/**
 * @param list Pointer on a list to add an element on it.
 * @param dataToAdd Pointer of the value to add.
 * @param display Pointer of a function which take a void* on parameter.\nIt's use to recognize the value type (generally use a display for the value added).
 * @param erasedDataValue Pointer of a function which take a void* on parameter.\nIt's use to desallocate the data inside the value.
 * @param size size_t represente the size of the dataToAdd.
 * @brief Add a value in front of the list.
 *
 */
void AddInFront(LIST *list, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void *), size_t size);

/**
 * @param list Pointer on a list to add an element on it.
 * @param dataToAdd Pointer of the value to add.
 * @param display Pointer of a function which take a void* on parameter.\nIt's use to recognize the value type (generally use a display for the value added).
 * @param erasedDataValue Pointer of a function which take a void* on parameter.\nIt's use to desallocate the data inside the value.
 * @param size size_t represente the size of the dataToAdd.
 * @brief Add a value on the last position of the list.
 *
 */
void AddAtLast(LIST *list, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void *), size_t size);

/**
 * @param list Pointer on a list to erased an element.
 * @brief Desallocate the element in front of the list.
 *
 */
void ErasedInFront(LIST *list);

/**
 * @param list Pointer on a list to erased an element.
 * @brief Desallocate the element on the last position of the list.
 *
 */
void ErasedAtLast(LIST *list);

/**
 * @param list Pointer on the list.
 * @brief Desallocate all elements of the list.
 *
 */
void ErasedList(LIST *list);

/**
 * @param list LIST to display.
 * @brief Display the list thanks to the function display of the list.
 *
 */
void Display(LIST list);

/**
 * @param list LIST desired.
 * @return The size of the list in an integer value.
 * @brief Give the size of the list.
 *
 */
int SizeList(LIST list);

#endif
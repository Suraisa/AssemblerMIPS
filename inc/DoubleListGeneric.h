/**
 * @file ListGeneric.h
 * @brief List-related stuff.
 *
 * Contains the List structure definition and the associated prototypes to have multiple types inside the list.
 */

#ifndef DOUBLELISTGENERIC_H
#define DOUBLELISTGENERIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NODE_DOUBLE
{
    void* data;
    void (*display)();
    void (*erasedDataValue)();
    struct NODE_DOUBLE* next;
    struct NODE_DOUBLE* prev;
}* LIST_DOUBLE;

/**
 * @brief Create an empty list.
 *
 */
LIST_DOUBLE CreateListDouble();

/**
 * @param list LIST_DOUBLE to test.
 * @return -#1 if the list is empty.\n-#0 if it's not
 * @brief Test if the list is empty.
 *
 */
int IsEmptyDouble(LIST_DOUBLE list);

/**
 * @param list Pointer on a list to add an element on it.
 * @param dataToAdd Pointer of the value to add.
 * @param display Pointer of a function which take a void* on parameter.\nIt's use to recognize the value type Double(generally use a display for the value added).
 * @param erasedDataValue Pointer of a function which take a void* on parameter.\nIt's use to desallocate the data inside the value.
 * @param size size_t represente the size of the dataToAdd.
 * @brief Add a value in front of the list.
 *
 */
void AddInFrontDouble(LIST_DOUBLE *list, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void *), size_t size);

/**
 * @param list Pointer on a list to add an element on it.
 * @param dataToAdd Pointer of the value to add.
 * @param display Pointer of a function which take a void* on parameter.\nIt's use to recognize the value type (generally use a display for the value added).
 * @param erasedDataValue Pointer of a function which take a void* on parameter.\nIt's use to desallocate the data inside the value.
 * @param size size_t represente the size of the dataToAdd.
 * @brief Add a value on the last position of the list.
 *
 */
void AddAtLastDouble(LIST_DOUBLE *list, void *dataToAdd, void (*display)(void *), void (*erasedDataValue)(void *), size_t size);

/**
 * @param list Pointer on a list to erased an element.
 * @brief Desallocate the element in front of the list.
 *
 */
void ErasedInFrontDouble(LIST_DOUBLE *list);

/**
 * @param list Pointer on a list to erased an element.
 * @brief Desallocate the element on the last position of the list.
 *
 */
void ErasedAtLastDouble(LIST_DOUBLE *list);

/**
 * @param list Pointer on the list.
 * @brief Desallocate all elements of the list.
 *
 */
void ErasedListDouble(LIST_DOUBLE *list);

/**
 * @param list LIST_DOUBLE to display.
 * @brief Display the list thanks to the function display of the list.
 *
 */
void DisplayDoubleList(LIST_DOUBLE list);

/**
 * @param list LIST_DOUBLE desired.
 * @return The size of the list in an integer value.
 * @brief Give the size of the list.
 *
 */
int SizeListDouble(LIST_DOUBLE list);

/**
 * @param list LIST_DOUBLE desired.
 * @param number Unsigned long int : number of element you want to pop from the list.
 * @return A list of poped element.
 * @brief Pop "number" element of the list, if number is higher than the list's size, it will stop at the last element of the list.
 *
 */
LIST_DOUBLE PopInFrontDouble(LIST_DOUBLE *list, unsigned long int number);

void ConcatenateListDouble(LIST_DOUBLE* list1, LIST_DOUBLE* list2);

#endif
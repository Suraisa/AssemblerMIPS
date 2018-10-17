/**
 * @file DisplayType.h
 * @brief Display function for types.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>

/**
 * @param value Void pointer to the value to display.
 * @brief This function display the value as an integer.
 *
 */
void DisplayInt(void* value);

/**
 * @param value Void pointer to the value to display.
 * @brief This function display the value as a float.
 *
 */
void DisplayFloat(void* value);

/**
 * @param value Void pointer to the value to display.
 * @brief This function display the value as a double.
 *
 */
void DisplayDouble(void* value);

/**
 * @param value Void pointer to the value to display.
 * @brief This function display the value as a char.
 *
 */
void DisplayChar(void* value);

/**
 * @param value Void pointer to the value to display.
 * @brief This function display the value as a string.
 *
 */
void DisplayString(void* value);

#endif
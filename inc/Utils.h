/**
 * @file Utils.h
 * @brief Utils stuff.
 *
 * Contains some convenient functions.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "ListGeneric.h"
#include "DoubleListGeneric.h"
#include "QueueGeneric.h"
#include "DisplayType.h"
#include <stdlib.h>

/**
 * @param string Char* to analyse.
 * @return an unsigned int which represent the string's size. 
 * @brief Return string's size.
 *
 */
unsigned int StringSize(char* string);

/**
 * @param list List of char to concatenate.
 * @return a string of concatenate char. 
 * @brief Concatenate a list of char to make a string.\nIt'll construct the string from the end of the list to the front.
 *
 */
char* ConcatenateCharList(LIST list);

/**
 * @param list LIST_DOUBLE of char to concatenate.
 * @return a string of concatenate char. 
 * @brief Concatenate a list of char to make a string.\nIt'll construct the string from the end of the list to the front.
 *
 */
char* ConcatenateCharListDouble(LIST_DOUBLE list);

/**
 * @param list List of int to concatenate.
 * @return a long int of concatenate int. 
 * @brief Concatenate a list of int to make a long int.\nIt'll construct the string from the end of the list to the front.
 *
 */
long int ConcatenateIntList(LIST);

/**
 * @param list LIST_DOUBLE of int to concatenate.
 * @return a long int of concatenate int. 
 * @brief Concatenate a list of int to make a long int.\nIt'll construct the string from the end of the list to the front.
 *
 */
long int ConcatenateIntListDouble(LIST_DOUBLE);

/**
 * @param value Char to test.
 * @return -#1 If it's true.\n-#0 If it's not 
 * @brief test if the value is a number.
 *
 */
int CharIsNumber(char value);

/**
 * @param value Char to test.
 * @return -#1 If it's true.\n-#0 If it's not 
 * @brief test if the value is lowercase letter.
 *
 */
int CharIsLowerLetter(char value);

/**
 * @param value Char to test.
 * @return -#1 If it's true.\n-#0 If it's not 
 * @brief test if the value is uppercase letter.
 *
 */
int CharIsUpperLetter(char value);

/**
 * @param value Char to test.
 * @return -#1 If it's true.\n-#0 If it's not 
 * @brief test if the value is a letter.
 *
 */
int CharIsLetter(char value);

/**
 * @param value Char to test.
 * @return -#1 If it's true.\n-#0 If it's not 
 * @brief test if the value is an hexadecimal letter.
 *
 */
int CharIsHexadecimal(char value);

/**
 * @param value Char to test.
 * @return -#1 If it's true.\n-#0 If it's not 
 * @brief test if the value is a letter or a number.
 *
 */
int CharIsNumberLetter(char value);

void StringUpper(char* string);

unsigned long int StringHexToDecimal(char* value);

int IsAvailableRegister(char* registerToTest, int** newName);

void ChangeRegisterName(int** newName, int number);

void ReplaceString(char** fullString, char* replaceString, int start, int end);

char* CutString(char* fullString, int start, int length);

#endif
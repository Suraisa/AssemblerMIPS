#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <math.h>
#include "ListGeneric.h"
#include "DisplayType.h"
#include "Global.h"

/**
 * @return -#1 if the list is empty.\n-#0 if it's not
 * @brief Test if the list is empty.
 *
 */
LIST* CreateHashTable();

/**
 * @param list LIST to test.
 * @return -#1 if the list is empty.\n-#0 if it's not
 * @brief Test if the list is empty.
 *
 */
int Hash(char * string);

/**
 * @param list LIST to test.
 * @return -#1 if the list is empty.\n-#0 if it's not
 * @brief Test if the list is empty.
 *
 */
void AddHashTable(LIST** hash,char** string);

#endif

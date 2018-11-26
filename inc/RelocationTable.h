#ifndef RELOCATIONTABLE_H
#define RELOCATIONTABLE_H

#include "DoubleListGeneric.h"
#include "Section.h"

typedef enum
{
  R_MIPS_32=2,
  R_MIPS_26=4,
  R_MIPS_HI16=5,
  R_MIPS_LO16=6
} TYPE;

extern char* definedR_MIPS[7];

typedef struct
{
  COLLECTIONS symbolSection; // 0 : text, 1 : data, 2 : bss
  unsigned int relativeAdress; // Relative adress of the symbol in relation to the section
  TYPE typeRMIPS;
  LEXEME* symbolAdress;
} LINKRELOCATION;

typedef struct
{
  LIST_DOUBLE relocationText;
  LIST_DOUBLE relocationData;
} RELOCATIONTABLE;

/**
 * @brief return a table of relocation of two empty lists of relocation.
 *
 */
RELOCATIONTABLE CreateRelocationList();

/**
 * @param relocationList Pointer on a list of relocation to fill it.
 * @param symbolSection Integer embodying the section of the table of relocation, 0 for ".text", 1 for ".data" and 2 for ".bss".
 * @param relativeAdress Relative adress of the symbol in relation to the section.
 * @param typeRMIPS Number of bits we must relocate.
 * @param symbolAdress Adress of return at the symbol we relocated.
 * @brief Fill a list of relocation.
 *
 */
void FillRelocationList(LIST_DOUBLE* relocationList, COLLECTIONS symbolSection, unsigned int relativeAdress, TYPE typeRMIPS, LEXEME* symbolAdress);

/**
 * @param relocationList LIST_DOUBLE to display.
 * @brief Display the relocation list.
 *
 */
void DisplayRelocationList(LIST_DOUBLE relocationList);

/**
 * @param relocationTable Table of relocation which contain the text' table and the data's table.
 * @brief Display the relocation table entire.
 *
 */
void DisplayRelocationTable(RELOCATIONTABLE relocationTable);

/**
 * @param relocationList Pointer on a list of relocation to update it.
 * @param collections Section containing the lexemes.
 * @brief Desallocate all elements of the list.
 *
 */
void UpdateRelocationList(LIST_DOUBLE* relocationList, COLLECTION_LISTS collections)

/**
 * @param relocationTable Table of relocation which contain the text' table and the data's table.
 * @param collections Section containing the lexemes.
 * @brief Desallocate all elements of the list.
 *
 */
void UpdateRelocationTable(RELOCATIONTABLE relocationTable, COLLECTION_LISTS collections);

/**
 * @param relocationTable Pointer on the table of relocation.
 * @brief Desallocate all elements of the list.
 *
 */
void ErasedRelocationTable(RELOCATIONTABLE* relocationTable);

#endif

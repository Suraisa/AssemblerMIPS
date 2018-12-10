#ifndef RELOCATIONTABLE_H
#define RELOCATIONTABLE_H

#include "DoubleListGeneric.h"
#include "Section.h"
#include "FSMOperand.h"
#include "Lexeme.h"

typedef enum
{
  R_MIPS_ERROR=0,
  R_MIPS_32=2,
  R_MIPS_26=4,
  R_MIPS_HI16=5,
  R_MIPS_LO16=6
} MIPS_TYPE;

extern char* definedR_MIPS[7];

typedef struct
{
  COLLECTIONS symbolSection; // 0 : text, 1 : data, 2 : bss
  unsigned long int relativeAddress; // Relative adress of the symbol in relation to the section
  MIPS_TYPE typeRMIPS;
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
RELOCATIONTABLE CreateRelocationTable();

/**
 * @param relocationList Pointer on a list of relocation to fill it.
 * @param symbolSection Integer embodying the section of the table of relocation, 0 for ".text", 1 for ".data" and 2 for ".bss".
 * @param relativeAddress Relative adress of the symbol in relation to the section.
 * @param typeRMIPS Number of bits we must relocate.
 * @brief Fill a list of relocation.
 *
 */
void FillRelocationList(LIST_DOUBLE* relocationList, COLLECTIONS symbolSection, unsigned long int relativeAddress, MIPS_TYPE typeRMIPS);

/**
 * @param value Display value, ie a node of a list.
 * @brief Display the relocation list.
 *
 */
void DisplayRelocation(void* value);

/**
 * @param relocationTable Table of relocation which contain the text' table and the data's table.
 * @brief Display the relocation table entire.
 *
 */
void DisplayRelocationTable(RELOCATIONTABLE relocationTable);

/**
 * @param relocationList Pointer on a list of relocation to update it.
 * @param section Section containing the lexemes.
 * @brief Update of the relocation's table of text.
 *
 */
void UpdateRelocationText(LIST_DOUBLE* relocationList, SECTION** section, LIST_DOUBLE *hash, INSTRUCTION* dicoInstruct);

/**
 * @param relocationList Pointer on a list of relocation to update it.
 * @param section Section containing the lexemes.
 * @brief Update of the relocation's table of data.
 *
 */
void UpdateRelocationData(LIST_DOUBLE* relocationList, SECTION** section, LIST_DOUBLE *hash);

/**
 * @param relocationTable Table of relocation which contain the text' table and the data's table.
 * @param hash Hash table.
 * @param section Section containing the lexemes.
 * @brief Update of the relocation's table.
 *
 */
void UpdateRelocationTable(RELOCATIONTABLE* relocationTable, LIST_DOUBLE *hash, COLLECTION_LISTS* collectionsList, INSTRUCTION* dicoInstruct);

/**
 * @param relocationTable Pointer on the table of relocation.
 * @brief Desallocate all elements of the list.
 *
 */
void ErasedRelocationTable(RELOCATIONTABLE* relocationTable);

#endif

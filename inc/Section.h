#ifndef SECTION_H
#define SECTION_H

#include "DoubleListGeneric.h"
#include "DoubleQueueGeneric.h"
#include "FSMOperand.h"
#include "DicoInstruct.h"
#include "Global.h"

#ifndef ENUM_COLLECTION
#define ENUM_COLLECTION
typedef enum
{
    TEXT,
    DATA,
    BSS,
    UNDEF
} COLLECTIONS;
#endif

#ifndef ENUM_COLLECTION_STATE
#define ENUM_COLLECTION_STATE
typedef enum
{
    DIR0,
    ASCIZZ,
    SPACE,
    WORD,
    BYTE,
    SET_NOREORDER,
    INSTRUCTION0,
    INSTRUCTION1,
    INIT_COLLECTION
} COLLECTION_STATE;
#endif

typedef enum
{
    INST,
    DIR,
    LABEL
} COLLECTION_DATA_TYPE;

typedef struct
{
    char* name;
    int dicoIndex;
    int lowerBits;
    unsigned long int lineNumber;
    int pseudoInstruction;
    QUEUE_DOUBLE lexemeList[OP_PSEUDO_MAX];
} INSTRUCTION_DATA;

typedef struct
{
    COLLECTIONS section;
    QUEUE_DOUBLE lexemeList;
} LABEL_DATA;

typedef union
{
    INSTRUCTION_DATA instruction;
    LIST_DOUBLE directiveValue;
    LABEL_DATA label;
} COLLECTION_DATA;

typedef struct
{
    QUEUE_DOUBLE collection[3];
    LIST_DOUBLE* labelTable;
} COLLECTION_LISTS;

typedef struct
{
    COLLECTION_STATE state;
    unsigned long int shift;
    char dataType;
    COLLECTION_DATA data;
} SECTION;

/**
 * @brief Table of collection's Type equivalent to the COLLECTION_STATE.
 *
 */
extern char* collectionType[8];

/**
 * @brief Table of collection's section equivalent to the COLLECTIONS.
 *
 */
extern char* collectionSection[4];

#include "HashTable.h"
#include "PseudoInstruction.h"
#include "FSMCollection.h"


/**
 * @param collections COLLECTION_LISTS* which is the collection list to initialize.
 * @brief Table of collection's section equivalent to the COLLECTIONS.
 *
 */
void InitializeCollectionLists(COLLECTION_LISTS* collections);

/**
 * @param state COLLECTION_STATE which is the state of the instruction.
 * @param shif unsigned long int which is the shift of the instruction.
 * @param instructionName char* which is the name of the instruction.
 * @param dicoIndex int which is the index of the instruction in the dictionary.
 * @param lineNumber unsigned long int which is the line number of the instructio.
 * 
 * @brief Create an instruction Section with the setup.
 * @return an instruction SECTION*
 * 
 */
SECTION CreateInstructionSection(COLLECTION_STATE state, unsigned long int shift, char* instructionName, int dicoIndex, unsigned long int lineNumber, int pseudoInstruction);

/**
 * @param state COLLECTION_STATE which is the state of the instruction.
 * @param shif unsigned long int which is the shift of the instruction.
 * @param lexemeList LIST_DOUBLE* which is a list of lexeme corresponding to the lexeme of this directive.
 * 
 * @brief Create an instruction Section with the setup.
 * @return an directive SECTION*
 * 
 */
SECTION CreateDirectiveSection(COLLECTION_STATE state, unsigned long int shift, LIST_DOUBLE* lexemeList);

/**
 * @param stateMachine COLLECTION_FSM which is the fsm of the Collection.
 * @param lexemeList LIST_DOUBLE* which is a list of lexeme corresponding to the lexeme of this label.
 * 
 * @brief Create an instruction Section with the setup.
 * @return an label SECTION*
 * 
 */
SECTION CreateLabelSection(COLLECTION_FSM stateMachine, LIST_DOUBLE* lexemeList);

/**
 * @param lexemeList LIST_DOUBLE* which is a list of lexeme where we want to count.
 * 
 * @brief Count the nomber of lexeme in the operand.
 * @return Integer which is the number of lexeme in the operand.
 * 
 */
int NumberLexemeOperand(LIST_DOUBLE lexemeList);

/**
 * @param stateMachine COLLECTION_FSM* which is the fsm of the Collection.
 * @param section SECTION* which is the section where we want to add the operand.
 * @param lexemeList LIST_DOUBLE* which is a list of lexeme where we want to pop some of them for the operand.
 * @param instructionDictionary INSTRUCTION* which is a the instructions' dictionary.
 * 
 * @brief Add an operand in the instruction.
 * @return -#1 If it worked.\n-#0 If it didn't
 * 
 */
int AddOperand(COLLECTION_FSM* stateMachine, SECTION* section, LIST_DOUBLE* fullLexemesList, LIST_DOUBLE* lexemeList, INSTRUCTION* instructionDictionary, PSEUDO_INSTRUCTION* pseudoDictionary);

/**
 * @param value void* is a SECTION*.
 * 
 * @brief Display a section.
 * 
 */
void DisplaySection(void* value);

/**
 * @param collections COLLECTION_LISTS which is the collection to display.
 * 
 * @brief Display a COLLECTION_LISTS.
 * 
 */
void DisplayCollectionLists(COLLECTION_LISTS collections);

/**
 * @param value void* is a SECTION*.
 * 
 * @brief Deallocate the section.
 * 
 */
void ErasedSection(void* value);

/**
 * @param value void* is a SECTION*.
 * 
 * @brief Deallocate the collectionList.
 * 
 */
void ErasedCollectionLists(void* value);

#endif
#include "Section.h"

char* collectionType[8] = {
    "DIR0",
    "ASCIZZ",
    "SPACE",
    "WORD",
    "BYTE",
    "SET_NOREORDER",
    "INSTRUCTION0",
    "INSTRUCTION1"};

char* collectionSection[4] = {
    "TEXT",
    "DATA",
    "BSS",
    "UNDEF"};

void InitializeCollectionLists(COLLECTION_LISTS* collections)
{
    int index;
    for(index = 0; index<3; index++)
    {
        collections->collection[index] = CreateQueueDouble();
    }
    
    for(index = 0; index<3; index++)
    {
        *(collections->labelTable + index) = NULL;
    }
}

SECTION CreateInstructionSection(COLLECTION_STATE state, unsigned long int shift, char* instructionName, int dicoIndex, unsigned long int lineNumber, int pseudoInstruction)
{
    SECTION section;
    section.state = state;
    section.shift = shift;
    section.dataType = INST;
    section.data.instruction.name = instructionName;
    section.data.instruction.dicoIndex = dicoIndex;
    section.data.instruction.lineNumber = lineNumber;
    section.data.instruction.lowerBits = 1;
    section.data.instruction.pseudoInstruction = pseudoInstruction;

    int index;
    for(index = 0; index<3; index++)
    {
        section.data.instruction.lexemeList[index] = NULL;
    }

    return section;
}

SECTION CreateDirectiveSection(COLLECTION_STATE state, unsigned long int shift, LIST_DOUBLE* lexemeList)
{
    SECTION section;
    section.state = state;
    section.shift = shift;
    section.dataType = DIR;
    section.data.directiveValue = *lexemeList;

    return section;
}

SECTION CreateLabelSection(COLLECTION_FSM stateMachine, LIST_DOUBLE* lexemeList)
{
    SECTION section;
    section.state = stateMachine.currentState;
    section.shift = stateMachine.nextShift[stateMachine.actualCollection];
    section.dataType = LABEL;
    section.data.label.section = stateMachine.actualCollection;
    section.data.label.lexemeList = *lexemeList;

    return section;
}

int NumberLexemeOperand(LIST_DOUBLE lexemeList)
{
    if(IsEmptyDouble(lexemeList))
        return 0;

    LIST_DOUBLE nodeI = lexemeList;
    LIST_DOUBLE firstNode = lexemeList;
    int index = 0;
    do
    {
        index++;
        nodeI = nodeI->next;
    }while(nodeI != firstNode && ((LEXEME *)nodeI->data)->state != COMMA && ((LEXEME *)nodeI->data)->state != RETURN && ((LEXEME *)nodeI->data)->state != COMMENT && ((LEXEME *)nodeI->data)->state != COLON);
    return index;
}

int AddOperand(COLLECTION_FSM* stateMachine, SECTION* section, LIST_DOUBLE* lexemeList, INSTRUCTION* instructionDictionary, PSEUDO_INSTRUCTION* pseudoDictionary)
{
    int index=0;
    while (index<3 && !IsEmptyDouble(section->data.instruction.lexemeList[index]))
    {
        index++;
    }

    if(!section->data.instruction.pseudoInstruction && index>=instructionDictionary[section->data.instruction.dicoIndex].typeNumber)
    {
        return 0;
    }

    if(section->data.instruction.pseudoInstruction && index>=pseudoDictionary[section->data.instruction.dicoIndex].typeNumber)
    {
        return 0;
    }

    FSM_STATE_OPERAND fsmOperand;

    if(!section->data.instruction.pseudoInstruction)
    {
        InitializationOperandFsm(&fsmOperand, instructionDictionary[section->data.instruction.dicoIndex].operands[index]);
        OperandFSM(&fsmOperand, lexemeList);
        if(fsmOperand.error)
        {
            section->data.instruction.dicoIndex = IndexPseudoInstruction(pseudoDictionary, section->data.instruction.name);

            if(section->data.instruction.dicoIndex == -1)
                return 0;

            section->data.instruction.pseudoInstruction = 1;
            InitializationOperandFsm(&fsmOperand, pseudoDictionary[section->data.instruction.dicoIndex].operands[index]);
            OperandFSM(&fsmOperand, lexemeList);
        }
    }
    else
    {
        InitializationOperandFsm(&fsmOperand, pseudoDictionary[section->data.instruction.dicoIndex].operands[index]);
        OperandFSM(&fsmOperand, lexemeList);
    }

    if (fsmOperand.error)
            return 0;

    section->data.instruction.lexemeList[index] = *lexemeList;
    return 1;
}

void DisplaySection(void* value)
{
    SECTION section = *(SECTION*)value;
    char *separator = "-------------------------------------\n*************************************\n-------------------------------------";
    printf("\n");
    printf("%s\n", separator);
    printf("State: %25s\n", collectionType[section.state]);
    printf("Shift: %25lu", section.shift);

    switch(section.dataType)
    {
        case 0:
        {
            printf("\nInstruction: %19s\n", section.data.instruction.name);
            printf("Line number: %19lu\n", section.data.instruction.lineNumber);
            int index;
            for (index = 0; index < 3; index++)
            {
                if(!IsEmptyDouble(section.data.instruction.lexemeList[index]))
                {
                    DisplayDoubleList(section.data.instruction.lexemeList[index]);
                }
            }
            break;
        }
        case 1:
        {
            DisplayDoubleList(section.data.directiveValue);
            break;
        }
        case 2:
        {
            printf("\nLabel section: %17s\n", collectionSection[section.data.label.section]);
            DisplayDoubleList(section.data.label.lexemeList);
            break;
        }
    }
    printf("%s\n", separator);
    printf("\n");
}

void DisplayCollectionLists(COLLECTION_LISTS collections)
{
    char* separator = "\n-------------------------------\n";
    printf("\x1b[35m" "%sTEXT's Collection%s" "\x1b[0m",separator,separator);
    DisplayDoubleList(collections.collection[0]);
    printf("\x1b[35m" "%sDATA's Collection%s" "\x1b[0m",separator,separator);
    DisplayDoubleList(collections.collection[1]);
    printf("\x1b[35m" "%sBSS's Collection%s" "\x1b[0m",separator,separator);
    DisplayDoubleList(collections.collection[2]);
    printf("\x1b[35m" "%sLABELS' Collection%s" "\x1b[0m",separator,separator);
    DisplayHashTable(collections.labelTable);
}

void ErasedSection(void* value)
{
    switch(((SECTION*)value)->dataType)
    {
        case INST:
        {
            int index = 0;
            for (index; index < 3; index++)
            {
                ErasedListDouble(&(*(SECTION*)value).data.instruction.lexemeList[index]);
            }
            break;
        }
        case DATA:
        {
            ErasedListDouble(&(*(SECTION*)value).data.directiveValue);
            break;
        }
        case BSS:
        {
            ErasedListDouble(&(*(SECTION*)value).data.label.lexemeList);
            break;
        }
    }
}

void ErasedCollectionLists(void* value)
{
    COLLECTION_LISTS* collectionLists = (COLLECTION_LISTS*)value;
    
    int index;
    for(index = 0; index<3; index++)
    {
        ErasedListDouble(&(collectionLists->collection[index]));
    }

    ErasedHashTable(&(collectionLists->labelTable));
}
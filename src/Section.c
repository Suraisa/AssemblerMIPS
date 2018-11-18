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

char* collectionSection[3] = {
    "TEXT",
    "DATA",
    "BSS"};

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

SECTION* CreateInstructionSection(COLLECTION_STATE state, unsigned long int shift, char* instructionName, int operandNumber, unsigned long int lineNumber)
{
    SECTION* section = calloc(1, sizeof(*section));
    if(section == NULL)
    {
        printf("\nERROR: Not enought memory for instructions\n");
        return NULL;
    }
    section->state = state;
    section->shift = shift;
    section->dataType = INST;
    section->data.instruction.name = instructionName;
    section->data.instruction.operandNumber = operandNumber;
    section->data.instruction.lineNumber = lineNumber;

    int index;
    for(index = 0; index<3; index++)
    {
        section->data.instruction.lexemeList[index] = NULL;
    }

    return section;
}

SECTION* CreateDirectiveSection(COLLECTION_STATE state, unsigned long int shift, LIST_DOUBLE* lexemeList)
{
    SECTION* section = calloc(1, sizeof(*section));
    if(section == NULL)
    {
        printf("\nERROR: Not enought memory for directive\n");
        return NULL;
    }

    section->state = state;
    section->shift = shift;
    section->dataType = DIR;
    section->data.directiveValue = *lexemeList;

    return section;
}

SECTION* CreateLabelSection(COLLECTION_FSM stateMachine, LIST_DOUBLE* lexemeList)
{
    SECTION* section = calloc(1, sizeof(*section));
    if(section == NULL)
    {
        printf("\nERROR: Not enought memory for label\n");
        return NULL;
    }
    section->state = stateMachine.currentState;
    section->shift = stateMachine.nextShift[stateMachine.actualCollection];
    section->dataType = LABEL;
    section->data.label.section = stateMachine.actualCollection;
    section->data.label.lexemeList = *lexemeList;

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
        printf("%s\n",((LEXEME *)nodeI->data)->type);
    }while(nodeI != firstNode && ((LEXEME *)nodeI->data)->state != COMMA && ((LEXEME *)nodeI->data)->state != RETURN && ((LEXEME *)nodeI->data)->state != COMMENT && ((LEXEME *)nodeI->data)->state != COLON);
    return index;
}

int AddOperand(COLLECTION_FSM* stateMachine, SECTION* section, LIST_DOUBLE* lexemeList)
{
    int index=0;

    while (index<3 && !IsEmptyDouble(section->data.instruction.lexemeList[index]))
    {
        index++;
    }
    if (index>=section->data.instruction.operandNumber)
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
    printf("%sTEXT's Collection%s",separator,separator);
    DisplayDoubleList(collections.collection[0]);
    printf("%sDATA's Collection%s",separator,separator);
    DisplayDoubleList(collections.collection[1]);
    printf("%sBSS's Collection%s",separator,separator);
    DisplayDoubleList(collections.collection[2]);
    printf("%sLABELS' Collection%s",separator,separator);
    DisplayHashTable(collections.labelTable);
}

void ErasedSection(void* value)
{
    SECTION section = *(SECTION*)value;
    switch(section.dataType)
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
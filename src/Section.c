#include "Section.h"

char* collectionType[11] = {
    "DIR0",
    "ASCIZZ",
    "SPACE",
    "WORD",
    "BYTE",
    "SET_NOREORDER",
    "INSTRUCTION0",
    "INSTRUCTION1",
    "INSTRUCTION2",
    "INSTRUCTION3",
    "INSTRUCTION4"};

char* collectionSection[3] = {
    "TEXT",
    "DATA",
    "BSS"};

void InitializeCollectionLists(COLLECTION_LISTS* collections)
{
    int index;
    for(index = 0; index<3; index++)
    {
        collections->collection[index] = CreateQueue();
    }
    
    for(index = 0; index<3; index++)
    {
        *(collections->labelTable + index) = NULL;
    }
}

SECTION* CreateInstructionSection(COLLECTION_STATE state, unsigned long int shift, char* instructionName, int operandNumber, unsigned long int lineNumber)
{
    SECTION* section = calloc(1, sizeof(*section));
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

SECTION* CreateDirectiveSection(COLLECTION_STATE state, unsigned long int shift, LIST* lexemeList)
{
    SECTION* section = calloc(1, sizeof(*section));
    section->state = state;
    section->shift = shift;
    section->dataType = DIR;
    section->data.directiveValue = *lexemeList;

    return section;
}

SECTION* CreateLabelSection(COLLECTION_FSM stateMachine, LIST* lexemeList)
{
    SECTION* section = calloc(1, sizeof(*section));
    section->state = stateMachine.currentState;
    section->shift = stateMachine.nextShift[stateMachine.actualCollection];
    section->dataType = LABEL;
    section->data.label.section = stateMachine.actualCollection;
    section->data.label.lexemeList = *lexemeList;

    return section;
}

int NumberLexemeOperand(LIST lexemeList)
{
    LIST nodeI;
    int index = 0;
    for (nodeI = lexemeList; !IsEmpty(nodeI) && ((LEXEME *)nodeI->data)->state != COMMA && ((LEXEME *)nodeI->data)->state != RETURN && ((LEXEME *)nodeI->data)->state != COMMENT && ((LEXEME *)nodeI->data)->state != COLON; nodeI = nodeI->next)
    {
        index++;
    }
    return index;
}

int AddOperand(COLLECTION_FSM* stateMachine, SECTION* section, LIST* lexemeList)
{
    int index='0';
    while (index<'3' && !IsEmpty(section->data.instruction.lexemeList[index-'0']))
    {
        index++;
    }
    if (index>=section->data.instruction.operandNumber)
        return 0;

    section->data.instruction.lexemeList[index-'0'] = *lexemeList;
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
                if(!IsEmpty(section.data.instruction.lexemeList[index]))
                {
                    Display(section.data.instruction.lexemeList[index]);
                }
            }
            break;
        }
        case 1:
        {
            Display(section.data.directiveValue);
            break;
        }
        case 2:
        {
            printf("\nLabel section: %17s\n", collectionSection[section.data.label.section]);
            Display(section.data.label.lexemeList);
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
    Display(collections.collection[0]);
    printf("%sDATA's Collection%s",separator,separator);
    Display(collections.collection[1]);
    printf("%sBSS's Collection%s",separator,separator);
    Display(collections.collection[2]);
    printf("%sLABELS' Collection%s",separator,separator);
    DisplayHashTable(collections.labelTable);
}

void ErasedSection(void* value)
{
    SECTION section = *(SECTION*)value;
    switch(section.dataType)
    {
        case 0:
        {
            int index;
            for (index = 0; index < 3; index++)
            {
                ErasedList(&(*(SECTION*)value).data.instruction.lexemeList[index]);
            }
            break;
        }
        case 1:
        {
            ErasedList(&(*(SECTION*)value).data.directiveValue);
            break;
        }
        case 2:
        {
            ErasedList(&(*(SECTION*)value).data.label.lexemeList);
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
        ErasedList(&(collectionLists->collection[index]));
    }

    ErasedHashTable(&(collectionLists->labelTable));
}
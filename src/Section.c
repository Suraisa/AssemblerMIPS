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

SECTION* CreateInstructionSection(COLLECTION_STATE state, unsigned long int shift, char* instructionName, int operandNumber)
{
    SECTION* section = calloc(1, sizeof(*section));
    section->state = state;
    section->shift = shift;
    section->dataType = INST;
    section->data.instruction.name = instructionName;
    section->data.instruction.operandNumber = operandNumber;

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

// SECTION* CreateLabelSection(COLLECTION_FSM stateMachine, unsigned long int shift)
// {
//     SECTION* section = calloc(1, sizeof(*section));
//     section->state = collectionSection[stateMachine.actualCollection];
//     section->shift = shift;
//     section->dataType = LABEL;

//     return section;
// }
int NumberLexemeOperand(LIST lexemeList)
{
    LIST nodeI;
    int index = 0;
    for (nodeI = lexemeList; !IsEmpty(nodeI) && ((LEXEME *)nodeI->data)->state != COMMA && ((LEXEME *)nodeI->data)->state != RETURN && ((LEXEME *)nodeI->data)->state != COMMENT; nodeI = nodeI->next)
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
            printf("Label section: %17s\n", collectionSection[section.data.section]);
            break;
        }
    }
    printf("%s\n", separator);
    printf("\n");
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

    for(index = 0; index<HASHLENGTH; index++)
    {
        ErasedList(&(collectionLists->labelTable[index]));
    }
}
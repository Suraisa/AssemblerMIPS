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

SECTION* CreateDirectiveSection(COLLECTION_STATE state, unsigned long int shift, LIST lexemeList)
{
    SECTION* section = calloc(1, sizeof(*section));
    section->state = state;
    section->shift = shift;
    section->dataType = 1;
    section->data.directiveValue = lexemeList;

    return section;
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
            int index;
            for (index = 0; index < 3; index++)
            {
                Display(section.data.instruction.lexemeList[index]);
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
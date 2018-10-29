#include "DicoInstruct.h"

int sizeDico = 0;

INSTRUCTION* AllocateDico(FILE *file, int* sizeDico)
{
    if(!fscanf(file, "%d", sizeDico))
        return 0;

    return (INSTRUCTION*)malloc(*sizeDico * sizeof(INSTRUCTION));
}

INSTRUCTION*InitializeDicoInstruct(char *nameDicoFile)
{
    FILE *file = NULL;
    int index = 0;
    INSTRUCTION* dictionary;

    if (!(file = fopen(nameDicoFile, "r")))
    {
        printf("ERROR the dictionary file doesn't exist");
        return NULL;
    }

    if(!(dictionary = AllocateDico(file, &sizeDico)))
    {
        printf("ERROR not enought memory");
        return NULL;
    }


    while(index<sizeDico && !feof(file))
    {
        fscanf(file, "%s %c %c", (dictionary[index].id), &(dictionary[index].type), &(dictionary[index].typeNumber));
        index++;
    }

    if(index != sizeDico)
    {
        printf("WARNING the dictionary's size is not the real one");
        sizeDico = index;
    }
    
    fclose(file);
    return dictionary;
}

int IndexInstruction(INSTRUCTION* dictionary, char *instructionName)
{
    int index = 0;
    int isNotEqual = 0;

    StringUpper(instructionName);

    while (index < sizeDico && (isNotEqual = strcmp(dictionary[index].id, instructionName) != 0))
    {
        index++;
    }

    if(isNotEqual)
        return -1;

    return index;
}
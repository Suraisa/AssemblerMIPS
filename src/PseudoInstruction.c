#include "PseudoInstruction.h"

int sizePseudoDico = 0;

int FindPseudoInstruction(char* nameToFind, FILE** dicoPseudoInstruction, SECTION* section)
{
    char readingChar[2];
    char* instructionName;
    int notFinded = 1;
    fscanf(*dicoPseudoInstruction, "%s", instructionName);

    while(notFinded = strcmp(nameToFind, instructionName) && !feof(*dicoPseudoInstruction))
    {
        do
        {   
            readingChar[0] = fgetc(*dicoPseudoInstruction);
            if(!feof(*dicoPseudoInstruction))
            {
                readingChar[1] = fgetc(*dicoPseudoInstruction);
            }
        }while ((!feof(*dicoPseudoInstruction)) && !(readingChar[0] == '\n' && readingChar[1] == '\n'));

        if(feof(*dicoPseudoInstruction))
            return 0;

        fscanf(*dicoPseudoInstruction, "%s", instructionName);
    }
    if(!notFinded)
    {
        readingChar[1] = fgetc(*dicoPseudoInstruction);
        do
        {
            readingChar[1] = fgetc(*dicoPseudoInstruction);
        }while(readingChar[1] != ' ');
        fscanf(*dicoPseudoInstruction, "%d", &(section->data.instruction.lowerBits));
        readingChar[1] = fgetc(*dicoPseudoInstruction);
    }
    return !notFinded;
}

PSEUDO_INSTRUCTION* AllocatePseudoDico(FILE *file, int* sizePseudoDico)
{
    if(!fscanf(file, "%d", sizePseudoDico))
        return 0;

    return (PSEUDO_INSTRUCTION*)malloc(*sizePseudoDico * sizeof(PSEUDO_INSTRUCTION));
}

PSEUDO_INSTRUCTION* InitializePseudoDicoInstruct(char *nameDicoFile)
{
    FILE *file = NULL;
    int index = 0;
    PSEUDO_INSTRUCTION* dictionary;

    if (!(file = fopen(nameDicoFile, "r")))
    {
        printf("ERROR the dictionary file doesn't exist");
        return NULL;
    }

    if(!(dictionary = AllocatePseudoDico(file, &sizePseudoDico)))
    {
        printf("ERROR not enought memory");
        return NULL;
    }

    char readingChar[2] = "  ";
    fscanf(file, "%s %s", (dictionary[index].id), &(dictionary[index].operands));
    index++;

    while(index<sizePseudoDico && !feof(file))
    {
        do
        {   
            readingChar[0] = ' ';
            readingChar[0] = readingChar[0] ^ readingChar[1];
            readingChar[1] = readingChar[0] ^ readingChar[1];
            readingChar[0] = readingChar[0] ^ readingChar[1];
            readingChar[1] = fgetc(file);
        }while ((!feof(file)) && !(readingChar[0] == '\n' && readingChar[1] == '\n'));

        if(feof(file))
            return NULL;

        fscanf(file, "%s %s", (dictionary[index].id), &(dictionary[index].operands));
        if(dictionary[index].operands[0] != '0')
        {
            dictionary[index].typeNumber = strlen(dictionary[index].operands);
        }
        else
        {
            dictionary[index].typeNumber = 0;
        }
        index++;
    }

    if(index != sizePseudoDico)
    {
        printf("WARNING the dictionary's size is not the real one");
        sizePseudoDico = index;
    }
    
    fclose(file);
    return dictionary;
}

int CreateNewListLexeme(FILE** readingFile, QUEUE_DOUBLE* lexemeQueue, QUEUE_DOUBLE* operands)
{
    char readingChar[2] = "  ";
    LEXEME_FSM lexemeStateMachine;
    InitializationLexemeFsm(&lexemeStateMachine);
    unsigned long int lineNumber = 1;
    LIST_DOUBLE ReadingValue = CreateListDouble();
    printf("\n\nLexemes' treatment:\n\n");

    if(*readingFile != NULL)
    {
        do
        {
            readingChar[0] = ' ';
            readingChar[0] = readingChar[0] ^ readingChar[1];
            readingChar[1] = readingChar[0] ^ readingChar[1];
            readingChar[0] = readingChar[0] ^ readingChar[1];
            readingChar[1] = fgetc(*readingFile);
            if(readingChar[1] == '^')
            {
                readingChar[1] = fgetc(*readingFile);
                ConcatenateListDouble(lexemeQueue, &(((SECTION*)(*operands)->data)->data.instruction.lexemeList[readingChar[1]-'1']));
            }
            else
            {
                LexemeFsm(&(readingChar[1]), lexemeQueue, &ReadingValue, &lexemeStateMachine, &lineNumber, feof(*readingFile));
            }
        }while(!feof(*readingFile) && !(readingChar[0] == '\n' && readingChar[1] == '\n'));
    }
    else
    {
        printf("\n\nThe file doesn't exit in the 'file' folder or you don't have enough memory available\n\n");
    }

    if(lexemeStateMachine.error)
        return 0;

    if(!feof(*readingFile))
    {
        ErasedAtLastDouble(lexemeQueue);
        ErasedAtLastDouble(lexemeQueue);
    }

    return 1;
}

int IndexPseudoInstruction(PSEUDO_INSTRUCTION* dictionary, char *instructionName)
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
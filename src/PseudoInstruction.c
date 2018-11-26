#include "PseudoInstruction.h"

LIST_DOUBLE FindPseudoInstruction(char* nameToFind, FILE* dicoPseudoInstruction)
{
    char readingChar;
    char* instructionName;
    fscanf(dicoPseudoInstruction, "%s", instructionName);

    while(strcmp(nameToFind, instructionName) && !feof(dicoPseudoInstruction))
    {
        do
        {   
            readingChar = fgetc(dicoPseudoInstruction);
        }while ((!feof(dicoPseudoInstruction)) && readingChar != '\n');

        if(feof(dicoPseudoInstruction))
            return NULL;

        fscanf(dicoPseudoInstruction, "%s", instructionName);
    }
}
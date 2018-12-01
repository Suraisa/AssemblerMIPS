#include "PseudoInstruction.h"

int FindPseudoInstruction(char* nameToFind, FILE* dicoPseudoInstruction)
{
    char readingChar[2];
    char* instructionName;
    int notFinded = 1;
    fscanf(dicoPseudoInstruction, "%s", instructionName);

    while(notFinded = strcmp(nameToFind, instructionName) && !feof(dicoPseudoInstruction))
    {
        do
        {   
            readingChar[0] = fgetc(dicoPseudoInstruction);
            if(!feof(dicoPseudoInstruction))
            {
                readingChar[1] = fgetc(dicoPseudoInstruction);
            }
        }while ((!feof(dicoPseudoInstruction)) && !(readingChar[0] == '\n' && readingChar[1] == '\n'));

        if(feof(dicoPseudoInstruction))
            return 0;

        fscanf(dicoPseudoInstruction, "%s", instructionName);
    }
    return !notFinded;
}


#include <stdio.h>
#include <string.h>

#include "DoubleQueueGeneric.h"
#include "Global.h"
#include "Utils.h"
#include "FSMLexeme.h"
#include "DoubleListGeneric.h"
#include "FSMOperand.h"

int main(int argc, char *argv[])
{
    FILE* readingFile;

    if (argc != 2)
    {
        char fileToRead[fileNameSize];

        printf("\n\nLaunch the 1 file as an argument or now you can:\nWrite a file's name inside the 'file' folder to use the MISP assembler on this file:\n\n");
        scanf("%s",fileToRead);

        char filePath[fileNameSize] = "files/";
        strcat(filePath, fileToRead);
        readingFile = fopen(filePath, "r");
    }
    else
    {
        char *readingFileName;
        readingFileName = argv[argc-1];
        if (NULL == readingFileName)
        {
        fprintf( stderr, "Missing ASM source file, aborting.\n" );
        return 0;
        }
        readingFile = fopen(readingFileName, "r");
    }

    char readingChar;
    LEXEME_FSM lexemeStateMachine;
    InitializationLexemeFsm(&lexemeStateMachine);
    unsigned long int lineNumber = 1;
    LIST_DOUBLE ReadingValue = CreateListDouble();
    QUEUE_DOUBLE lexemeQueue = CreateQueueDouble();

    /*
    ------------------
    Lexemes' treatment
    ------------------
    */
    printf("\n\nLexemes' treatment:\n\n");

    if(readingFile != NULL)
    {
        while ((!feof(readingFile)))
        {
        readingChar = fgetc(readingFile);
        LexemeFsm(&readingChar, &lexemeQueue, &ReadingValue, &lexemeStateMachine, &lineNumber, feof(readingFile));
        }

        if(!lexemeStateMachine.error)
        {
        DisplayDoubleList(lexemeQueue);
        }

        fclose(readingFile);
    }
    else
    {
        printf("\n\nThe file doesn't exit in the 'file' folder or you don't have enough memory available\n\n");
        return -1;
    }

    if(lexemeStateMachine.error)
        return -1;

    FSM_STATE_OPERAND fsmOperand;
    InitializationOperandFsm(&fsmOperand, BASE_OFFSET);
    OperandFSM(&fsmOperand, &lexemeQueue);

    if(!fsmOperand.error)
    {
      DisplayDoubleList(lexemeQueue);
    }

    ErasedListDouble(&lexemeQueue);
    fsmOperand.error ? printf("\nERROR !\n") : printf("\nWorks\n");
    if(fsmOperand.error)
        return -1;

    return 0;
}
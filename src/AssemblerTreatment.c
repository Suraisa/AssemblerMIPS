#include "AssemblerTreatment.h"

void OpenAssembleeFile(int argc, char *argv[], FILE** readingFile)
{
    if (argc != 2)
    {
        char fileToRead[fileNameSize];

        printf("\n\nLaunch the 1 file as an argument or now you can:\nWrite a file's name inside the 'file' folder to use the MISP assembler on this file:\n\n");
        scanf("%s",fileToRead);

        char filePath[fileNameSize] = "files/";
        strcat(filePath, fileToRead);
        *readingFile = fopen(filePath, "r");
    }
    else
    {
        char *readingFileName;
        readingFileName = argv[argc-1];
        if (NULL == readingFileName)
        {
            fprintf( stderr, "Missing ASM source file, aborting.\n" );
            return;
        }
        *readingFile = fopen(readingFileName, "r");
    }
}

int LexemePass(FILE** readingFile, QUEUE_DOUBLE* lexemeQueue)
{
    char readingChar;
    LEXEME_FSM lexemeStateMachine;
    InitializationLexemeFsm(&lexemeStateMachine);
    unsigned long int lineNumber = 1;
    LIST_DOUBLE ReadingValue = CreateListDouble();

    printf("\n\nLexemes' treatment:\n\n");

    if(*readingFile != NULL)
    {
        while ((!feof(*readingFile)))
        {
        readingChar = fgetc(*readingFile);
        LexemeFsm(&readingChar, lexemeQueue, &ReadingValue, &lexemeStateMachine, &lineNumber, feof(*readingFile));
        }

        if(!lexemeStateMachine.error)
        {
        DisplayDoubleList(*lexemeQueue);
        }

        fclose(*readingFile);
    }
    else
    {
        printf("\n\nThe file doesn't exit in the 'file' folder or you don't have enough memory available\n\n");
        return 0;
    }

    if(lexemeStateMachine.error)
        return 0;

    return 1;
}

int InstructLabelTreatment(QUEUE_DOUBLE* lexemeQueue, INSTRUCTION* dicoInstruct, LIST_DOUBLE* hash)
{
    if (IsEmptyDouble(*lexemeQueue))
        return 1;

    LIST_DOUBLE firstNode = *lexemeQueue;
    LIST_DOUBLE slider = firstNode;
    int index;
    long int value = 0;
    SECTION* section = NULL;
    LEXEME lexeme;
    char operandType = '\0';
    int error = 0;

    do
    {
        for(index = 0; index<3; index++)
        {
            operandType = dicoInstruct[((SECTION*)slider->data)->data.instruction.dicoIndex].typeNumber>index ? dicoInstruct[((SECTION*)slider->data)->data.instruction.dicoIndex].operands[index] : '\0';
            if(operandType == 'R' || operandType == 'A' || operandType == 'B')
            {
                if(!IsEmptyDouble(((SECTION*)slider->data)->data.instruction.lexemeList[index]) && ((LEXEME*)((SECTION*)slider->data)->data.instruction.lexemeList[index]->data)->state == SYMBOL)
                {
                    section = IsInHashTable(hash, ((char*)((LEXEME*)((SECTION*)slider->data)->data.instruction.lexemeList[index]->data)->value));

                    if(!section)
                    {
                            lexeme = CreateLongIntLexeme(DECIMAL, 0, ((LEXEME*)((SECTION*)slider->data)->data.instruction.lexemeList[index]->data)->lineNumber);
                            ErasedListDouble(&((SECTION*)slider->data)->data.instruction.lexemeList[index]);
                            AddInFrontDouble(&((SECTION*)slider->data)->data.instruction.lexemeList[index], &lexeme, DisplayLexeme, ErasedValueLexeme, sizeof(lexeme));
                    }
                    else
                    {                        
                        if(operandType == 'R' && operandType == 'A')
                        {
                            if(section->data.label.section == TEXT)
                            {
                                value = section->shift - 4 - ((SECTION*)slider->data)->shift;
                                lexeme = CreateLongIntLexeme(DECIMAL, value, ((LEXEME*)((SECTION*)slider->data)->data.instruction.lexemeList[index]->data)->lineNumber);
                                ErasedListDouble(&((SECTION*)slider->data)->data.instruction.lexemeList[index]);
                                AddInFrontDouble(&((SECTION*)slider->data)->data.instruction.lexemeList[index], &lexeme, DisplayLexeme, ErasedValueLexeme, sizeof(lexeme));
                            }
                            else
                            {
                                printf("\nError in Relative value at line %lu\n",((LEXEME*)((SECTION*)slider->data)->data.instruction.lexemeList[index]->data)->lineNumber);
                                error = 1;
                            }
                        }
                        else if(operandType != 'R')
                        {
                            value = section->shift;
                            lexeme = CreateLongIntLexeme(DECIMAL, value, ((LEXEME*)((SECTION*)slider->data)->data.instruction.lexemeList[index]->data)->lineNumber);
                            ErasedInFrontDouble(&((SECTION*)slider->data)->data.instruction.lexemeList[index]);
                            AddInFrontDouble(&((SECTION*)slider->data)->data.instruction.lexemeList[index], &lexeme, DisplayLexeme, ErasedValueLexeme, sizeof(lexeme));
                        }
                    }
                }
            }
        }
        slider = slider->next;
    }while(firstNode != slider);
    return !error;
}

void DataLabelTreatment(QUEUE_DOUBLE* lexemeQueue, INSTRUCTION* dicoInstruct, LIST_DOUBLE* hash)
{
    if (IsEmptyDouble(*lexemeQueue))
        return;

    LIST_DOUBLE firstNode = *lexemeQueue;
    LIST_DOUBLE slider = firstNode;
    int index;
    long int value = 0;
    SECTION* section = NULL;
    LEXEME lexeme;

    do
    {
        if(!IsEmptyDouble(((SECTION*)slider->data)->data.directiveValue) && ((LEXEME*)((SECTION*)slider->data)->data.directiveValue->data)->state == SYMBOL)
        {
            section = IsInHashTable(hash, (char*)((LEXEME*)((SECTION*)slider->data)->data.directiveValue->data)->value);

            if(!section)
            {
                lexeme = CreateLongIntLexeme(DECIMAL, 0, ((LEXEME*)((SECTION*)slider->data)->data.directiveValue->data)->lineNumber);
                ErasedListDouble(&((SECTION*)slider->data)->data.directiveValue);
                AddInFrontDouble(&((SECTION*)slider->data)->data.directiveValue, &lexeme, DisplayLexeme, ErasedValueLexeme, sizeof(lexeme));
            }
            else
            {                        
                value = section->shift;
                lexeme = CreateLongIntLexeme(DECIMAL, value, ((LEXEME*)((SECTION*)slider->data)->data.directiveValue->data)->lineNumber);
                ErasedInFrontDouble(&((SECTION*)slider->data)->data.directiveValue);
                AddInFrontDouble(&((SECTION*)slider->data)->data.directiveValue, &lexeme, DisplayLexeme, ErasedValueLexeme, sizeof(lexeme));
            }
        }
        slider = slider->next;
    }while(firstNode != slider);
}

int LabelTreatment(COLLECTION_LISTS* collections, INSTRUCTION* dicoInstruct)
{
    DataLabelTreatment(&collections->collection[DATA], dicoInstruct, collections->labelTable);
    return InstructLabelTreatment(&collections->collection[TEXT], dicoInstruct, collections->labelTable);
}
#include <stdio.h>

#include "QueueGeneric.h"
#include "Global.h"
#include "Utils.h"
#include "FSMLexeme.h"
#include "ListGeneric.h"
#include <string.h>
#include "DicoInstruct.h"

int main()
{

  FILE *readingFile;
  char readingChar;
  LEXEME_FSM stateMachine;
  InitializationLexemeFsm(&stateMachine);
  unsigned long int lineNumber = 1;
  LIST ReadingValue = CreateList();
  QUEUE lexemeQueue = CreateQueue();
  INSTRUCTION* dictionary;

  char fileToRead[fileNameSize];

  if(!(dictionary = InitializeDicoInstruct("src/DicoInstruct.txt")))
    return -1;

  printf("\n\nWrite a file's name inside the 'file' folder to use the MISP assembler on this file:\n\n");
  scanf("%s",fileToRead);

  char filePath[fileNameSize] = "files/";
  strcat(filePath, fileToRead);
  readingFile = fopen(filePath, "r");
  

  if(readingFile != NULL)
  {
    while ((!feof(readingFile)))
    {
      readingChar = fgetc(readingFile);
      LexemeFsm(&readingChar, &lexemeQueue, &ReadingValue, &stateMachine, &lineNumber, feof(readingFile));
    }

    if(!stateMachine.error)
    {
      Display(lexemeQueue);
    }

    fclose(readingFile);
    ErasedQueue(&lexemeQueue);
  }
  else
  {
    printf("\n\nThe file doesn't exit in the 'file' folder or you don't have enough memory available\n\n");
  }

  free(dictionary);

  if(stateMachine.error)
    return -1;

  return 0;
}
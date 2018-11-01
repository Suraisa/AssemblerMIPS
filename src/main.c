#include <stdio.h>

#include "QueueGeneric.h"
#include "Global.h"
#include "Utils.h"
#include "FSMLexeme.h"
#include "ListGeneric.h"
#include <string.h>
#include "DicoInstruct.h"
#include "FSMCollection.h"

int main()
{

  FILE *readingFile;
  char readingChar;
  LEXEME_FSM lexemeStateMachine;
  InitializationLexemeFsm(&lexemeStateMachine);
  unsigned long int lineNumber = 1;
  LIST ReadingValue = CreateList();
  QUEUE lexemeQueue = CreateQueue();

  char fileToRead[fileNameSize];

  printf("\n\nWrite a file's name inside the 'file' folder to use the MISP assembler on this file:\n\n");
  scanf("%s",fileToRead);

  char filePath[fileNameSize] = "files/";
  strcat(filePath, fileToRead);
  readingFile = fopen(filePath, "r");
  

/*
Lexemes' treatment
*/

  if(readingFile != NULL)
  {
    while ((!feof(readingFile)))
    {
      readingChar = fgetc(readingFile);
      LexemeFsm(&readingChar, &lexemeQueue, &ReadingValue, &lexemeStateMachine, &lineNumber, feof(readingFile));
    }

    if(!lexemeStateMachine.error)
    {
      Display(lexemeQueue);
    }

    fclose(readingFile);
  }
  else
  {
    printf("\n\nThe file doesn't exit in the 'file' folder or you don't have enough memory available\n\n");
  }

  if(lexemeStateMachine.error)
    return -1;

/*
Collections' treatment
*/

  INSTRUCTION* dictionary;
  COLLECTION_FSM collectionStateMachine;
  COLLECTION_LISTS collections;

  if(!(dictionary = InitializeDicoInstruct("src/DicoInstruct.txt")))
    return -1;

  InitializationCollectionFsm(&collectionStateMachine);

  while(!IsEmpty(lexemeQueue))
  {
    CollectionFsm(&collectionStateMachine, &lexemeQueue, &collections);
  }

  free(dictionary);
  ErasedQueue(&lexemeQueue);

  if(lexemeStateMachine.error)
    return -1;


  return 0;
}
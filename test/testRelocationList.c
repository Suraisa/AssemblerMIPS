# include "RelocationTable.h"

#include <stdio.h>

#include "DoubleQueueGeneric.h"
#include "Global.h"
#include "Utils.h"
#include "FSMLexeme.h"
#include "DoubleListGeneric.h"
#include <string.h>
#include "DicoInstruct.h"
#include "FSMCollection.h"

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

/*
----------------------
Collections' treatment
----------------------
*/

  printf("\n\nCollections' treatment:\n\n");

  INSTRUCTION* dictionary;
  COLLECTION_FSM collectionStateMachine;
  COLLECTION_LISTS collections;

  if(!(dictionary = InitializeDicoInstruct("src/DicoInstruct.txt")))
  {
    printf("\n\nYou don't have enough memory available for the dictionary.\n\n");
    return -1;
  }

  if(!(InitializationCollection(&collections)))
  {
    printf("\n\nYou don't have enough memory available for the hashTable.\n\n");
    return -1;
  }

  InitializationCollectionFsm(&collectionStateMachine);

  while(!IsEmptyDouble(lexemeQueue))
  {
    CollectionFsm(&collectionStateMachine, &lexemeQueue, &collections, dictionary);
  }

  if(!collectionStateMachine.error)
  {
    DisplayCollectionLists(collections);
  }

  /*
  ----------------------
  Test of relocation's tables
  ----------------------
  */
  RELOCATIONTABLE relocationTable=CreateRelocationList();
  if (!relocationList)
    return -1;

  collections.collection[TEXT];
  COLLECTIONS symbolSection=TEXT;
  unsigned long int relativeAddress=12;
  TYPE typeRMIPS=R_MIPS_HI16;
  LEXEME** symbolAddress=&((SECTION*)collections.collection[TEXT]->data)->data.instruction.lexemeList[2];

  FillRelocationList(&relocationTable.relocationText, symbolSection, relativeAddress, typeRMIPS, symbolAddress);
  ErasedListDouble(relocationTable.relocationText);
  //DisplayRelocationTable(relocationTable);

  //UpdateRelocationText(&relocationList, &((SECTION*)collections.collection[TEXT]->data));

  free(dictionary);
  ErasedQueueDouble(&lexemeQueue);
  ErasedCollectionLists(&collections);

  return 0;
}

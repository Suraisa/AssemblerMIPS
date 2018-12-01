#include <stdio.h>

#include "DoubleQueueGeneric.h"
#include "Global.h"
#include "Utils.h"
#include "FSMLexeme.h"
#include "DoubleListGeneric.h"
#include <string.h>
#include "DicoInstruct.h"
#include "FSMCollection.h"
#include "RelocationTable.h"
#include "AssemblerTreatment.h"

int main(int argc, char *argv[])
{
  FILE* readingFile;
  
  OpenAssembleeFile(argc, argv, &readingFile);
  
  if(!readingFile)
    return 0;

  QUEUE_DOUBLE lexemeQueue = CreateQueueDouble();

/*
------------------
Lexemes' treatment
------------------
*/

  if(!LexemePass(&readingFile, &lexemeQueue))
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

  if(collectionStateMachine.error)
  {
    free(dictionary);
    ErasedQueueDouble(&lexemeQueue);
    ErasedCollectionLists(&collections);
    return -1;
  }

/*
-----------------------
Rallocations' treatment
-----------------------
*/

  RELOCATIONTABLE relocationTable = CreateRelocationTable();

  UpdateRelocationTable(&relocationTable, collections.labelTable, &collections, dictionary);  

  DisplayRelocationTable(relocationTable);

  ErasedListDouble(&relocationTable.relocationText);

  free(dictionary);
  ErasedQueueDouble(&lexemeQueue);
  ErasedCollectionLists(&collections);

  return 0;
}
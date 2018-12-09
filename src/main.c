#include <stdio.h>

#include <pelf/pelf.h>
#include <pelf/section.h>

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
#include "PseudoInstruction.h"
#include "BitTreatment.h"

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
  {
    ErasedListDouble(&lexemeQueue);
    return 1;
  }

/*
----------------------
Collections' treatment
----------------------
*/

  printf("\n\nCollections' treatment:\n\n");

  INSTRUCTION* dictionary;
  PSEUDO_INSTRUCTION* pseudoDictionary;
  COLLECTION_FSM collectionStateMachine;
  COLLECTION_LISTS collections;

  if(!(dictionary = InitializeDicoInstruct("src/DicoInstruct.txt")))
  {
    printf("\n\nYou don't have enough memory available for the dictionary.\n\n");
    return 1;
  }

  if(!(pseudoDictionary = InitializePseudoDicoInstruct("src/DicoPseudoInstruct.txt")))
  {
    printf("\n\nYou don't have enough memory available for the dictionary.\n\n");
    return 1;
  }

  if(!(InitializationCollection(&collections)))
  {
    printf("\n\nYou don't have enough memory available for the hashTable.\n\n");
    return 1;
  }

  InitializationCollectionFsm(&collectionStateMachine);

  while(!IsEmptyDouble(lexemeQueue))
  {
    CollectionFsm(&collectionStateMachine, &lexemeQueue, &collections, dictionary,pseudoDictionary);
  }

  if(!collectionStateMachine.error)
  {
    // DisplayCollectionLists(collections);  
  }

  if(collectionStateMachine.error)
  {
    free(dictionary);
    free(pseudoDictionary);
    ErasedQueueDouble(&lexemeQueue);
    ErasedCollectionLists(&collections);
    return 1;
  }
  int index = 0;

/*
-----------------------
Rallocations' treatment
-----------------------
*/
  free(pseudoDictionary);

  RELOCATIONTABLE relocationTable = CreateRelocationTable();

  UpdateRelocationTable(&relocationTable, collections.labelTable, &collections, dictionary);

  //DisplayRelocationTable(relocationTable);
  
  LabelTreatment(&collections, dictionary);
  DisplayCollectionLists(collections);

  SECTION_FIELD field = BitInstructionTreatment(dictionary, collections.collection[TEXT]);
  free(field.bitField);

  free(dictionary);
  ErasedCollectionLists(&collections);

  ErasedRelocationTable(&relocationTable);

  ErasedQueueDouble(&lexemeQueue);

  return 0;
}
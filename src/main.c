#include <stdio.h>

#include <pelf/section.h>
#include "make_mips_elf.h"

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

  INSTRUCTION* dictionary;
  PSEUDO_INSTRUCTION* pseudoDictionary;
  COLLECTION_FSM collectionStateMachine;
  COLLECTION_LISTS collections;

  if(!CollectionPass(&dictionary, &pseudoDictionary, &lexemeQueue, &collectionStateMachine, &collections))
    return 1;
  

/*
-----------------------
Rallocations' treatment
-----------------------
*/
  free(pseudoDictionary);

  RELOCATIONTABLE relocationTable = CreateRelocationTable();

  UpdateRelocationTable(&relocationTable, collections.labelTable, &collections, dictionary);

  DisplayRelocationTable(relocationTable);
  
  LabelTreatment(&collections, dictionary);
  DisplayCollectionLists(collections);

/*
-------------------
Bit field treatment
-------------------
*/

  SECTION_FIELD field = BitInstructionTreatment(dictionary, collections.collection[TEXT]);

  section     text = NULL;
  section     data = NULL;
  section      bss = NULL;
  section shstrtab = NULL;
  section   strtab = NULL;
  section   symtab = NULL;
  section  reltext = NULL;
  section  reldata = NULL;

  shstrtab = make_shstrtab_section();

  text = make_text_section((int*)field.bitField, field.size);

    if ( !text ) {
        fprintf( stderr, "Unable to write .text section (missing information).\n" );
        return -1;
    }

  print_section(shstrtab);
  print_section( text );

  del_section( shstrtab );
  del_section( text );


  free(dictionary);
  
  free(field.bitField);

  ErasedCollectionLists(&collections);

  ErasedRelocationTable(&relocationTable);

  ErasedQueueDouble(&lexemeQueue);

  return 0;
}
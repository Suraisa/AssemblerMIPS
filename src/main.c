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
#include "SymbolTreatment.h"

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
  
  free(pseudoDictionary);
/*
-----------------------
Rallocations' treatment
-----------------------
*/

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
  bss = make_bss_section((int)BitBssTreatment(collections.collection[BSS]));

  text = make_text_section((int*)field.bitField, field.size);
  // RELOC_TAB textReloc = CreateRelocTab(relocationTable.relocationText, symtab, shstrtab, strtab);
  // RELOC_TAB dataReloc = CreateRelocTab(relocationTable.relocationData, symtab, shstrtab, strtab);

  // reltext  = make_rel32_section( ".rel.text", textReloc.table,textReloc.size);
  // reldata  = make_rel32_section( ".rel.data", dataReloc.table,dataReloc.size);

  if (!text)
  {
    fprintf( stderr, "Unable to write .text section (missing information).\n" );
    return -1;
  }

  print_section(shstrtab);
  print_section(text);
  print_section(bss);
  // print_section(reltext);
  // print_section(reldata);

  del_section(shstrtab);
  del_section(text);
  del_section(bss);
  // del_section(reltext);
  // del_section(reldata);

  free(dictionary);
  
  free(field.bitField);
  // free(textReloc.table);
  // free(dataReloc.table);

  ErasedCollectionLists(&collections);

  ErasedRelocationTable(&relocationTable);

  ErasedQueueDouble(&lexemeQueue);

  return 0;
}
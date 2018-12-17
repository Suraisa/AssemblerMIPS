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
  LIST_DOUBLE allLabel;

  if(!CollectionPass(&dictionary, &pseudoDictionary, &lexemeQueue, &collectionStateMachine, &collections, &allLabel))
    return 1;

  free(pseudoDictionary);
/*
-----------------------
Rallocations' treatment
-----------------------
*/

  RELOCATIONTABLE relocationTable = CreateRelocationTable();

  UpdateRelocationTable(&relocationTable, collections.labelTable, &collections, dictionary, &allLabel);

  // DisplayRelocationTable(relocationTable);

/*
-------------------
BitinstructField treatment
-------------------
*/
ALL_ELF_TABLE tables;
InitializationAllElfTable(tables);

  tables.shstrtab = make_shstrtab_section();

  char** allSymbolTab = CreateSymStrTab(allLabel, &tables.symtab, tables.shstrtab, &tables.strtab);
  int numberOfElement = SizeListDouble(allLabel);

  LabelTreatment(&collections, dictionary);
  //DisplayCollectionLists(collections);

  SECTION_FIELD instructField = BitInstructionTreatment(dictionary, collections.collection[TEXT], collectionStateMachine.nextShift[TEXT]);
  SECTION_FIELD dataField = BitDataTreatment(collections.collection[DATA], collectionStateMachine.nextShift[DATA]);

  tables.bss = make_bss_section((int)collectionStateMachine.nextShift[BSS]);

  tables.text = make_text_section((int*)instructField.bitField, instructField.size);
  tables.data = make_data_section((int*)dataField.bitField, collectionStateMachine.nextShift[DATA]);
  RELOC_TAB textReloc = CreateRelocTab(relocationTable.relocationText, tables.symtab, tables.shstrtab, tables.strtab, allSymbolTab);
  RELOC_TAB dataReloc = CreateRelocTab(relocationTable.relocationData, tables.symtab, tables.shstrtab, tables.strtab, allSymbolTab);

  tables.reltext  = make_rel32_section( ".rel.text", textReloc.table,textReloc.size);
  tables.reldata  = make_rel32_section( ".rel.data", dataReloc.table,dataReloc.size);

  if (!tables.text)
  {
    fprintf( stderr, "Unable to write .text section (missing information).\n" );
    return -1;
  }

  char* machine = "mips";
  char* name = ChangeExtension(argv[argc-1], ".o");
  int noreorder = 1;

  elf_write_relocatable( name, machine, noreorder,
                           tables.text->start, tables.text->sz,
                           tables.data->start, tables.data->sz,
                           tables.bss->start, tables.bss->sz,
                           tables.shstrtab->start, tables.shstrtab->sz,
                           tables.strtab->start, tables.strtab->sz,
                           tables.symtab->start, tables.symtab->sz,
                           tables.reltext->start, tables.reltext->sz,
                           tables.reldata->start, tables.reldata->sz);

  //DisplayAllSections(tables);
  CleanTheProject(tables, allSymbolTab, numberOfElement, dictionary, name, instructField, dataField, textReloc, dataReloc, allLabel, collections, relocationTable, lexemeQueue);

  return 0;
}

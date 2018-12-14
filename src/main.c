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

  DisplayRelocationTable(relocationTable);

/*
-------------------
BitinstructField treatment
-------------------
*/

  section     text = NULL;
  section     data = NULL;
  section      bss = NULL;
  section shstrtab = NULL;
  section   strtab = NULL;
  section   symtab = NULL;
  section  reltext = NULL;
  section  reldata = NULL;

  shstrtab = make_shstrtab_section();

  char** allSymbolTab = CreateSymStrTab(allLabel, &symtab, shstrtab, &strtab);
  int numberOfElement = SizeListDouble(allLabel);

  LabelTreatment(&collections, dictionary);
  // DisplayCollectionLists(collections);

  SECTION_FIELD instructField = BitInstructionTreatment(dictionary, collections.collection[TEXT], collectionStateMachine.nextShift[TEXT]);
  SECTION_FIELD dataField = BitDataTreatment(collections.collection[DATA], collectionStateMachine.nextShift[DATA]);

  bss = make_bss_section((int)collectionStateMachine.nextShift[BSS]);

  text = make_text_section((int*)instructField.bitField, instructField.size);
  data = make_data_section((int*)dataField.bitField, collectionStateMachine.nextShift[DATA]);
  RELOC_TAB textReloc = CreateRelocTab(relocationTable.relocationText, symtab, shstrtab, strtab, allSymbolTab);
  RELOC_TAB dataReloc = CreateRelocTab(relocationTable.relocationData, symtab, shstrtab, strtab, allSymbolTab);

  reltext  = make_rel32_section( ".rel.text", textReloc.table,textReloc.size);
  reldata  = make_rel32_section( ".rel.data", dataReloc.table,dataReloc.size);

  if (!text)
  {
    fprintf( stderr, "Unable to write .text section (missing information).\n" );
    return -1;
  }

  char* machine = "mips";
  char* name = ChangeExtension(argv[argc-1], ".o");
  int noreorder = 1;

  elf_write_relocatable( name, machine, noreorder,
                           text->start, text->sz,
                           data->start, data->sz,
                           bss->start, bss->sz,
                           shstrtab->start, shstrtab->sz,
                           strtab->start, strtab->sz,
                           symtab->start, symtab->sz,
                           reltext->start, reltext->sz,
                           reldata->start, reldata->sz);

  // print_section(shstrtab);
  // print_section(text);
  // print_section(data);
  // print_section(bss);
  // print_section(strtab);
  // print_section(symtab);
  // print_section(reltext);
  // print_section(reldata);

  del_section(shstrtab);
  del_section(text);
  del_section(data);
  del_section(bss);
  del_section(strtab);
  del_section(symtab);
  del_section(reltext);
  del_section(reldata);

  ErasedTableString(allSymbolTab, numberOfElement);

  free(dictionary);
  free(name);
  
  free(instructField.bitField);
  free(dataField.bitField);
  free(textReloc.table);
  free(dataReloc.table);

  ErasedListDouble(&allLabel);

  ErasedCollectionLists(&collections);

  ErasedRelocationTable(&relocationTable);

  ErasedQueueDouble(&lexemeQueue);

  return 0;
}
#ifndef ASSEMBLER_TREATMENT_H
#define ASSEMBLER_TREATMENT_H

#include <stdio.h>
#include <stdlib.h>

#include "Global.h"
#include "DoubleListGeneric.h"
#include "FSMLexeme.h"
#include "Section.h"
#include "Lexeme.h"

typedef struct
{
    section text;
    section data;
    section bss;
    section shstrtab;
    section strtab;
    section symtab;
    section reltext;
    section reldata;
} ALL_ELF_TABLE;

void OpenAssembleeFile(int argc, char *argv[], FILE** readingFile);

int LexemePass(FILE** readingFile, QUEUE_DOUBLE* lexemeQueue);

int CollectionPass(INSTRUCTION** dictionary, PSEUDO_INSTRUCTION** pseudoDictionary, LIST_DOUBLE* lexemeQueue, COLLECTION_FSM* collectionStateMachine, COLLECTION_LISTS* collections, LIST_DOUBLE* allSymbol);

int InstructLabelTreatment(QUEUE_DOUBLE* lexemeQueue, INSTRUCTION* dicoInstruct, LIST_DOUBLE* hash);

void DataLabelTreatment(QUEUE_DOUBLE* lexemeQueue, INSTRUCTION* dicoInstruct, LIST_DOUBLE* hash);

int LabelTreatment(COLLECTION_LISTS* lexemeQueue, INSTRUCTION* dicoInstruct);

#define InitializationAllElfTable(elfTable)\
{\
    elfTable.text = NULL;\
    elfTable.data = NULL;\
    elfTable.bss = NULL;\
    elfTable.shstrtab = NULL;\
    elfTable.strtab = NULL;\
    elfTable.symtab = NULL;\
    elfTable.reltext = NULL;\
    elfTable.reldata = NULL;\
}\

#define ErasedAllSections(elfTable)\
{\
    del_section(elfTable.shstrtab);\
    del_section(elfTable.text);\
    del_section(elfTable.data);\
    del_section(elfTable.bss);\
    del_section(elfTable.strtab);\
    del_section(elfTable.symtab);\
    del_section(elfTable.reltext);\
    del_section(elfTable.reldata);\
}\

#define DisplayAllSections(elfTable)\
{\
    print_section(elfTable.shstrtab);\
    print_section(elfTable.text);\
    print_section(elfTable.data);\
    print_section(elfTable.bss);\
    print_section(elfTable.strtab);\
    print_section(elfTable.symtab);\
    print_section(elfTable.reltext);\
    print_section(elfTable.reldata);\
}\

#define CleanTheProject(elfTable, allSymbolTab, numberElement, instructDictionary, binaryName, instructField, dataField, textReloc, dataReloc, allLabel, collections, relocationTable, lexemeQueue)\
{\
    ErasedAllSections(elfTable);\
    ErasedTableString(allSymbolTab, numberElement);\
    free(instructDictionary);\
    free(binaryName);\
    free(instructField.bitField);\
    free(dataField.bitField);\
    free(textReloc.table);\
    free(dataReloc.table);\
    ErasedListDouble(&allLabel);\
    ErasedCollectionLists(&collections);\
    ErasedRelocationTable(&relocationTable);\
    ErasedQueueDouble(&lexemeQueue);\
}\

#endif
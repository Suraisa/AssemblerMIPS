#ifndef SYMBOL_TREATMENT_H
#define SYMBOL_TREATMENT_H

#include <pelf/section.h>
#include "make_mips_elf.h"
#include "DoubleListGeneric.h"
#include "RelocationTable.h"
#include "Section.h"

typedef struct
{
    void* symbolSection;
    int undef;
}SYM_TREATMENT;

typedef struct
{
    unsigned long int shift;
    int undef;
    COLLECTIONS sectionType;
}SYM_ELEMENT;

typedef struct
{
    char* table;
    unsigned long int size;
}STR_TAB;

typedef struct
{
    Elf32_Rel* table;
    unsigned long int size;
}RELOC_TAB;

RELOC_TAB CreateRelocTab(LIST_DOUBLE relocList, section symtab, section shstrtab, section strtab, char** allSymbol);

SYM_ELEMENT CreateSymbolElement(unsigned long int shift, int undef, COLLECTIONS type);

Elf32_Sym* CreateSymbol(section strtab, section shstrtab, SYM_ELEMENT* element, unsigned long int size, char** allSymbols);

char** CreateSymStrTab(LIST_DOUBLE symbolSection, section* symtab, section shstrtab, section* strtab);

#endif
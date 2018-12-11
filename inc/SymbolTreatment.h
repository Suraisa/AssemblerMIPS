#ifndef SYMBOL_TREATMENT_H
#define SYMBOL_TREATMENT_H

#include <pelf/section.h>
#include "make_mips_elf.h"
#include "DoubleListGeneric.h"
#include "RelocationTable.h"
#include "Section.h"

typedef struct
{
    Elf32_Sym* table;
    unsigned long int size;
}SYM_TAB;

typedef struct
{
    Elf32_Rel* table;
    unsigned long int size;
}RELOC_TAB;

RELOC_TAB CreateRelocTab(LIST_DOUBLE relocList, section symtab, section shstrtab, section strtab);

#endif
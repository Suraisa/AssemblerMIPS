#ifndef MAKE_MIPS_ELF_H
#define MAKE_MIPS_ELF_H

#include <stdlib.h>
#include <string.h>

#include <pelf/pelf.h>
#include <pelf/section.h>

int elf_get_sym_index_from_name(section symtab, section shstrtab, section strtab, char* sym_name);

#define add_string_to_table( section, symbol ) /* Explicitly add terminal '\0' */ \
  write_section( section, (unsigned char *)symbol, 1+strlen(symbol), section->sz)

section make_shstrtab_section();

section make_strtab_section( char* symbols[], int nb_sym);

section make_data_section( int data_prog[], int nb_data);

section make_text_section( int instructions[], int nb_inst);

section make_bss_section( int bss_prog);

section make_symtab_section(section shstrtab, section strtab,  Elf32_Sym symbols[], int nb_syms);

section make_rel32_section(char *relname, Elf32_Rel relocations[], int nb_reloc);

#endif
#include "SymbolTreatment.h"

RELOC_TAB CreateRelocTab(LIST_DOUBLE relocList, section symtab, section shstrtab, section strtab)
{
    if(IsEmptyDouble(relocList))
        return (RELOC_TAB){.size = 0, .table = NULL};

    RELOC_TAB relocTable;
    relocTable.size = SizeListDouble(relocList);
    relocTable.table = malloc(relocTable.size*sizeof(relocTable.table));

    if(relocTable.table == NULL)
        return (RELOC_TAB){.size = 0, .table = NULL};

    LIST_DOUBLE firstNode = relocList;
    LIST_DOUBLE slider = firstNode;
    LINKRELOCATION* relocElement;
    int index = 0;

    do
    {
        relocElement = (LINKRELOCATION*)slider->data;
        relocTable.table[index].r_offset = relocElement->relativeAddress;
        relocTable.table[index].r_info = ELF32_R_INFO(elf_get_sym_index_from_name(symtab, shstrtab, strtab, collectionSection[relocElement->symbolSection]),relocElement->typeRMIPS);
        index++;
        slider = slider->next;
    }while(slider != firstNode);

    return relocTable;
} 

SYM_ELEMENT CreateSymbolElement(unsigned long int shift, int undef, COLLECTIONS type)
{
    SYM_ELEMENT symbol;
    symbol.shift = shift;
    symbol.undef = undef;
    symbol.sectionType = type;
    return symbol;
}

Elf32_Sym* CreateSymbol(section strtab, section shstrtab, SYM_ELEMENT* element, unsigned long int size, char** allSymbols)
{
    int index;
    int type;
    Elf32_Sym* symTableElements = malloc(size*sizeof(Elf32_Sym));
    for (index = 0; index<size; index++)
    {
        symTableElements[index].st_name = elf_get_string_offset( strtab->start, strtab->sz, allSymbols[index] );
        symTableElements[index].st_size = 0;
        symTableElements[index].st_value = element[index].shift;
        type = element[index].undef ? STB_GLOBAL : STB_LOCAL;
        symTableElements[index].st_info = ELF32_ST_INFO( type, STT_NOTYPE );
        symTableElements[index].st_other = 0;
        symTableElements[index].st_shndx = elf_get_string_index( shstrtab->start, shstrtab->sz, collectionSection[element[index].sectionType] );
    }
    return symTableElements;
}

void CreateSymStrTab(LIST_DOUBLE symbolSection, section* symtab, section shstrtab, section* strtab)
{
    if(IsEmptyDouble(symbolSection))
        return;

    LIST_DOUBLE firstNode = symbolSection;
    LIST_DOUBLE slider = firstNode;
    int numberOfElement = SizeListDouble(firstNode);
    char** table = malloc(numberOfElement*sizeof(*table));
    int index = 0;
    int indexOperand;
    int sectionUndef;
    SYM_ELEMENT* syms = malloc(sizeof(SYM_ELEMENT)*numberOfElement);
    COLLECTION_DATA sectionData;
    do
    {
        if(((SYM_TREATMENT*)slider->data)->undef)
        {
            sectionUndef = ((SECTION*)((SYM_TREATMENT*)slider->data)->symbolSection)->dataType;
            sectionData = ((SECTION*)((SYM_TREATMENT*)slider->data)->symbolSection)->data;
            if(sectionUndef == TEXT)
            {
                indexOperand = 0;
                while(((LEXEME*)sectionData.instruction.lexemeList[indexOperand]->data)->state != SYMBOL)
                {
                    indexOperand++;
                }
                table[index] = (char*)((LEXEME*)sectionData.instruction.lexemeList[indexOperand]->data)->value;
            }
            else
            {
                table[index] = (char*)((LEXEME*)sectionData.directiveValue->data)->value;
            }
            syms[index] = CreateSymbolElement(0, 1, UNDEF);
        }
        else
        {
            syms[index] = CreateSymbolElement(((SECTION*)((SYM_TREATMENT*)slider->data)->symbolSection)->shift, 0, ((SECTION*)((SYM_TREATMENT*)slider->data)->symbolSection)->data.label.section);
            table[index] = (char*)((LEXEME*)((SECTION*)((SYM_TREATMENT*)slider->data)->symbolSection)->data.label.lexemeList->data)->value;
        }
        index++;
        slider = slider->next;
    }while(slider != firstNode);
    *strtab = make_strtab_section(table, numberOfElement);
    Elf32_Sym* elf32Sym = CreateSymbol(*strtab, shstrtab, syms, numberOfElement, table);
    *symtab = make_symtab_section(shstrtab, *strtab, elf32Sym, numberOfElement);
    free(elf32Sym);
    free(table);
    free(syms);
}
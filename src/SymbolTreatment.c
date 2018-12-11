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
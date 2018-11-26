# include "RelocationTable.h"

char *definedR_MIPS[7] =
    {"",
     "",
     "R_MIPS_32",
     "",
     "R_MIPS_26",
     "R_MIPS_HI16",
     "R_MIPS_LO16"};

RELOCATIONTABLE CreateRelocationList(){
  LIST_DOUBLE relocationText;
  relocationText=CreateListDouble();

  LIST_DOUBLE relocationData;
  relocationData=CreateListDouble();

  RELOCATIONTABLE relocationTable;
  relocationTable.relocationText=relocationText;
  relocationTable.relocationData=relocationData;
  return relocationTable;
};

void FillRelocationList(LIST_DOUBLE* relocationList, COLLECTIONS symbolSection, unsigned int relativeAddress, TYPE typeRMIPS, LEXEME* symbolAddress){
  LINKRELOCATION* relocation = calloc(1, sizeof(*relocation));
  relocation->symbolSection=symbolSection;
  relocation->relativeAddress=relativeAddress;
  relocation->typeRMIPS=typeRMIPS;
  relocation->symbolAddress=symbolAddress;
  AddAtLastDouble(relocationList, relocation, void (*display)(void *), NULL, sizeof(*relocation))
};

void DisplayRelocationList(LIST_DOUBLE relocationList){
  LIST_DOUBLE nodeI = relocationList;
  LIST_DOUBLE firstNode = relocationList;

  if(!IsEmptyDouble(nodeI))
  {
    do
    {
      printf("Section where the symbol is defined : %s\n", *collectionSection[relocationList.symbolSection]);
      printf("Relative address : %u\n", relocation.relativeAddress);
      printf("Type : %s\n", *definedR_MIPS[relocationList.typeRMIPS]);
      DisplayCollectionLists(relocation->symbolAddress);

      nodeI = nodeI->next;
    }while(nodeI != firstNode);
  }

  printf("\n%s", separator);
  printf("\n");
}

void DisplayRelocationTable(RELOCATIONTABLE relocationTable)
{
    char *separator = "--------------------------------";
    printf("\n");
    printf("%s\n", separator);
    printf("Tables de relocation : \n");
    printf("%s\n", separator);

    printf("--> Text's table de relocation : \n");
    DisplayRelocationList(relocationTable.relocationText);

    printf("--> Data's table de relocation : \n");
    DisplayRelocationList(relocationTable.relocationData);
}

void UpdateRelocationList(LIST_DOUBLE* relocationList, COLLECTION_LISTS collections){
  section->state = state;
  section->shift = shift;
  section->dataType = INST;
  section->data.instruction.name = instructionName;
  section->data.instruction.dicoIndex = dicoIndex;
  section->data.instruction.lineNumber = lineNumber;
}

void UpdateRelocationTable(RELOCATIONTABLE relocationTable, SECTION section){
  UpdateRelocationList(relocationTable->relocationText, collections);
  UpdateRelocationList(relocationTable->relocationData, collections);

  DisplayRelocationTable(relocationTable);

  ErasedRelocationTable(&relocationTable);
}

void ErasedRelocationTable(RELOCATIONTABLE* relocationTable){
  ErasedListDouble(relocationTable->relocationText);
  ErasedListDouble(relocationTable->relocationData);
};

# include "RelocationTable.h"

char *definedR_MIPS[7] =
    {"ERROR",
     "",
     "R_MIPS_32",
     "",
     "R_MIPS_26",
     "R_MIPS_HI16",
     "R_MIPS_LO16"};

RELOCATIONTABLE CreateRelocationTable(){
  LIST_DOUBLE relocationText;
  relocationText=CreateListDouble();

  LIST_DOUBLE relocationData;
  relocationData=CreateListDouble();

  RELOCATIONTABLE relocationTable;
  relocationTable.relocationText=relocationText;
  relocationTable.relocationData=relocationData;
  return relocationTable;
}

void FillRelocationList(LIST_DOUBLE* relocationList, COLLECTIONS symbolSection, unsigned long int relativeAddress, TYPE typeRMIPS, LEXEME** symbolAddress){
  LINKRELOCATION* relocation = calloc(1, sizeof(*relocation));
  relocation->symbolSection=symbolSection;
  relocation->relativeAddress=relativeAddress;
  relocation->typeRMIPS=typeRMIPS;
  relocation->symbolAddress=*symbolAddress;
  AddAtLastDouble(relocationList, relocation, DisplayRelocation, NULL, sizeof(*relocation));
}

void DisplayRelocation(void* value){

  char *separator = "--------------------------------";

  printf("\n%s", separator);

  printf("Section where the symbol is defined : %s\n", collectionSection[((LINKRELOCATION*)value)->symbolSection]);
  printf("Relative adress : %lu\n", ((LINKRELOCATION*)value)->relativeAddress);
  printf("Type : %s\n", definedR_MIPS[((LINKRELOCATION*)value)->typeRMIPS]);
  printf("Address of the symbol : %p",((LINKRELOCATION*)value)->symbolAddress);

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
    DisplayDoubleList(relocationTable.relocationText);

    printf("--> Data's table de relocation : \n");
    DisplayDoubleList(relocationTable.relocationData);
}

void UpdateRelocationText(LIST_DOUBLE* relocationList, SECTION** section, LIST_DOUBLE **hash){
  int index;
  for (index=0 ; index<3 ; index++){
    if ((**section).data.instruction.lexemeList[index]->data){
      if (((LEXEME*)((**section).data.instruction.lexemeList[index]->data)->state)==SYMBOL){
        SECTION* dataSection = IsInHashTable(hash,*(char**)(((LEXEME*)(**section).data.instruction.lexemeList[index]->data)->value));
        if (!dataSection){
          FillRelocationList(relocationList, BSS, 0, 0, section);
        }
        else{
          FillRelocationList(relocationList, dataSection->data.label.section, dataSection->shift, R_MIPS_26, section);
        }
      }
    }
  }
}

void UpdateRelocationData(LIST_DOUBLE* relocationList, SECTION** section, LIST_DOUBLE **hash){
  if ((**section).data.directiveValue->data){
    if (((LEXEME*)((**section).data.directiveValue->data)->state)==SYMBOL){
      SECTION* dataSection = IsInHashTable(hash,*(char**)(((LEXEME*)(**section).data.directiveValue->data)->value));
      if (!dataSection){
        FillRelocationList(relocationList, BSS, 0, 0, section);
      }
      else{
        FillRelocationList(relocationList, dataSection->data.label.section, dataSection->shift, R_MIPS_32, section);
      }
    }
  }
}

void UpdateRelocationTable(RELOCATIONTABLE relocationTable, LIST_DOUBLE **hash, SECTION** section){
  switch((**section).dataType)
  {
    case TEXT :
      UpdateRelocationText(&relocationTable.relocationText, section, hash);
    case DATA :
      UpdateRelocationText(&relocationTable.relocationData, section, hash);
    default :
      printf("\nError, not BSS for relocation's table\n");
  }
}

void ErasedRelocationTable(RELOCATIONTABLE* relocationTable){
  ErasedListDouble(&relocationTable->relocationText);
  ErasedListDouble(&relocationTable->relocationData);
}

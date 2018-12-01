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
  RELOCATIONTABLE relocationTable;
  relocationTable.relocationText=CreateListDouble();
  relocationTable.relocationData=CreateListDouble();
  return relocationTable;
}

void FillRelocationList(LIST_DOUBLE* relocationList, COLLECTIONS symbolSection, unsigned long int relativeAddress, MIPS_TYPE typeRMIPS, LEXEME** symbolAddress){
  LINKRELOCATION relocation;
  relocation.symbolSection=symbolSection;
  relocation.relativeAddress=relativeAddress;
  relocation.typeRMIPS=typeRMIPS;
  relocation.symbolAddress=*symbolAddress;
  AddAtLastDouble(relocationList, &relocation, DisplayRelocation, NULL, sizeof(relocation));
}

void DisplayRelocation(void* value){

  char *separator = "--------------------------------";

  printf("\n%s", separator);

  printf("\nSection where the symbol is defined : %s\n", collectionSection[((LINKRELOCATION*)value)->symbolSection]);
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
    printf("Relocation's Tables : \n");
    printf("%s\n", separator);

    printf("--> Text's table de relocation : \n");
    DisplayDoubleList(relocationTable.relocationText);

    printf("--> Data's table de relocation : \n");
    DisplayDoubleList(relocationTable.relocationData);
    printf("\n%s\n", separator);
}

void UpdateRelocationText(LIST_DOUBLE* relocationList, SECTION** section, LIST_DOUBLE *hash, INSTRUCTION* dicoInstruct){
  int index;
  MIPS_TYPE rMips;
  SECTION* dataSection;
  OPERAND_TYPE operandType;
  for (index=0 ; index<3 ; index++){
    if ((**section).data.instruction.lexemeList[index]){
      if (((LEXEME*)(**section).data.instruction.lexemeList[index]->data)->state == SYMBOL){
        dataSection = IsInHashTable(hash,((char*)((LEXEME*)(**section).data.instruction.lexemeList[index]->data)->value));
        operandType = dicoInstruct[(**section).data.instruction.dicoIndex].operands[index];
        switch (operandType)
        {
          case ABSOLUTE:
          {
            rMips = R_MIPS_26;
            break;
          }
          case IMMEDIAT:
          {
            rMips = R_MIPS_LO16;
            break;
          }
          case BASE_OFFSET:
          {
            rMips = R_MIPS_HI16;
            break;
          }
          default:
          {
            if(operandType == RELATIVE)
              return;
            printf("\nERROR in the relocation table\n");
            return;
          }
        }
        if (!dataSection){
          FillRelocationList(relocationList, UNDEF, (**section).shift, rMips, (LEXEME**)(&((**section).data.instruction.lexemeList[index]->data)));
        }
        else{
          FillRelocationList(relocationList, dataSection->data.label.section, (**section).shift, rMips, (LEXEME**)(&((**section).data.instruction.lexemeList[index]->data)));
        }
      }
    }
  }
}

void UpdateRelocationData(LIST_DOUBLE* relocationList, SECTION** section, LIST_DOUBLE *hash){
  if (((LEXEME*)((**section).data.directiveValue->data))->state == SYMBOL){
    SECTION* dataSection = IsInHashTable(hash,(char*)(((LEXEME*)(**section).data.directiveValue->data)->value));
    if (!dataSection){
      FillRelocationList(relocationList, UNDEF, 0, 0, (LEXEME**)(&((**section).data.directiveValue->data)));
    }
    else{
      FillRelocationList(relocationList, dataSection->data.label.section, dataSection->shift, R_MIPS_32, (LEXEME**)(&((**section).data.directiveValue->data)));
    }
  }
}

void UpdateRelocationTable(RELOCATIONTABLE* relocationTable, LIST_DOUBLE *hash, COLLECTION_LISTS* collectionsList, INSTRUCTION* dicoInstruct){
  int i;
  for(i = 0; i<2; i++)
  {
    LIST_DOUBLE* collectionList = &collectionsList->collection[i];

    if(!*collectionList)
      return;

    LIST_DOUBLE firstNode = *collectionList;
    LIST_DOUBLE slider = *collectionList;

    SECTION** section = (SECTION**)&(*collectionList)->data;

    switch((**section).dataType)
    {
      case TEXT :
        do
        {
          UpdateRelocationText(&(*relocationTable).relocationText, section, hash, dicoInstruct);
          slider = slider->next;
          section = (SECTION**)&slider->data;
        }while(slider != firstNode);
        break;
      case DATA :
        do
        {
          UpdateRelocationData(&(*relocationTable).relocationData, section, hash);
          slider = slider->next;
          section = (SECTION**)&slider->data;
        }while(slider != firstNode);
        break;
      default :
        printf("\nError, not BSS for relocation's table\n");
        break;
    }
  }
}

void ErasedRelocationTable(RELOCATIONTABLE* relocationTable){
  ErasedListDouble(&relocationTable->relocationText);
  ErasedListDouble(&relocationTable->relocationData);
}

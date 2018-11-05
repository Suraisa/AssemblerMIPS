# include "HashTable.h"

LIST* CreateHashTable(){
  LIST* hashTable=calloc(HASHLENGTH,sizeof(*hashTable));
  if (!hashTable){
    for (int index=0 ; index<HASHCHAR ; index++ ){
      hashTable[index]=CreateList();
    }
  }
  return hashTable;
}

int Hash(char* string){
  int sum=0;
  for (int index=0 ; index<HASHCHAR ; index++ ){
    sum+=string[index]*pow(HASHVALUE,index);
  }
  return sum%HASHLENGTH;
}

int AddHashTable(LIST** hash,char** string){
  int index;
  index=Hash(*string);
  LIST nodeI;
  for (nodeI = (*hash)[index]; !IsEmpty(nodeI); nodeI = nodeI->next)
  {
      if (!strcmp(*(char**)nodeI->data,*string)){
        return 0;
      }
  }
  AddInFront(&((*hash)[index]),string,DisplayString, NULL, sizeof(*string));
  return 1;
}

void DisplayHashTable(LIST* hashTable){
  for (int index=0;index<HASHLENGTH;index++){
    printf("Index %d :\n",index);
    Display(hashTable[index]);
  }
}

void ErasedHashTable(LIST** hash)
{
  for (int index=0;index<HASHLENGTH;index++){
    ErasedList(&((*hash)[index]));
  }
}

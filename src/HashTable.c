# include "HashTable.h"

LIST* CreateHashTable(){
  LIST hashTable[HASHLENGTH];
  return hashTable;
}

/*LIST hashTable[HASHLENGTH];
hashTable=CreateList();
hashTable=calloc(HASHLENGTH,sizeof(*hashTable));

LIST newNode = CreateList();
newNode = (LIST)calloc(1, sizeof(*newNode));
newNode->data = malloc(size);

if (newNode->data == NULL)
    return;

return hashTable;*/

int Hash(char* string){
  int sum=0;
  for (int index=0 ; index<HASHCHAR ; index++ ){
    sum+=string[index]*pow(HASHVALUE,index);
  }
  return sum%HASHLENGTH;
}

void AddHashTable(LIST** hash,char** string){
  int index;
  index=Hash(*string);
  int bool=0;
  LIST nodeI;
  for (nodeI = (*hash)[index]; !IsEmpty(nodeI); nodeI = nodeI->next)
  {
      if (strcmp((*hash)[index]->data,*string)==0) bool=1;
  }
  if (bool==0){
    AddInFront((*hash)[index],string,DisplayString, NULL, sizeof(*string));
  }
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
    ErasedList((*hash)[index]);
  }
}

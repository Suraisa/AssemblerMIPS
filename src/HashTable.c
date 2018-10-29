# include "HashTable.h"

LIST* CreateHashTable(){
  LIST hashTable[HASHLENGTH];
  return hashTable;
}

int Hash(char* string){
  int sum=0;
  for (int index=0 ; index<HASHCHAR ; index++ ){
    sum+=string[index]*pow(HASHVALUE,index);
  }
  return sum%HASHLENGTH;
}

void AddHashTable(LIST** hash,char** string){
  int index;
  index=Hash(string);
  AddInFront((*hash)[index],string,DisplayString, NULL, sizeof(*string));
}

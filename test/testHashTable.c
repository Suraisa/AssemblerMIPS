#include "HashTable.h"

int main()
{

    LIST* hashTable=CreateHashTable();
    if (!hashTable)
      return -1;

    char* stringTable[5] = {"test", "secondtest", "thirdtest", "truc", "truc"};

    for (int indexString=0; indexString<5;indexString++){
      int i;
      i=Hash(stringTable[indexString]);
      printf("Index of %s : %d \n",stringTable[indexString],i);
    }

    LIST** ptr_hash=&hashTable;
    for (int indexString=0; indexString<5;indexString++){
      char** ptrString=&stringTable[indexString];
      if(!AddHashTable(ptr_hash,ptrString))
      {
          printf("\nString %s : already present\n\n",stringTable[indexString]);
      }
    }
    
    DisplayHashTable(hashTable);
    printf("\n");
    ErasedHashTable(ptr_hash);
    DisplayHashTable(hashTable);
    return 0;
}

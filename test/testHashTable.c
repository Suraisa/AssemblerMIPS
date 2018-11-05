#include "HashTable.h"

int main()
{

    LIST* hashTable=CreateHashTable();
    if (!hashTable)
      return -1;

    char* a = "test";
    char* b = "secondtest";
    char* c = "thirdtest";
    char* d = "truc";
    char* e = "truc";

    int ia, ib, ic, id, ie;
    ia=Hash(a);
    printf("Index of %s : %d \n",a,ia);
    ib=Hash(b);
    printf("Index of %s : %d \n",b,ib);
    ic=Hash(c);
    printf("Index of %s : %d \n",c,ic);
    id=Hash(d);
    printf("Index of %s : %d \n",d,id);
    ie=Hash(e);
    printf("Index of %s : %d \n",e,ie);


    LIST** ptr_hash=&hashTable;
    char** ptr_a=&a;
    if(!AddHashTable(ptr_hash,ptr_a))
    {
        printf("String already present");
    }
    char** ptr_b=&b;
    if(!AddHashTable(ptr_hash,ptr_b))
    {
        printf("String already present");
    }
    char** ptr_c=&c;
    if(!AddHashTable(ptr_hash,ptr_c))
    {
        printf("String already present");
    }
    char** ptr_d=&d;
    if(!AddHashTable(ptr_hash,ptr_d))
    {
        printf("String already present");
    }
    char** ptr_e=&e;
    if(!AddHashTable(ptr_hash,ptr_e))
    {
        printf("String already present");
    }
    DisplayHashTable(hashTable);

    ErasedHashTable(ptr_hash);
    DisplayHashTable(hashTable);
    return 0;
}

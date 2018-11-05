#include "HashTable.h"

int main()
{

    LIST* hashTable=CreateHashTable();
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

    printf("test2");
    printf("test3");
    LIST** ptr_hash=&hashTable;
    char** ptr_a=&a;
    AddHashTable(ptr_hash,ptr_a);
    char** ptr_b=&b;
    AddHashTable(ptr_hash,ptr_b);
    char** ptr_c=&c;
    AddHashTable(ptr_hash,ptr_c);
    char** ptr_d=&d;
    AddHashTable(ptr_hash,ptr_d);
    char** ptr_e=&e;
    AddHashTable(ptr_hash,ptr_e);
    DisplayHashTable(hashTable);

    ErasedHashTable(ptr_hash);
    DisplayHashTable(hashTable);
}

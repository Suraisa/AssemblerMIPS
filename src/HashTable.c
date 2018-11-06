#include "HashTable.h"
#include "Section.h"


LIST *CreateHashTable()
{
  LIST *hashTable = calloc(HASHLENGTH, sizeof(*hashTable));
  if (!hashTable)
  {
    int index;
    for (index = 0; index < HASHCHAR; index++)
    {
      hashTable[index] = CreateList();
    }
  }
  return hashTable;
}

int Hash(char *string)
{
  int sum = 0;
  int index;
  for (index = 0; index < HASHCHAR; index++)
  {
    sum += string[index] * pow(HASHVALUE, index);
  }
  return sum % HASHLENGTH;
}

int AddHashTable(LIST **hash, SECTION *label)
{
  int index;
  char *string = (char *)((LEXEME *)(label->data.label.lexemeList)->data)->value;
  index = Hash(string);
  LIST nodeI;
  for (nodeI = (*hash)[index]; !IsEmpty(nodeI); nodeI = nodeI->next)
  {
    if (!strcmp((char *)((LEXEME *)(((SECTION *)nodeI->data)->data.label.lexemeList)->data)->value, string))
      return 0;
  }
  AddInFront(&((*hash)[index]), label, DisplaySection, NULL, sizeof(*label));
  return 1;
}

void DisplayHashTable(LIST *hashTable)
{
  int index;
  for (index = 0; index < HASHLENGTH; index++)
  {
    printf("Index %d :\n", index);
    Display(hashTable[index]);
  }
}

void ErasedHashTable(LIST **hash)
{
  int index;
  for (index = 0; index < HASHLENGTH; index++)
  {
    ErasedList(&((*hash)[index]));
  }
  free(*hash);
}

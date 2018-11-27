#include "HashTable.h"
#include "Section.h"


LIST_DOUBLE *CreateHashTable()
{
  LIST_DOUBLE *hashTable = calloc(HASHLENGTH, sizeof(*hashTable));
  if (!hashTable)
  {
    int index;
    for (index = 0; index < HASHCHAR; index++)
    {
      hashTable[index] = CreateListDouble();
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

int AddHashTable(LIST_DOUBLE **hash, SECTION *label)
{
  int index;
  char *string = (char *)((LEXEME *)(label->data.label.lexemeList)->data)->value;
  index = Hash(string);
  LIST_DOUBLE nodeI = (*hash)[index];
  LIST_DOUBLE firstNode = (*hash)[index];

  if(!IsEmptyDouble(nodeI))
  {
    do
    {

      if (!strcmp((char *)((LEXEME *)(((SECTION *)nodeI->data)->data.label.lexemeList)->data)->value, string))
        return 0;

      nodeI = nodeI->next;
    }while(nodeI != firstNode);
  }
    AddInFrontDouble(&((*hash)[index]), label, DisplaySection, NULL, sizeof(*label));
    return 1;
}

SECTION* IsInHashTable(LIST_DOUBLE *hash, char* string)
{
  int index;
  index = Hash(string);
  LIST_DOUBLE nodeI = (hash)[index];
  LIST_DOUBLE firstNode = (hash)[index];

  if(!IsEmptyDouble(nodeI))
  {
    do
    {
      if (!strcmp((char *)((LEXEME *)(((SECTION *)nodeI->data)->data.label.lexemeList)->data)->value, string))
        return ((SECTION *)nodeI->data);

      nodeI = nodeI->next;
    }while(nodeI != firstNode);
  }
  return NULL;
}

void DisplayHashTable(LIST_DOUBLE *hashTable)
{
  int index;
  for (index = 0; index < HASHLENGTH; index++)
  {
    printf("Index %d :\n", index);
    DisplayDoubleList(hashTable[index]);
  }
}

void ErasedHashTable(LIST_DOUBLE **hash)
{
  int index;
  for (index = 0; index < HASHLENGTH; index++)
  {
    ErasedListDouble(&((*hash)[index]));
  }
  free(*hash);
}

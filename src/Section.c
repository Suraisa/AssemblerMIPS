#include "Section.h"

void InitializeCollectionLists(COLLECTION_LISTS* collections)
{
    int index = 0;
    for(index; index<3; index++)
    {
        collections->collection[index] = CreateQueue();
    }
}
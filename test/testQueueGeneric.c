#include <stdio.h>

#include "QueueGeneric.h"
#include "DisplayType.h"

int main()
{
    QUEUE queue = CreateQueue();
    long int a = 0;
    float b = 5.8;
    double c = 8.2;
    char d = 'x';
    char *e = "test";

    PushQueue(&queue, &d, &DisplayChar, NULL, sizeof(char));
    d = 'b';
    PushQueue(&queue, &d, &DisplayChar, NULL, sizeof(char));
    d = 'c';
    PushQueue(&queue, &d, &DisplayChar, NULL, sizeof(char));
    
    Display(queue);

    IsEmpty(queue) ? printf("\nThe queue is empty.\n") : printf("\nThe queue is not empty.\n");

    ErasedQueue(&queue);

    Display(queue);

    IsEmpty(queue) ? printf("\nThe queue is empty.\n") : printf("\nThe queue is not empty.\n");

    return 0;
}
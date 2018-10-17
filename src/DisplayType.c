#include "DisplayType.h"

void DisplayInt(void *value)
{
    printf("\nThe int value is : %d\n", *((int *)value));
}

void DisplayFloat(void *value)
{
    printf("\nThe float value is : %f\n", *((float *)value));
}

void DisplayDouble(void *value)
{
    printf("\nThe double value ist : %lf\n", *((double *)value));
}

void DisplayChar(void *value)
{
    printf("\nThe char value is : %c\n", *((char *)value));
}

void DisplayString(void *value)
{
    printf("\nThe string value is : %s\n", *((char **)value));
}
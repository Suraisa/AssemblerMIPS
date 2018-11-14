#include "Utils.h"

unsigned int StringSize(char *string)
{
    if (string == NULL)
        return 0;

    int index = 0;

    while (string[index] != '\0')
    {
        index++;
    }

    return index + 1;
}

char* ConcatenateCharListDouble(LIST_DOUBLE list)
{
    if (IsEmptyDouble(list) || list->display != &DisplayChar)
    {
        char *empty = (char *)malloc(sizeof(char));

        if (empty == NULL)
                return "";

        *empty = '\0';
        return empty;
    }

    unsigned long int listSize = SizeListDouble(list);
    char *concatenate = (char *)malloc((listSize+1) * sizeof(char));

    if (concatenate == NULL)
                return "";

    int counter = listSize;
    concatenate[counter] = '\0';
    counter--;
    LIST_DOUBLE slider = list;
    LIST_DOUBLE firstNode = list;

    do
    {
        if(list->display != &DisplayChar)
            return concatenate;
        
        concatenate[counter] = *((char *)slider->data);
        counter--;
        slider = slider->next;
    }while(slider != firstNode);
    
    return concatenate;
}

char* ConcatenateCharList(LIST list)
{
    if (IsEmpty(list) || list->display != &DisplayChar)
    {
        char *empty = (char *)malloc(sizeof(char));

        if (empty == NULL)
                return "";

        *empty = '\0';
        return empty;
    }

    unsigned long int listSize = SizeList(list);
    char *concatenate = (char *)malloc((listSize+1) * sizeof(char));

    if (concatenate == NULL)
                return "";

    int counter = listSize;
    concatenate[counter] = '\0';
    counter--;
    LIST slider = CreateList();

    for (slider = list; !IsEmpty(slider); slider = slider->next)
    {
        if(list->display != &DisplayChar)
            return concatenate;
        
        concatenate[counter] = *((char *)slider->data);
        counter--;
    }
    return concatenate;
}

long int ConcatenateIntListDouble(LIST_DOUBLE list)
{
    if (IsEmptyDouble(list) || list->display != &DisplayInt)
        return 0;

    long int value = 0;
    unsigned long int listSize = SizeListDouble(list);
    unsigned long int counter = 0;
    LIST_DOUBLE slider = list;
    LIST_DOUBLE firstNode = list;

    do
    {
        if (list->display != &DisplayInt)
            return value;

        value += (*(int *)slider->data) * pow(10, counter);
        counter++;
        slider = slider->next;
    }while(slider != firstNode);

    return value;
}

long int ConcatenateIntList(LIST list)
{
    if (IsEmpty(list) || list->display != &DisplayInt)
        return 0;

    long int value = 0;
    unsigned long int listSize = SizeList(list);
    unsigned long int counter = 0;
    LIST slider;

    for (slider = list; !IsEmpty(slider); slider = slider->next)
    {
        if (list->display != &DisplayInt)
            return value;

        value += (*(int *)slider->data) * pow(10, counter);
        counter++;
    }

    return value;
}

int CharIsNumber(char value)
{
    return 0x30 <= value && value <= 0x39;
}

int CharIsUpperLetter(char value)
{
    return 0x41 <= value && value <= 0x5A;
}

int CharIsLowerLetter(char value)
{
    return 0x61 <= value && value <= 0x7A;
}

int CharIsLetter(char value)
{
    return CharIsUpperLetter(value) || CharIsLowerLetter(value);
}

int CharIsHexadecimal(char value)
{
    return CharIsNumber(value) || (0x41 <= value && value <= 0x46) || (0x61 <= value && value <= 0x66);
}

int CharIsNumberLetter(char value)
{
    return CharIsLetter(value) || CharIsHexadecimal(value);
}

void StringUpper(char* string)
{
    int index = 0;
    char uppedString[100];
    strcpy(uppedString, string);
    while(string[index] != '\0')
    {
        if(islower(uppedString[index]))
        {
            uppedString[index] = toupper(uppedString[index]);
        }
        index++;
    }
    strcpy(string, uppedString);
}

unsigned long int StringHexToDecimal(char* value)
{
    char *next;
    unsigned long int decimal = strtol(value, &next, 16);
    return decimal;
}

int IsAvailableRegister(char* registerToTest)
{
    
}
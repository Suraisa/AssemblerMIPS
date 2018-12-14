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

int IsAvailableRegister(char* registerToTest, int** newName)
{
    if (StringSize(registerToTest)<=3 && CharIsNumber((registerToTest)[0]))
    {
        if ((registerToTest)[0] > '3' && StringSize(registerToTest) == 3)
            return 0;

        if((registerToTest)[0] == '3' && StringSize(registerToTest) == 3 && (registerToTest)[1] >= '2' && (registerToTest)[1] < '0')
            return 0;
        
        ChangeRegisterName(newName, atoi(registerToTest));
        return 1;
    }
    else if (!strcmp(registerToTest, "zero"))
    {
        ChangeRegisterName(newName, 0);
        return 1;
    }
    else if ((registerToTest)[0] == 'a' && StringSize(registerToTest) == 3)
    {
        if((registerToTest)[1] >= '0' && (registerToTest)[1] <= '3')
        {
            ChangeRegisterName(newName, (registerToTest)[1] + 4 - '0');
            return 1;
        }
        else if((registerToTest)[1] == 't')
        {
            ChangeRegisterName(newName, 1);
            return 1;
        }
        return 0;
    }
    else if ((registerToTest)[0] == 'v' && StringSize(registerToTest) == 3)
    {
        if((registerToTest)[1] >= '0' && (registerToTest)[1] <= '1')
        {
            ChangeRegisterName(newName, (registerToTest)[1] + 2 - '0');
            return 1;
        }
        return 0;
    }
    else if (((registerToTest)[0] == 't' || (registerToTest)[0] == 's') && StringSize(registerToTest) == 3)
    {
        if((registerToTest)[1] >= '0' && (registerToTest)[1] <= '9')
        {
            if((registerToTest)[1] <= '7')
            {
                if((registerToTest)[0] == 's')
                {
                    ChangeRegisterName(newName, (registerToTest)[1] + 16 - '0');
                }
                else
                {
                    ChangeRegisterName(newName, (registerToTest)[1] + 8 - '0');
                }
            }
            else if((registerToTest)[0] == 't')
            {
                ChangeRegisterName(newName, (registerToTest)[1] + 16 - '0');
            }
            else
            {
                return 0;
            }
            return 1;
        }
        return 0;
    }
    else if ((registerToTest)[0] == 'k' && StringSize(registerToTest) == 3 && (registerToTest)[1] >= '0' && (registerToTest)[1] <= '1')
    {
        ChangeRegisterName(newName, (registerToTest)[1] + 26 - '0');
        return 1;
    }
    else if (!strcmp(registerToTest, "gp"))
    {
        ChangeRegisterName(newName, 28);
        return 1;
    }
    else if (!strcmp(registerToTest, "sp"))
    {
        ChangeRegisterName(newName, 29);
        return 1;
    }
    else if (!strcmp(registerToTest, "fp"))
    {
        ChangeRegisterName(newName, 30);
        return 1;
    }
    else if (!strcmp(registerToTest, "ra"))
    {
        ChangeRegisterName(newName, 31);
        return 1;
    }
    return 0;
}

void ChangeRegisterName(int** newName, int number)
{
    *newName = malloc(sizeof(**newName));
    **newName = number;
}

void ReplaceString(char** fullString, char* replaceString, int start, int end)
{
    char* newString = malloc(strlen(*fullString)-(end-start) + strlen(replaceString));

    char* firstPart = CutString(*fullString, 0, start);
    char* secondPart = CutString(*fullString, end+1, strlen(*fullString)-end-1);
    strcpy(newString, "");
    strcat(newString, firstPart);
    free(firstPart);
    strcat(newString, replaceString);
    strcat(newString, secondPart);
    free(secondPart);
    free(*fullString);
    
    *fullString = newString;
}

char* CutString(char* fullString, int start, int length)
{
    char* newString = malloc(length+1);

    if(!newString)
        return NULL;
    
    int index; 
    for(index = start; index<start+ length; index++)
    {
        newString[index-start] = fullString[index];
    }

    newString[index-start] = '\0';

    return newString;
}

void ErasedTableString(char** table, int size)
{
    int i;
    for (i = 0; i<size; i++)
    {
        free(table[i]);
    }
    free(table);
}

char* ChangeExtension(char* string, char* extension)
{
    char valueToFind = '.';
    int size = 0;
    int index = 0;
    while(string[size] != valueToFind)
    {
        size++;
    }
    char* stringCreated = calloc(size+strlen(extension)+1, sizeof(*stringCreated));
    for(index = 0; index<size; index++)
    {
        stringCreated[index] = string[index];
    }
    strcat(stringCreated, extension);
    return stringCreated;
}

int StringInList(LIST_DOUBLE list, char* string)
{
    if(IsEmptyDouble(list))
        return 0;
    
    LIST_DOUBLE firstNode = list;
    LIST_DOUBLE slider = firstNode;
    int isIn = 0;
    do
    {
        if(!strcmp((char*)slider->data, string))
            return 1;

        slider = slider->next;
    }while(slider != firstNode );
    return 0;
}
#include "PseudoInstruction.h"

int main()
{
    FILE* file;
    file = fopen("src/DicoPseudoInstruct.txt", "r");
    char* string = "MOVE";
    
    printf("%d\n", FindPseudoInstruction(string, file));

    return 0;
}
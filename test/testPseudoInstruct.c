#include "PseudoInstruction.h"

int main()
{
    FILE* file;
    file = fopen("src/DicoPseudoInstruct.txt", "r");
    char* string = "MOVE";
    SECTION section;
    
    PSEUDO_INSTRUCTION* dico = InitializePseudoDicoInstruct("src/DicoPseudoInstruct.txt");

    int index;

    for (index = 0; index<sizePseudoDico; index++)
    {
        printf("\n%s\n", dico[index].id);
    }

    printf("%d\n", FindPseudoInstruction(string, &file, &section));

    LIST_DOUBLE lexemeQueue = CreateListDouble();
    // if(!CreateNewListLexeme(&file, &lexemeQueue))
    // {
    //     ErasedQueueDouble(&lexemeQueue);
    //     return 0;
    // }

    // DisplayDoubleList(lexemeQueue);

    // ErasedListDouble(&lexemeQueue);
    free(dico);
    fclose(file);

    return 0;
}
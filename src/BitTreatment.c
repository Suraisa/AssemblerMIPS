#include "BitTreatment.h"

#define Swap(value1, value2)\
{\
    value1 = value1 ^ value2;\
    value2 = value1 ^ value2;\
    value1 = value1 ^ value2;\
}\

#define SwapCode(value)\
{\
    Swap(value[0], value[3]);\
    Swap(value[1], value[2]);\
}\

SECTION_FIELD CreateSectionField(unsigned long int size)
{
    SECTION_FIELD sectionBitField;
    sectionBitField.bitField = malloc(size * sizeof(BIT_FIELD));
    sectionBitField.size = size;

    return sectionBitField;
}

#define JBitTreatment(jInst, absolute, indexOperand)\
{\
    if(indexOperand == 0)\
    {\
        jInst.target = absolute;\
    }\
}\

#define RBitTreatment(indexRegisterDico, indexRegister, dicoInstructDetails, rInst, value)\
{\
    while(indexRegisterDico<4 && dicoInstructDetails.modificable[indexRegisterDico] != '1')\
    {\
        indexRegisterDico++;\
    }\
    if(dicoInstructDetails.modificable[indexRegisterDico] == '1')\
    {\
        switch(dicoInstructDetails.order[indexRegisterDico])\
        {\
            case 's':\
            {\
                rInst.rs = value;\
                break;\
            }\
            case 'a':\
            {\
                rInst.sa = value;\
                break;\
            }\
            case 'd':\
            {\
                rInst.rd = value;\
                break;\
            }\
            case 't':\
            {\
                rInst.rt = value;\
                break;\
            }\
        }\
        indexRegister++;\
    }\
    indexRegisterDico++;\
}\

#define IBitTreatment(hasBaseOffset, indexRegisterDico, indexRegister, indexOperand, dicoInstruct, iInst, lexemeList)\
{\
    if(dicoInstruct.operands[indexOperand] != 'I' && dicoInstruct.operands[indexOperand] != 'R')\
    {\
        while(indexRegisterDico<2 && dicoInstruct.details.modificable[indexRegisterDico] != '1')\
        {\
            indexRegisterDico++;\
        }\
        if(dicoInstruct.details.modificable[indexRegisterDico] == '1')\
        {\
            switch(dicoInstruct.details.order[indexRegisterDico])\
            {\
                case 's':\
                {\
                    switch(dicoInstruct.operands[indexOperand])\
                    {\
                        case 'B':\
                        {\
                            hasBaseOffset = 1;\
                            iInst.rs = *(unsigned int*)((LEXEME*)lexemeList->next->next->data)->value;\
                            iInst.immediate = *(unsigned int*)((LEXEME*)lexemeList->data)->value;\
                            break;\
                        }\
                        default:\
                        {\
                            iInst.rs = *(unsigned int*)((LEXEME*)lexemeList->data)->value;\
                            break;\
                        }\
                    }\
                    break;\
                }\
                case 't':\
                {\
                    switch(dicoInstruct.operands[indexOperand])\
                    {\
                        case 'B':\
                        {\
                            printf("\nError in the dictionary for instruction : %s\n", dicoInstruct.id);\
                            break;\
                        }\
                        default:\
                        {\
                            iInst.rt = *(unsigned int*)((LEXEME*)lexemeList->data)->value;\
                        }\
                    }\
                    break;\
                }\
            }\
            indexRegister++;\
        }\
    }\
    else\
    {\
        iInst.immediate = *(unsigned int*)((LEXEME*)lexemeList->data)->value;\
    }\
    indexRegisterDico++;\
}\

SECTION_FIELD BitInstructionTreatment(INSTRUCTION* dictionary, LIST_DOUBLE instructions)
{
    if(instructions == NULL)
        return (SECTION_FIELD){.bitField = NULL, .size = 0};


    SECTION_FIELD instBitTreatment = CreateSectionField(((SECTION*)instructions->prev->data)->shift/4+1);
    
    LIST_DOUBLE firstNode = instructions;
    LIST_DOUBLE slider = instructions;
    INSTRUCTION_DATA instData;
    INSTRUCTION dicoInstruct;
    int hasBaseOffset = 0;
    int indexOperand = 0;
    int indexSlider = 0;
    int indexRegisterDico = 0;
    int indexRegister = 0;

    do
    {
        instData = ((SECTION*)slider->data)->data.instruction;
        dicoInstruct = dictionary[instData.dicoIndex];
        instBitTreatment.bitField[indexSlider].intInst = dicoInstruct.bitField.intInst;
        while (indexOperand<3 && !IsEmptyDouble(instData.lexemeList[indexOperand]))
        {
            switch(dicoInstruct.details.type)
            {
                case 'J':
                {
                    JBitTreatment(instBitTreatment.bitField[indexSlider].jInst, *(unsigned int*)((LEXEME*)instData.lexemeList[indexOperand]->data)->value, indexOperand);
                    break;
                }
                case 'R':
                {
                    RBitTreatment(indexRegisterDico, indexRegister, dicoInstruct.details, instBitTreatment.bitField[indexSlider].rInst, *(unsigned int*)((LEXEME*)instData.lexemeList[indexRegister]->data)->value);
                    break;
                }
                case 'I':
                {
                    IBitTreatment(hasBaseOffset, indexRegisterDico, indexRegister, indexOperand, dicoInstruct, instBitTreatment.bitField[indexSlider].iInst, instData.lexemeList[indexRegister]);
                    break;
                }
            }
            indexOperand++;
        }
        SwapCode(instBitTreatment.bitField[indexSlider].code);
        printf("%s %08x\n", dicoInstruct.id, instBitTreatment.bitField[indexSlider].intInst);
        indexRegister = 0;
        indexOperand = 0;    
        indexRegisterDico = 0;  
        hasBaseOffset = 0;  
        indexSlider++;
        slider = slider->next;
    }while(firstNode != slider);

    return instBitTreatment;
}

unsigned long int BitBssTreatment(LIST_DOUBLE bss)
{
    if (IsEmptyDouble(bss))
        return 0;

    SECTION* section = (SECTION*)bss->prev->data;
    long int lastSpaceAllocated = *(long int*)((LEXEME*)section->data.directiveValue->data)->value;
    return section->shift + lastSpaceAllocated + lastSpaceAllocated%8%8;
}
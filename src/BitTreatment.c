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

SECTION_FIELD BitInstructionTreatment(INSTRUCTION* dictionary, LIST_DOUBLE instructions, unsigned long int size)
{
    if(instructions == NULL)
        return (SECTION_FIELD){.bitField = NULL, .size = 0};


    SECTION_FIELD instBitTreatment = CreateSectionField(size>>2);
    
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
        indexRegister = 0;
        indexOperand = 0;    
        indexRegisterDico = 0;  
        hasBaseOffset = 0;  
        indexSlider++;
        slider = slider->next;
    }while(firstNode != slider);

    return instBitTreatment;
}

SECTION_FIELD BitDataTreatment(LIST_DOUBLE data, unsigned long int size)
{
    if(data == NULL)
        return (SECTION_FIELD){.bitField = NULL, .size = 0};

    SECTION_FIELD dataBitTreatment = CreateSectionField((size + size%4%4)>>2);
    
    LIST_DOUBLE firstNode = data;
    LIST_DOUBLE slider = data;
    int indexField = 0;
    int octet = 0;
    int shift = 0;
    int counter;
    do
    {
        switch(((SECTION*)slider->data)->state)
        {
            case ASCIZZ:
            {
                for(counter = 0; counter<strlen(((LEXEME*)((SECTION*)slider->data)->data.directiveValue->data)->value)+1; counter++)
                {
                    dataBitTreatment.bitField[indexField].code[octet] = ((char*)((LEXEME*)((SECTION*)slider->data)->data.directiveValue->data)->value)[counter];
                    octet++;
                    if(!(octet%4) && octet)
                    {
                        octet = 0;
                        indexField ++;
                    }
                }
                break;
            }
            case WORD:
            {
                shift = octet%4;
                if(!shift && octet)
                {
                    for (counter = 0; counter<shift%4; counter++)
                    {
                        dataBitTreatment.bitField[indexField].code[octet] = 0;
                        octet++;
                    }
                    octet = 0;
                    indexField ++;
                }
                dataBitTreatment.bitField[indexField].intInst = *(long int*)((LEXEME*)((SECTION*)slider->data)->data.directiveValue->data)->value;
                SwapCode(dataBitTreatment.bitField[indexField].code);
                indexField ++;
                break;
            }
            case BYTE:
            {
                dataBitTreatment.bitField[indexField].code[octet] = *(long int*)((LEXEME*)((SECTION*)slider->data)->data.directiveValue->data)->value;
                octet++;
                break;
            }
            case SPACE:
            {
                for(counter = 0; counter<*(long int*)((LEXEME*)((SECTION*)slider->data)->data.directiveValue->data)->value; counter++)
                {
                    dataBitTreatment.bitField[indexField].code[octet] = 0;
                    octet++;
                    if(!(octet%4) && octet)
                    {
                        octet = 0;
                        indexField ++;
                    }
                }
                break;
            }
        }
        slider = slider->next;
    }while(firstNode != slider);
    
    return dataBitTreatment;
}
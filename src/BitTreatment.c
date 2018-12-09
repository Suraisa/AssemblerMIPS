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
                    if(indexOperand == 0)
                    {
                        instBitTreatment.bitField[indexSlider].jInst.target = *(unsigned int*)((LEXEME*)instData.lexemeList[indexOperand]->data)->value;
                    }
                    break;
                }
                case 'R':
                {
                    while(indexRegisterDico<4 && dicoInstruct.details.modificable[indexRegisterDico] != '1')
                    {
                        indexRegisterDico++;
                    }
                    if(dicoInstruct.details.modificable[indexRegisterDico] == '1')
                    {
                        switch(dicoInstruct.details.order[indexRegisterDico])
                        {
                            case 's':
                            {
                                instBitTreatment.bitField[indexSlider].rInst.rs = *(unsigned int*)((LEXEME*)instData.lexemeList[indexRegister]->data)->value;
                                break;
                            }
                            case 'a':
                            {
                                instBitTreatment.bitField[indexSlider].rInst.sa = *(unsigned int*)((LEXEME*)instData.lexemeList[indexRegister]->data)->value;
                                break;
                            }
                            case 'd':
                            {
                                instBitTreatment.bitField[indexSlider].rInst.rd = *(unsigned int*)((LEXEME*)instData.lexemeList[indexRegister]->data)->value;
                                break;
                            }
                            case 't':
                            {
                                instBitTreatment.bitField[indexSlider].rInst.rt = *(unsigned int*)((LEXEME*)instData.lexemeList[indexRegister]->data)->value;
                                break;
                            }
                        }
                        indexRegister++;
                    }
                    indexRegisterDico++;
                    break;
                }
                case 'I':
                {
                    if(dicoInstruct.operands[indexOperand] != 'I')
                    {
                        while(indexRegisterDico<2 && dicoInstruct.details.modificable[indexRegisterDico] != '1')
                        {
                            indexRegisterDico++;
                        }
                        if(dicoInstruct.details.modificable[indexRegisterDico] == '1')
                        {
                            switch(dicoInstruct.details.order[indexRegisterDico])
                            {
                                case 's':
                                {
                                    switch(dicoInstruct.operands[indexOperand])
                                    {
                                        case 'B':
                                        {
                                            hasBaseOffset = 1;
                                            instBitTreatment.bitField[indexSlider].iInst.rs = *(unsigned int*)((LEXEME*)instData.lexemeList[indexRegister]->next->next->data)->value;
                                            instBitTreatment.bitField[indexSlider].iInst.immediate = *(unsigned int*)((LEXEME*)instData.lexemeList[indexRegister]->data)->value;
                                            break;
                                        }
                                        default:
                                        {
                                            instBitTreatment.bitField[indexSlider].iInst.rs = *(unsigned int*)((LEXEME*)instData.lexemeList[indexRegister]->data)->value;
                                            break;
                                        }
                                    }
                                    break;
                                }
                                case 't':
                                {
                                    switch(dicoInstruct.operands[indexOperand])
                                    {
                                        case 'B':
                                        {
                                            printf("\nError in the dictionary for instruction : %s", dicoInstruct.id);
                                            break;
                                        }
                                        default:
                                        {
                                            instBitTreatment.bitField[indexSlider].iInst.rt = *(unsigned int*)((LEXEME*)instData.lexemeList[indexRegister]->data)->value;
                                        }
                                    }
                                    break;
                                }
                            }
                            indexRegister++;
                        }
                    }
                    else
                    {
                        instBitTreatment.bitField[indexSlider].iInst.immediate = *(unsigned int*)((LEXEME*)instData.lexemeList[indexOperand]->data)->value;
                    }
                    indexRegisterDico++;
                    break;
                }
            }
            indexOperand++;
        }
        SwapCode(instBitTreatment.bitField[indexSlider].code);
        printf("%08x\n", instBitTreatment.bitField[indexSlider].intInst);
        indexRegister = 0;
        indexOperand = 0;    
        indexRegisterDico = 0;    
        indexSlider++;
        slider = slider->next;
    }while(firstNode != slider);

    return instBitTreatment;
}
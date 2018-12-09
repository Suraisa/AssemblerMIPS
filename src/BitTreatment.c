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


    SECTION_FIELD instBitTreatment = CreateSectionField(((SECTION*)instructions->prev->data)->shift/4);
    
    LIST_DOUBLE firstNode = instructions;
    LIST_DOUBLE slider = instructions;
    INSTRUCTION_DATA instData;
    INSTRUCTION dicoInstruct;
    int indexOperand;
    int indexSlider = 0;
    int indexRegisterDico;
    int indexRegister = 0;

    do
    {
        instData = ((SECTION*)slider->data)->data.instruction;
        dicoInstruct = dictionary[instData.dicoIndex];

        for (indexOperand = 0; indexOperand<3; indexOperand++)
        {
            switch(dicoInstruct.details.type)
            {
                case 'J':
                {
                    if(indexOperand == 0)
                    {
                        instBitTreatment.bitField[indexSlider].intInst = dicoInstruct.bitField.intInst;
                        instBitTreatment.bitField[indexSlider].jInst.target = *(unsigned int*)((LEXEME*)instData.lexemeList[indexOperand]->data)->value;
                        SwapCode(instBitTreatment.bitField[indexSlider].code);
                    }
                    break;
                }
                case 'R':
                {
                    instBitTreatment.bitField[indexSlider].intInst = dicoInstruct.bitField.intInst;
                    for(indexRegisterDico = 0; indexRegisterDico<4; indexRegisterDico++)
                    {
                        if(dicoInstruct.details.modificable[indexRegisterDico])
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
                    }
                    indexRegister = 0;
                    instBitTreatment.bitField[indexSlider].jInst.target = *(unsigned int*)((LEXEME*)instData.lexemeList[indexOperand]->data)->value;
                    SwapCode(instBitTreatment.bitField[indexSlider].code);
                }
                case 'I':
                {

                }
            }
        }
        indexSlider++;
        slider = slider->next;
    }while(firstNode != slider);
}
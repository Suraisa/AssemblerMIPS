#include "FSMCollection.h"

int InitializationCollection(COLLECTION_LISTS* collectionLists)
{
    collectionLists->labelTable = CreateHashTable();
    if(!collectionLists->labelTable)
        return 0;

    int index;
    for(index = 0; index<3; index++)
    {
        collectionLists->collection[index] = CreateListDouble();
    }
    return 1;
}

void InitializationCollectionFsm(COLLECTION_FSM *stateMachine)
{
    stateMachine->previousState = INIT_COLLECTION;
    stateMachine->currentState = INIT_COLLECTION;
    stateMachine->actualCollection = TEXT;
    stateMachine->error = 0;
    stateMachine->inState = 0;
    stateMachine->pseudoInstruction = 0;
    stateMachine->numberOfInversedLexeme = 0;
    int index;
    for (index=0; index<3; index++)
    {
        stateMachine->nextShift[index] = 0;
        stateMachine->shift[index] = 0;
    }
}

void InitCollectionFsm(COLLECTION_FSM *stateMachine, QUEUE_DOUBLE *lexemeQueue)
{
    switch (((LEXEME *)(*lexemeQueue)->data)->state)
    {
        case RETURN:
        case COMMENT:
        {
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            break;
        }
        case SYMBOL:
        {
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INSTRUCTION0;
            break;
        }
        case DIRECTIVE:
        {
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = DIR0;
            break;
        }
        case COLON:
        {
            if(stateMachine->previousState == INSTRUCTION0)
                break;

            PrintErrorCollection(stateMachine, ((LEXEME*)((*lexemeQueue)->data))->lineNumber, "You can't use:","now" ,((LEXEME*)((*lexemeQueue)->data))->type);
        }
        default:
        {
            PrintErrorCollection(stateMachine, ((LEXEME*)((*lexemeQueue)->data))->lineNumber, "You can't use:","now" ,((LEXEME*)((*lexemeQueue)->data))->type);
            break;
        }
    }
}

void CollectionFsm(COLLECTION_FSM *stateMachine, QUEUE_DOUBLE *lexemeQueue, COLLECTION_LISTS* collections, INSTRUCTION* instructionDictionary, PSEUDO_INSTRUCTION* pseudoDictionary)
{
    switch (stateMachine->currentState)
    {
        case INIT_COLLECTION:
        {
            InitCollectionFsm(stateMachine, lexemeQueue);
            if (stateMachine->currentState != INIT_COLLECTION)
            {
                stateMachine->inState = 0;
                CollectionFsm(stateMachine, lexemeQueue, collections, instructionDictionary, pseudoDictionary);
                break;
            }
            ErasedInFrontDouble(lexemeQueue);
            break;
        }
        case DIR0:
        {
            char* lexemeValue = (char*)(((LEXEME *)(*lexemeQueue)->data)->value);

            if(!strcmp(lexemeValue, ".text"))
            {
                stateMachine->actualCollection = TEXT;
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = INIT_COLLECTION;
            }
            else if(!strcmp(lexemeValue, ".bss"))
            {
                stateMachine->actualCollection = BSS;
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = INIT_COLLECTION;
            }
            else if(!strcmp(lexemeValue, ".data"))
            {
                stateMachine->actualCollection = DATA;
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = INIT_COLLECTION;
            }
            else if(!strcmp(lexemeValue, ".set"))
            {
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = SET_NOREORDER;
            }
            else if(!strcmp(lexemeValue, ".space"))
            {
                if(stateMachine->actualCollection == TEXT)
                {
                    PrintErrorCollection(stateMachine, ((LEXEME*)((*lexemeQueue)->data))->lineNumber, "You're not in the correct section for:","" ,((LEXEME*)((*lexemeQueue)->data))->type);
                }
                else
                {
                    stateMachine->currentState = SPACE;
                    stateMachine->previousState = stateMachine->currentState;
                }
            }
            else if(!strcmp(lexemeValue, ".word"))
            {
                if(stateMachine->actualCollection != DATA)
                {
                    PrintErrorCollection(stateMachine, ((LEXEME*)((*lexemeQueue)->data))->lineNumber, "You're not in the correct section for:","" ,((LEXEME*)((*lexemeQueue)->data))->type);
                }
                else
                {
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = WORD;
                }
            }
            else if(!strcmp(lexemeValue, ".byte"))
            {
                if(stateMachine->actualCollection != DATA)
                {
                    PrintErrorCollection(stateMachine, ((LEXEME*)((*lexemeQueue)->data))->lineNumber, "You're not in the correct section for:","" ,((LEXEME*)((*lexemeQueue)->data))->type);
                }
                else
                {
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = BYTE;
                }
            }
            else if(!strcmp(lexemeValue, ".asciiz"))
            {
                if(stateMachine->actualCollection != DATA)
                {
                    PrintErrorCollection(stateMachine, ((LEXEME*)((*lexemeQueue)->data))->lineNumber, "Your directive doesn't exist:","" ,((LEXEME*)((*lexemeQueue)->data))->type);
                }
                else
                {
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = ASCIZZ;
                }
            }
            else
            {
                PrintErrorCollection(stateMachine, ((LEXEME*)((*lexemeQueue)->data))->lineNumber, "Your directive doesn't exist:","" ,((LEXEME*)((*lexemeQueue)->data))->type);
            }
            ErasedInFrontDouble(lexemeQueue);
            break;
        }
        case SPACE:
        {
            LIST_DOUBLE popedLexeme = PopInFrontDouble(lexemeQueue, 1);
            switch (((LEXEME*)(popedLexeme->data))->state)
            {
                default:
                    PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    break;
                case OPERATOR:
                    if(*(char*)((LEXEME*)(popedLexeme->data))->value == '-')
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    }
                    ErasedListDouble(&popedLexeme);
                    popedLexeme = PopInFrontDouble(lexemeQueue, 1);
                case HEXADECIMAL:
                case DECIMAL:
                {
                    (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                    (stateMachine->nextShift)[stateMachine->actualCollection] += *(long int*)((LEXEME*)(popedLexeme->data))->value;
                    SECTION section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                    PushQueueDouble(&(collections->collection[stateMachine->actualCollection]), &section, DisplaySection, ErasedSection, sizeof(section));
                    break;
                }
            }
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            break;
        }
        case ASCIZZ:
        {
            LIST_DOUBLE popedLexeme = PopInFrontDouble(lexemeQueue, 1);
            switch (((LEXEME*)(popedLexeme->data))->state)
            {
                default:
                {
                    PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    ErasedListDouble(&popedLexeme);
                    break;
                }
                case RETURN:
                case COMMENT:
                {
                    if(stateMachine->inState != 2)
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    }
                    else
                    {
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                    }
                    ErasedListDouble(&popedLexeme);
                    break;
                }
                case COMMA:
                {
                    if(stateMachine->inState == 2)
                    {
                        stateMachine->inState = 3;
                    }
                    else
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    }
                    ErasedListDouble(&popedLexeme);
                    break;
                }
                case STRING:
                {
                    int stringSize = StringSize((char*)((LEXEME*)(popedLexeme->data))->value)-1;
                    (stateMachine->nextShift)[stateMachine->actualCollection] += stringSize;
                    if(stringSize >= 18)
                        {
                            PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "The size of the string is too big","" ,((LEXEME*)(popedLexeme->data))->type);
                        }
                    if(IsEmptyDouble(*lexemeQueue))
                    {
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                    }
                    SECTION section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                    PushQueueDouble(&(collections->collection[stateMachine->actualCollection]), &section, DisplaySection, ErasedSection, sizeof(section));
                    stateMachine->inState = 2;
                    break;
                }
            }
            if(!stateMachine->inState)
            {
                stateMachine->inState = 1;
            }
            break;
        }
        case WORD:
        {
            LIST_DOUBLE popedLexeme = PopInFrontDouble(lexemeQueue, 1);
            switch (((LEXEME*)(popedLexeme->data))->state)
            {
                default:
                {
                    PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    break;
                }
                case RETURN:
                case COMMENT:
                {
                    if(stateMachine->currentState != 3)
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    }
                    else
                    {
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                    }
                    ErasedListDouble(&popedLexeme);
                    break;
                }
                case COMMA:
                {
                    if(stateMachine->inState == 3)
                    {
                        stateMachine->inState = 4;
                    }
                    else
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    }
                    ErasedListDouble(&popedLexeme);
                    break;
                }
                case SYMBOL:
                {
                    if(stateMachine->inState == 4 || !stateMachine->inState)
                    {
                        (stateMachine->nextShift)[stateMachine->actualCollection] += (4-(stateMachine->nextShift)[stateMachine->actualCollection]%4)%4;
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                        SECTION section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                        PushQueueDouble(&(collections->collection[stateMachine->actualCollection]), &section, DisplaySection, ErasedSection, sizeof(section));
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                    }
                    else
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                        ErasedListDouble(&popedLexeme);
                    }

                    break;
                }
                case OPERATOR:
                {
                    if(stateMachine->inState != 2)
                    {
                        if(*(char*)((LEXEME*)(popedLexeme->data))->value == '-')
                        {
                            stateMachine->inState = 2;
                        }
                        ErasedListDouble(&popedLexeme);

                    }
                    else
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                        ErasedListDouble(&popedLexeme);
                    }
                    break;
                }
                case HEXADECIMAL:
                case DECIMAL:
                {
                    if(stateMachine->inState != 2)
                    {
                        if(*(long int*)((LEXEME*)(popedLexeme->data))->value > INT_MAX)
                        {
                            PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "Value out of range","-2147483648 min, 2147483647 max" ,((LEXEME*)(popedLexeme->data))->type);
                        }
                    }
                    else
                    {
                        if(*(long int*)((LEXEME*)(popedLexeme->data))->value > (unsigned int)-INT_MAX)
                        {
                            PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "Value out of range","-2147483648 min, 2147483647 max" ,((LEXEME*)(popedLexeme->data))->type);
                        }
                        else
                        {
                            *(long int*)((LEXEME*)(popedLexeme->data))->value *= -1;
                        }
                    }
                    (stateMachine->nextShift)[stateMachine->actualCollection] += (stateMachine->nextShift)[stateMachine->actualCollection]%4 == 0 ? 0:4-(stateMachine->nextShift)[stateMachine->actualCollection]%4;
                    (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                    (stateMachine->nextShift)[stateMachine->actualCollection] += 4;

                    SECTION section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                    PushQueueDouble(&(collections->collection[stateMachine->actualCollection]), &section, DisplaySection, ErasedSection, sizeof(section));
                    stateMachine->inState = 3;
                    break;
                }
            }
            if(!stateMachine->inState)
            {
                stateMachine->inState = 1;
            }
            break;
        }
        case BYTE:
        {
            LIST_DOUBLE popedLexeme = PopInFrontDouble(lexemeQueue, 1);
            switch (((LEXEME*)(popedLexeme->data))->state)
            {
                default:
                {
                    PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","" ,((LEXEME*)(popedLexeme->data))->type);
                    break;
                }
                case RETURN:
                case COMMENT:
                {
                    if(stateMachine->inState != 1)
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    }
                    else
                    {
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                    }
                    ErasedListDouble(&popedLexeme);
                    break;
                }
                case COMMA:
                {
                    if(stateMachine->inState == 1)
                    {
                        stateMachine->inState = 3;
                    }
                    else
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    }
                    ErasedListDouble(&popedLexeme);
                    break;
                }
                case SYMBOL:
                {
                    if(!stateMachine->inState)
                    {
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                        SECTION section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                        PushQueueDouble(&(collections->collection[stateMachine->actualCollection]), &section, DisplaySection, ErasedSection, sizeof(section));
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                    }
                    else
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                        ErasedListDouble(&popedLexeme);
                    }
                    break;
                }
                case OPERATOR:
                {
                    if(*(char*)((LEXEME*)(popedLexeme->data))->value == '-')
                    {
                        stateMachine->inState = 2;
                    }
                    ErasedListDouble(&popedLexeme);
                    break;
                }
                case HEXADECIMAL:
                case DECIMAL:
                {
                    if(stateMachine->inState == 1)
                    {
                        PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                    }
                    else
                    {
                        if(stateMachine->inState != 2)
                        {
                            if(((LEXEME*)(popedLexeme->data))->state == DECIMAL)
                            {
                                if (*(long int*)((LEXEME*)(popedLexeme->data))->value > 127)
                                {
                                    PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "Value out of range","-128 min, 127 max" ,((LEXEME*)(popedLexeme->data))->type);
                                }
                            }
                            else
                            {
                                if(*(long int*)((LEXEME*)(popedLexeme->data))->value > 0xff)
                                {
                                    PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "Value out of range","0x00 min, 0xFF max" ,((LEXEME*)(popedLexeme->data))->type);
                                }
                            }
                        }
                        else
                        {
                            if(((LEXEME*)(popedLexeme->data))->state == DECIMAL)
                            {
                                if (*(long int*)((LEXEME*)(popedLexeme->data))->value > 128)
                                {
                                    PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "Value out of range","-128 min, 127 max" ,((LEXEME*)(popedLexeme->data))->type);
                                }
                                else
                                {
                                    *(long int*)((LEXEME*)(popedLexeme->data))->value *= -1;
                                }
                            }
                            else
                            {
                                PrintErrorCollection(stateMachine, ((LEXEME*)(popedLexeme->data))->lineNumber, "Value out of range","0x00 min, 0xFF max" ,((LEXEME*)(popedLexeme->data))->type);
                            }
                        }
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                        SECTION section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                        PushQueueDouble(&(collections->collection[stateMachine->actualCollection]), &section, DisplaySection, ErasedSection, sizeof(section));
                        stateMachine->inState = 1;
                        break;
                    }
                }
            }
            break;
        }
        case SET_NOREORDER:
        {
            if(IsEmptyDouble(*lexemeQueue) || (((LEXEME *)(*lexemeQueue)->data)->state) != SYMBOL)
            {
                PrintErrorCollection(stateMachine, 0, "In directive .set","" ,"");
            }
            else
            {
                char* lexemeValue = (char*)(((LEXEME *)(*lexemeQueue)->data)->value);

                if(strcmp(lexemeValue, "noreorder"))
                    stateMachine->error = 1;

                ErasedInFrontDouble(lexemeQueue);
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = INIT_COLLECTION;
            }
            break;
        }
        case INSTRUCTION0:
        {
            LIST_DOUBLE popedLexeme = PopInFrontDouble(lexemeQueue, 1);
            unsigned long int lineNumber = ((LEXEME*)popedLexeme->data)->lineNumber;
            int isPseudoInstruction = 0;

            if(!IsEmptyDouble(*lexemeQueue) && (((LEXEME *)(*lexemeQueue)->data)->state == STRING || ((LEXEME *)(*lexemeQueue)->data)->state == DIRECTIVE || ((LEXEME *)(*lexemeQueue)->data)->state == COMMA))
            {
                PrintErrorCollection(stateMachine, lineNumber, "You can't use:","now" ,((LEXEME*)(popedLexeme->data))->type);
                ErasedListDouble(&popedLexeme);
                break;
            }
            else
            {
                if(!IsEmptyDouble(*lexemeQueue) && ((LEXEME *)(*lexemeQueue)->data)->state == COLON)
                {
                    SECTION section = CreateLabelSection(*stateMachine, &popedLexeme);
                    if(!AddHashTable(&(collections->labelTable), &section))
                    {
                        PrintErrorCollection(stateMachine, lineNumber, "Two label with the same name","" , (char*)((LEXEME*)popedLexeme->data)->value);
                    
                    }
                    else
                    {
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                    }
                    break;
                }
                else if(stateMachine->actualCollection == TEXT)
                {
                    (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                    int dictionaryIndex = IndexInstruction(instructionDictionary, (char*)((LEXEME*)popedLexeme->data)->value);
                    if(dictionaryIndex == -1)
                    {
                        dictionaryIndex = IndexPseudoInstruction(pseudoDictionary, (char*)((LEXEME*)popedLexeme->data)->value);
                        isPseudoInstruction = 1;
                        if(dictionaryIndex == -1)
                        {
                            PrintErrorCollection(stateMachine, lineNumber, "The instruction","doesn't exist" , (char*)((LEXEME*)popedLexeme->data)->value);
                            ErasedListDouble(&popedLexeme);
                            stateMachine->currentState = INIT_COLLECTION;
                            break;
                        }
                        else
                        {
                            if(pseudoDictionary[dictionaryIndex].typeNumber == 0)
                            {
                                FILE* file = fopen("src/DicoPseudoInstruct.txt", "r");
                                SECTION section;
                                printf("%d\n",FindPseudoInstruction((char*)((LEXEME*)popedLexeme->data)->value, &file, &section));
                                QUEUE_DOUBLE concatenateList = CreateListDouble();
                                CreateNewListLexeme(&file, &concatenateList, &section);
                                ConcatenateListDouble(lexemeQueue, &concatenateList);
                                *lexemeQueue = concatenateList;
                                fclose(file);
                                stateMachine->currentState = INIT_COLLECTION;
                                ErasedListDouble(&popedLexeme);
                                stateMachine->pseudoInstruction = 0;
                                break;
                            }
                        }
                    }
                    
                    stateMachine->previousState = stateMachine->currentState;

                    if(IsEmptyDouble(*lexemeQueue) || ((LEXEME *)(*lexemeQueue)->data)->state == RETURN || ((LEXEME *)(*lexemeQueue)->data)->state == COMMENT)
                    {
                        if(!IsEmptyDouble(collections->collection[stateMachine->actualCollection]->prev) && ((SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data))->data.instruction.lineNumber == lineNumber)
                        {
                            PrintErrorCollection(stateMachine, lineNumber, "You have two instructions on the same line", "", "");
                            ErasedListDouble(&popedLexeme);
                            stateMachine->currentState = INIT_COLLECTION;
                            break;
                        }

                        SECTION section;
                        
                        if(!isPseudoInstruction)
                        {
                            section = CreateInstructionSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], instructionDictionary[dictionaryIndex].id, dictionaryIndex, lineNumber, isPseudoInstruction);
                        }
                        else
                        {
                            section = CreateInstructionSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], pseudoDictionary[dictionaryIndex].id, dictionaryIndex, lineNumber, isPseudoInstruction);
                        }
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                        stateMachine->currentState = INIT_COLLECTION;
                        AddAtLastDouble(&(collections->collection[stateMachine->actualCollection]), &section, DisplaySection, ErasedSection, sizeof(section));
                        ErasedListDouble(&popedLexeme);
                        break;
                    }
                    SECTION section;
                    stateMachine->currentState = INSTRUCTION1;
                    if(!isPseudoInstruction)
                    {
                        section = CreateInstructionSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], instructionDictionary[dictionaryIndex].id, dictionaryIndex, lineNumber, isPseudoInstruction);
                    }
                    else
                    {
                        section = CreateInstructionSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], pseudoDictionary[dictionaryIndex].id, dictionaryIndex, lineNumber, isPseudoInstruction);
                    }
                    AddAtLastDouble(&(collections->collection[stateMachine->actualCollection]), &section, DisplaySection, ErasedSection, sizeof(section));
                    ErasedAtLastDouble(&popedLexeme);
                    break;
                }
                else
                {
                    PrintErrorCollection(stateMachine, lineNumber, "Instruction are available in:", "", collectionSection[TEXT]);
                    ErasedListDouble(&popedLexeme);
                    break;
                }
            }
            break;
        }
        case INSTRUCTION1:
        {
            int nbOperand = 0;
            unsigned long int lineNumber = 0;
            int numberLexemes = 0;
            LIST_DOUBLE lexemeList = NULL;
            if(stateMachine->numberOfInversedLexeme)
            {
                ((SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data))->data.instruction.lowerBits=1;
                stateMachine->numberOfInversedLexeme--;
            }
            do
            {
                numberLexemes = NumberLexemeOperand(*lexemeQueue);
                lexemeList = PopInFrontDouble(lexemeQueue, numberLexemes);


                if(!IsEmptyDouble(lexemeList))
                {
                    if(IsEmptyDouble(*lexemeQueue))
                    {
                        if(!AddOperand(stateMachine, (SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data), lexemeQueue, &lexemeList, instructionDictionary, pseudoDictionary))
                        {
                            PrintErrorCollection(stateMachine, ((LEXEME *)lexemeList->data)->lineNumber, "Error in the instruction:", "", ((SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data))->data.instruction.name);
                            ErasedAtLastDouble(&(collections->collection[stateMachine->actualCollection]));
                            ErasedListDouble(&lexemeList);
                            break;
                        }
                        stateMachine->inState = 0;
                        if(stateMachine->pseudoInstruction)
                        {
                            stateMachine->pseudoInstruction = 0;
                            ErasedAtLastDouble(&(collections->collection[stateMachine->actualCollection]));
                            ErasedListDouble(&lexemeList);
                            stateMachine->previousState = stateMachine->currentState;
                            stateMachine->currentState = INIT_COLLECTION;
                        }
                        nbOperand++;
                    }
                    else if(((LEXEME *)(*lexemeQueue)->data)->state == COMMA || ((LEXEME *)(*lexemeQueue)->data)->state == COMMENT || ((LEXEME *)(*lexemeQueue)->data)->state == RETURN)
                    {
                        if (!AddOperand(stateMachine, (SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data), lexemeQueue, &lexemeList, instructionDictionary, pseudoDictionary))
                        {
                            PrintErrorCollection(stateMachine, ((LEXEME *)lexemeList->data)->lineNumber, "Error in the instruction:", "", ((SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data))->data.instruction.name);
                            ErasedAtLastDouble(&(collections->collection[stateMachine->actualCollection]));
                            break;
                        }
                        else
                        {
                            nbOperand++;
                            if(stateMachine->pseudoInstruction)
                            {
                                stateMachine->pseudoInstruction = 0;
                                ErasedAtLastDouble(&(collections->collection[stateMachine->actualCollection]));
                                stateMachine->previousState = stateMachine->currentState;
                                stateMachine->currentState = INIT_COLLECTION;
                                break;
                            }
                            if(((LEXEME *)(*lexemeQueue)->data)->state == COMMA)
                            {
                                lineNumber = ((LEXEME *)(*lexemeQueue)->data)->lineNumber;
                                stateMachine->inState = !stateMachine->inState;
                                ErasedInFrontDouble(lexemeQueue);
                            }
                            else
                            {
                                stateMachine->inState = 0;
                                stateMachine->previousState = stateMachine->currentState;
                                (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                                (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                                stateMachine->currentState = INIT_COLLECTION;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if(stateMachine->inState)
                    {
                        PrintErrorCollection(stateMachine, lineNumber, "In instruction:", "because of : ','", ((SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data))->data.instruction.name);
                        ErasedAtLastDouble(&(collections->collection[stateMachine->actualCollection]));
                        stateMachine->inState = 2;
                        break;
                    }
                    stateMachine->inState = 0;
                    stateMachine->previousState = stateMachine->currentState;
                    (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                    (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                    stateMachine->currentState = INIT_COLLECTION;
                    nbOperand++;
                    break;
                }
            }
            while(stateMachine->inState != 2 || (!((SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data))->data.instruction.pseudoInstruction && nbOperand != instructionDictionary[((SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data))->data.instruction.dicoIndex].typeNumber) ^ ((SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data))->data.instruction.pseudoInstruction && nbOperand != pseudoDictionary[((SECTION*)(collections->collection[stateMachine->actualCollection]->prev->data))->data.instruction.dicoIndex].typeNumber);
            break;
        }
        default:
        {
            stateMachine->error = 1;
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            break;
        }
    }
}

void PrintErrorCollection(COLLECTION_FSM* stateMachine, unsigned long int lineNumber, char *problem, char* why, char* type)
{
    stateMachine->error = 1;
    if(lineNumber != 0)
        printf("\x1b[31m" "\nERROR: " "\x1b[0m" "%s : ""\x1b[35m" "'%s'" "\x1b[0m" " %s (line ""\x1b[36m" "%lu" "\x1b[0m" ")\n", problem, type, why, lineNumber);
    else
        printf("\x1b[31m" "\nERROR: " "\x1b[0m" "%s : " "\x1b[35m" "'%s'" "\x1b[0m" " %s\n", problem, type, why);
    stateMachine->previousState = stateMachine->currentState;
    stateMachine->currentState = INIT_COLLECTION;
}

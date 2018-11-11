#include "FSMCollection.h"

int InitializationCollection(COLLECTION_LISTS* collectionLists)
{
    collectionLists->labelTable = CreateHashTable();
    if(!collectionLists->labelTable)
        return 0;

    int index;
    for(index = 0; index<3; index++)
    {
        collectionLists->collection[index] = CreateList();
    }
    return 1;
}

void InitializationCollectionFsm(COLLECTION_FSM *stateMachine)
{
    stateMachine->previousState = INIT_COLLECTION;
    stateMachine->currentState = INIT_COLLECTION;
    stateMachine->error = 0;
    stateMachine->inState = 0;
    int index;
    for (index=0; index<3; index++)
    {
        stateMachine->nextShift[index] = 0;
        stateMachine->shift[index] = 0;
    }
}

void InitCollectionFsm(COLLECTION_FSM *stateMachine, QUEUE *lexemeQueue)
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

            printf("\nERROR: You can't use: '%s' here (line %lu)\n",((LEXEME *)(*lexemeQueue)->data)->type, (((LEXEME *)(*lexemeQueue)->data)->lineNumber));
        }
        default:
        {
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            stateMachine->error = 1;
            printf("\nERROR: You can't use: '%s' here (line %lu)\n",((LEXEME *)(*lexemeQueue)->data)->type, (((LEXEME *)(*lexemeQueue)->data)->lineNumber));
            break;
        }
    }
}

void CollectionFsm(COLLECTION_FSM *stateMachine, QUEUE *lexemeQueue, COLLECTION_LISTS* collections, INSTRUCTION* instructionDictionary)
{
    switch (stateMachine->currentState)
    {
        case INIT_COLLECTION:
        {
            InitCollectionFsm(stateMachine, lexemeQueue);
            if (stateMachine->currentState != INIT_COLLECTION)
            {
                stateMachine->inState = 0;
                CollectionFsm(stateMachine, lexemeQueue, collections, instructionDictionary);
                break;
            }
            ErasedInFront(lexemeQueue);
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
                    stateMachine->error = 1;
                    printf("\nERROR: You're not in the correct section for: %s (line %lu)\n",lexemeValue,((LEXEME *)(*lexemeQueue)->data)->lineNumber);
                    stateMachine->previousState = stateMachine->currentState;
                }
                else
                {
                    stateMachine->currentState = SPACE;
                }
                stateMachine->previousState = stateMachine->currentState;
            }
            else if(!strcmp(lexemeValue, ".word"))
            {
                if(stateMachine->actualCollection != DATA)
                {
                    stateMachine->error = 1;
                    printf("\nERROR: You're not in the correct section for: %s (line %lu)\n",lexemeValue,((LEXEME *)(*lexemeQueue)->data)->lineNumber);
                }
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = WORD;
            }
            else if(!strcmp(lexemeValue, ".byte"))
            {
                if(stateMachine->actualCollection != DATA)
                {
                    stateMachine->error = 1;
                    printf("\nERROR: You're not in the correct section for: %s (line %lu)\n",lexemeValue,((LEXEME *)(*lexemeQueue)->data)->lineNumber);
                }
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = BYTE;
            }
            else if(!strcmp(lexemeValue, ".asciiz"))
            {
                if(stateMachine->actualCollection != DATA)
                {
                    stateMachine->error = 1;
                    printf("\nERROR: You're not in the correct section for: %s (line %lu)\n",lexemeValue,((LEXEME *)(*lexemeQueue)->data)->lineNumber);
                }
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = ASCIZZ;
            }
            else
            {
                stateMachine->error = 1;
                printf("\nERROR: Your directive doesn't exist: %s (line %lu)\n",lexemeValue,((LEXEME *)(*lexemeQueue)->data)->lineNumber);
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = INIT_COLLECTION;
            }
            ErasedInFront(lexemeQueue);
            break;
        }
        case SPACE:
        {
            LIST popedLexeme = PopInFront(lexemeQueue, 1);
            switch (((LEXEME*)(popedLexeme->data))->state)
            {
                default:
                    stateMachine->error = 1;
                    printf("\nERROR: You can't use : '%s' in %s (line %lu)\n",((LEXEME*)(popedLexeme->data))->type, collectionType[stateMachine->actualCollection], ((LEXEME*)(popedLexeme->data))->lineNumber);
                    break;
                case OPERATOR:
                    if(*(char*)((LEXEME*)(popedLexeme->data))->value == '-')
                    {
                        stateMachine->error = 1;
                        printf("\nERROR: You can't use : '%s' in %s (line %lu)\n",((LEXEME*)(popedLexeme->data))->type, collectionType[stateMachine->actualCollection], ((LEXEME*)(popedLexeme->data))->lineNumber);
                    }
                    ErasedList(&popedLexeme);
                    popedLexeme = PopInFront(lexemeQueue, 1);
                case HEXADECIMAL:
                case DECIMAL:
                {
                    if(((LEXEME*)(popedLexeme->data))->state == HEXADECIMAL)
                    {
                        unsigned long int value;
                        value = StringHexToDecimal((char *)((LEXEME *)popedLexeme->data));
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += value;
                    }
                    else
                    {
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += *(long int*)((LEXEME*)(popedLexeme->data))->value;
                    }
                    SECTION* section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                    PushQueue(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
                    break;
                }
            }
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            break;
        }
        case ASCIZZ:
        {
            LIST popedLexeme = PopInFront(lexemeQueue, 1);
            switch (((LEXEME*)(popedLexeme->data))->state)
            {
                default:
                {
                    stateMachine->error = 1;
                    printf("\nERROR: You can't use : '%s' in %s (line %lu)\n",((LEXEME*)(popedLexeme->data))->type, collectionType[stateMachine->actualCollection], ((LEXEME*)(popedLexeme->data))->lineNumber);
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
                    break;
                }
                case RETURN:
                case COMMENT:
                {
                    if(stateMachine->inState != 2)
                    {
                        stateMachine->error = 1;
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                    }
                    unsigned long int concatenatedStringSize = ((stateMachine->nextShift)[stateMachine->actualCollection]-(stateMachine->shift)[stateMachine->actualCollection]);
                    if(concatenatedStringSize > 18)
                    {
                        stateMachine->error = 1;
                        printf("\nERROR: The size of the concatenated string is too big : %lu (line %lu)\n",concatenatedStringSize,((LEXEME*)(popedLexeme->data))->lineNumber);                        
                    }
                    ErasedList(&popedLexeme);
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
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
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                        stateMachine->error = 1;
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                    }
                    ErasedList(&popedLexeme);
                    break;
                }
                case STRING:
                {
                    (stateMachine->nextShift)[stateMachine->actualCollection] += StringSize((char*)((LEXEME*)(popedLexeme->data))->value)-1;
                    if(IsEmpty(*lexemeQueue))
                    {
                        unsigned long int concatenatedStringSize = ((stateMachine->nextShift)[stateMachine->actualCollection]-(stateMachine->shift)[stateMachine->actualCollection]);
                        if(concatenatedStringSize > 18)
                        {
                            stateMachine->error = 1;
                            printf("\nERROR: The size of the concatenated string is too big : %lu (line %lu)\n",concatenatedStringSize,((LEXEME*)(popedLexeme->data))->lineNumber);                        
                        }
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                    }
                    SECTION* section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                    PushQueue(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
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
            LIST popedLexeme = PopInFront(lexemeQueue, 1);
            switch (((LEXEME*)(popedLexeme->data))->state)
            {
                default:
                {
                    stateMachine->error = 1;
                    printf("\nERROR: You can't use : '%s' in %s (line %lu)\n",((LEXEME*)(popedLexeme->data))->type, collectionType[stateMachine->actualCollection], ((LEXEME*)(popedLexeme->data))->lineNumber);
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
                    break;
                }
                case RETURN:
                case COMMENT:
                {
                    if(stateMachine->currentState != 3)
                    {
                        stateMachine->error = 1;
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                    }
                    ErasedList(&popedLexeme);
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
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
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                        stateMachine->error = 1;
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                    }
                    ErasedList(&popedLexeme);
                    break;
                }
                case SYMBOL:
                {
                    if(!stateMachine->inState)
                    {
                        (stateMachine->nextShift)[stateMachine->actualCollection] += (stateMachine->nextShift)[stateMachine->actualCollection]%4 == 0 ? 0:4-(stateMachine->nextShift)[stateMachine->actualCollection]%4;
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                        SECTION* section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                        PushQueue(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
                    }
                    else
                    {
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                        ErasedList(&popedLexeme);
                        stateMachine->error = 1;
                    }
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
                    break;
                }
                case OPERATOR:
                {
                    if(stateMachine->inState != 2)
                    {
                        if(*(char*)((LEXEME*)(popedLexeme->data))->value == '-')
                        {
                            (stateMachine->nextShift)[stateMachine->actualCollection] += (stateMachine->nextShift)[stateMachine->actualCollection]%4 == 0 ? 0:4-(stateMachine->nextShift)[stateMachine->actualCollection]%4;
                            (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                            (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                            stateMachine->inState = 2;
                            SECTION* section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                            PushQueue(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
                        }
                        else
                        {
                            ErasedList(&popedLexeme);
                        }
                    }
                    else
                    {
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                        ErasedList(&popedLexeme);
                        stateMachine->error = 1;
                    }
                    break;
                }
                case HEXADECIMAL:
                case DECIMAL:
                {
                    if(stateMachine->inState != 2)
                    {
                        (stateMachine->nextShift)[stateMachine->actualCollection] += (stateMachine->nextShift)[stateMachine->actualCollection]%4 == 0 ? 0:4-(stateMachine->nextShift)[stateMachine->actualCollection]%4;
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                    }
                    SECTION* section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                    PushQueue(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
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
            LIST popedLexeme = PopInFront(lexemeQueue, 1);
            switch (((LEXEME*)(popedLexeme->data))->state)
            {
                default:
                {
                    stateMachine->error = 1;
                    printf("\nERROR: You can't use : '%s' in %s (line %lu)\n",((LEXEME*)(popedLexeme->data))->type, collectionType[stateMachine->actualCollection], ((LEXEME*)(popedLexeme->data))->lineNumber);
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
                    break;
                }
                case RETURN:
                case COMMENT:
                {
                    if(stateMachine->inState != 3)
                    {
                        stateMachine->error = 1;
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                    }
                    ErasedList(&popedLexeme);
                    (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                    (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
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
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                        stateMachine->error = 1;
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                    }
                    ErasedList(&popedLexeme);
                    break;
                }
                case SYMBOL:
                {
                    if(!stateMachine->inState)
                    {
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                        SECTION* section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                        PushQueue(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
                    }
                    else
                    {
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                        ErasedList(&popedLexeme);
                        stateMachine->error = 1;
                    }
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
                    break;
                }
                case OPERATOR:
                {
                    if(stateMachine->inState != 2)
                    {
                        if(*(char*)((LEXEME*)(popedLexeme->data))->value == '-')
                        {
                            (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                            (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                            stateMachine->inState = 2;
                            SECTION* section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                            PushQueue(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
                        }
                        else
                        {
                            ErasedList(&popedLexeme);
                        }
                    }
                    else
                    {
                        printf("\nERROR: You can't use : '%s' now (line %lu)\n",((LEXEME*)(popedLexeme->data))->type,((LEXEME*)(popedLexeme->data))->lineNumber);
                        ErasedList(&popedLexeme);
                        stateMachine->error = 1;
                    }
                    break;
                }
                case HEXADECIMAL:
                case DECIMAL:
                {
                    if(stateMachine->inState != 2)
                    {
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 1;
                    }
                    SECTION* section = CreateDirectiveSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], &popedLexeme);
                    PushQueue(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
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
        case SET_NOREORDER:
        {
            if(IsEmpty(*lexemeQueue) || (((LEXEME *)(*lexemeQueue)->data)->state) != SYMBOL)
            {
                stateMachine->error = 1;
                printf("ERROR: In directive .set\n");
            }
            else
            {
                char* lexemeValue = (char*)(((LEXEME *)(*lexemeQueue)->data)->value);

                if(strcmp(lexemeValue, "noreorder"))
                    stateMachine->error = 1;

                ErasedInFront(lexemeQueue);
            }

            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            break;
        }
        case INSTRUCTION0:
        {
            LIST popedLexeme = PopInFront(lexemeQueue, 1);
            unsigned long int lineNumber = ((LEXEME*)popedLexeme->data)->lineNumber;
            if(!IsEmpty(*lexemeQueue) && (((LEXEME *)(*lexemeQueue)->data)->state == STRING || ((LEXEME *)(*lexemeQueue)->data)->state == DIRECTIVE || ((LEXEME *)(*lexemeQueue)->data)->state == COMMA))
            {
                stateMachine->error = 1;
                ErasedList(&popedLexeme);
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = INIT_COLLECTION;
                break;
            }
            else
            {
                if(!IsEmpty(*lexemeQueue) && ((LEXEME *)(*lexemeQueue)->data)->state == COLON)
                {
                    SECTION* section = CreateLabelSection(*stateMachine, &popedLexeme);
                    if(!AddHashTable(&(collections->labelTable), section))
                    {
                        stateMachine->error = 1;
                        printf("\nERROR: Two label with the same name: '%s' (line %lu)\n", (char*)((LEXEME*)popedLexeme->data)->value, lineNumber);
                    }
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
                    break;
                }
                else if(stateMachine->actualCollection == TEXT)
                {
                    (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                    int dictionaryIndex = IndexInstruction(instructionDictionary, (char*)((LEXEME*)popedLexeme->data)->value);
                    if(dictionaryIndex == -1)
                    {
                        ErasedList(&popedLexeme);
                        stateMachine->error = 1;
                        printf("\nERROR: The instruction '%s' doesn't exist (line %lu)\n", (char*)((LEXEME*)popedLexeme->data)->value, lineNumber);
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                        break;
                    }
                    stateMachine->previousState = stateMachine->currentState;

                    if(instructionDictionary[dictionaryIndex].typeNumber == '0' || IsEmpty(*lexemeQueue) || ((LEXEME *)(*lexemeQueue)->data)->state == RETURN || ((LEXEME *)(*lexemeQueue)->data)->state == COMMENT)
                    {
                        if(((SECTION*)(collections->collection[stateMachine->actualCollection]->data))->data.instruction.lineNumber == lineNumber)
                        {
                            ErasedList(&popedLexeme);
                            stateMachine->error = 1;
                            printf("\nERROR: You have two instructions on the same line (line %lu)\n", lineNumber);
                            stateMachine->previousState = stateMachine->currentState;
                            stateMachine->currentState = INIT_COLLECTION;
                        break;
                        }

                        SECTION* section = CreateInstructionSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], instructionDictionary[dictionaryIndex].id, instructionDictionary[dictionaryIndex].typeNumber,lineNumber);
                        (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];                    
                        (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                        stateMachine->currentState = INIT_COLLECTION;
                        AddInFront(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
                        break;
                    }                
                    stateMachine->currentState = INSTRUCTION1;
                    SECTION* section = CreateInstructionSection(stateMachine->currentState, (stateMachine->shift)[stateMachine->actualCollection], instructionDictionary[dictionaryIndex].id, instructionDictionary[dictionaryIndex].typeNumber,lineNumber);
                    AddInFront(&(collections->collection[stateMachine->actualCollection]), section, DisplaySection, ErasedSection, sizeof(*section));
                    break;
                }
                else
                {
                    printf("\nERROR: Instruction are available in: '%s' (line %lu)\n",collectionSection[TEXT], lineNumber);
                    ErasedList(&popedLexeme);
                    stateMachine->error = 1;
                    stateMachine->previousState = stateMachine->currentState;
                    stateMachine->currentState = INIT_COLLECTION;
                    break;
                }
            }
            break;
        }
        case INSTRUCTION1:
        {
            int nbOperand = '0';
            unsigned long int lineNumber = 0;
            do
            {
                int numberLexemes = NumberLexemeOperand(*lexemeQueue);
                LIST lexemeList = PopInFront(lexemeQueue, numberLexemes);
                if(!IsEmpty(lexemeList))
                {
                    if(IsEmpty(*lexemeQueue))
                    {
                        if(!AddOperand(stateMachine, (SECTION*)(collections->collection[stateMachine->actualCollection]->data), &lexemeList))
                        {
                            printf("\nERROR: Too many operand in instruction: '%s' (line %lu)\n", ((SECTION*)(collections->collection[stateMachine->actualCollection]->data))->data.instruction.name, ((LEXEME *)lexemeList->data)->lineNumber);
                            ErasedInFront(&(collections->collection[stateMachine->actualCollection]));                            
                            ErasedList(&lexemeList);
                            stateMachine->error = 1;
                            stateMachine->previousState = stateMachine->currentState;
                            stateMachine->currentState = INIT_COLLECTION;
                            break;
                        }
                        stateMachine->inState = 0;
                        nbOperand++;
                        ((SECTION*)(collections->collection[stateMachine->actualCollection]->data))->data.instruction.operandNumber = nbOperand;
                    }
                    else if(((LEXEME *)(*lexemeQueue)->data)->state == COMMA || ((LEXEME *)(*lexemeQueue)->data)->state == COMMENT || ((LEXEME *)(*lexemeQueue)->data)->state == RETURN)
                    {
                        if (!AddOperand(stateMachine, (SECTION*)(collections->collection[stateMachine->actualCollection]->data), &lexemeList))
                        {
                            printf("\nERROR: Too many operand in instruction: '%s' (line %lu)\n", ((SECTION*)(collections->collection[stateMachine->actualCollection]->data))->data.instruction.name, ((LEXEME *)lexemeList->data)->lineNumber);
                            ErasedInFront(&(collections->collection[stateMachine->actualCollection]));                            
                            ErasedList(&lexemeList);
                            stateMachine->error = 1;
                            stateMachine->previousState = stateMachine->currentState;
                            stateMachine->currentState = INIT_COLLECTION;
                            break;
                        }
                        else
                        {
                            nbOperand++;
                            if(((LEXEME *)(*lexemeQueue)->data)->state == COMMA)
                            {
                                if(!stateMachine->inState)
                                {
                                    lineNumber = ((LEXEME *)(*lexemeQueue)->data)->lineNumber;
                                    stateMachine->inState = !stateMachine->inState;
                                }
                                ErasedInFront(lexemeQueue);
                            }
                            else
                            {
                                stateMachine->inState = 0;
                                stateMachine->previousState = stateMachine->currentState;
                                (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                                (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                                stateMachine->currentState = INIT_COLLECTION;
                                ((SECTION*)(collections->collection[stateMachine->actualCollection]->data))->data.instruction.operandNumber = nbOperand;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if(stateMachine->inState)
                    {
                        printf("\nERROR: In instruction: '%s' because of : ',' (line %lu)\n", ((SECTION*)(collections->collection[stateMachine->actualCollection]->data))->data.instruction.name, lineNumber);
                        ErasedInFront(&(collections->collection[stateMachine->actualCollection]));
                        stateMachine->error = 1;
                        stateMachine->inState = 2;
                        stateMachine->previousState = stateMachine->currentState;
                        stateMachine->currentState = INIT_COLLECTION;
                        break;
                    }
                    stateMachine->inState = 0;
                    stateMachine->previousState = stateMachine->currentState;
                    (stateMachine->shift)[stateMachine->actualCollection] = (stateMachine->nextShift)[stateMachine->actualCollection];
                    (stateMachine->nextShift)[stateMachine->actualCollection] += 4;
                    stateMachine->currentState = INIT_COLLECTION;
                    nbOperand++;
                    ((SECTION*)(collections->collection[stateMachine->actualCollection]->data))->data.instruction.operandNumber = nbOperand;
                    break;
                }
            }
            while(stateMachine->inState != 2 || nbOperand != ((SECTION*)(collections->collection[stateMachine->actualCollection]->data))->data.instruction.operandNumber);            
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
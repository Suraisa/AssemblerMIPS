#include "FSMCollection.h"

void InitializationCollectionFsm(COLLECTION_FSM *stateMachine)
{
    stateMachine->previousState = INIT_COLLECTION;
    stateMachine->currentState = INIT_COLLECTION;
    stateMachine->error = 0;
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
        default:
        {
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            stateMachine->error = 1;
            break;
        }
    }
}

void CollectionFsm(COLLECTION_FSM *stateMachine, QUEUE *lexemeQueue, COLLECTION_LISTS* collections)
{
    if (IsEmpty(*lexemeQueue))
        return;

    switch (stateMachine->currentState)
    {
        case INIT_COLLECTION:
        {
            InitCollectionFsm(stateMachine, lexemeQueue);
            if (stateMachine->currentState != INIT_COLLECTION)
            {
                CollectionFsm(stateMachine, lexemeQueue, collections);
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
                }
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = SPACE;
            }
            else if(!strcmp(lexemeValue, ".word") || !strcmp(lexemeValue, ".byte"))
            {
                if(stateMachine->actualCollection != DATA)
                {
                    stateMachine->error = 1;
                }
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = WORD_BYTE;
            }
            else if(!strcmp(lexemeValue, ".ascizz"))
            {
                if(stateMachine->actualCollection != DATA)
                {
                    stateMachine->error = 1;
                }
                stateMachine->previousState = stateMachine->currentState;
                stateMachine->currentState = ASCIZZ;
            }
            else
            {
                stateMachine->error = 1;
                stateMachine->currentState = INIT_COLLECTION;
            }
            break;
        }
        case SPACE:
        {
            LEXEME* lexeme = (LEXEME *)(*lexemeQueue)->data;
            if(lexeme->state == HEXADECIMAL || lexeme->state == DECIMAL)
                collections->collection[stateMachine->actualCollection];
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            break;
        }
        case ASCIZZ:
        {
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            break;
        }
        case WORD_BYTE:
        {
            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            break;
        }
        case SET_NOREORDER:
        {
            if((((LEXEME *)(*lexemeQueue)->data)->state))
                stateMachine->error = 1;
            
            char* lexemeValue = (char*)(((LEXEME *)(*lexemeQueue)->data)->value);

            if(!strcmp(lexemeValue, "noreorder"))
                stateMachine->error = 1;

            stateMachine->previousState = stateMachine->currentState;
            stateMachine->currentState = INIT_COLLECTION;
            break;
        }
        case INSTRUCTION0:
        {
            break;
        }
        case INSTRUCTION1:
        {
            break;
        }
        case INSTRUCTION2:
        {
            break;
        }
        case INSTRUCTION3:
        {
            break;
        }
        case INSTRUCTION4:
        {
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
    //printf("\n%s %d\n", (char*)(((LEXEME *)(*lexemeQueue)->data)->value),stateMachine->currentState);
}
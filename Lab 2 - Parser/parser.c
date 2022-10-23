#include "parser.h"
#include <stdio.h>
AT_COMMAND_DATA data;
#define LF 0x0A
#define CR 0x0D

STATE_MACHINE_RETURN_VALUE data_parser(uint8_t currentCharacter)
{
    static State_Machine state = _0_START_STATE;
    // printf("State: %d\n", state);
    // printf("Current character: %d \n", currentCharacter);
    switch (state)
    {
    case _0_START_STATE:
    {
        if (currentCharacter == CR)
            state = _1_FIRST_CR;
        break;
    }
    case _1_FIRST_CR:
    {
        if (currentCharacter == LF)
            state = _2_FIRST_LF;
        else
            return STATE_MACHINE_READY_WITH_ERROR;

        break;
    }
    case _2_FIRST_LF:
    {
        if (currentCharacter == 'O')
            state = _3_OK_O;
        else if (currentCharacter == 'E')
            state = _4_ERROR_E;
        else if (currentCharacter == '+')
            state = _5_FIRST_PLUS;
        else
            return STATE_MACHINE_READY_WITH_ERROR;

        break;
    }
    case _3_OK_O:
    {
        if (currentCharacter == 'K')
            state = 17;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _4_ERROR_E:
    {
        if (currentCharacter == 'R')
            state = _11_ERROR_FIRST_R;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _5_FIRST_PLUS:
    {
        if (32 <= currentCharacter && currentCharacter <= 126)
            state = _6_NEXT_ASCII;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _6_NEXT_ASCII:
    {
        if (currentCharacter == CR)
            state = _7_FIRST_CR_ASCII;
        else if (32 <= currentCharacter && currentCharacter <= 126)
            state = _6_NEXT_ASCII;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _7_FIRST_CR_ASCII:
    {
        if (currentCharacter == LF)
            state = _8_FIRST_LF_ASCII;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _8_FIRST_LF_ASCII:
    {
        if (currentCharacter == CR)
            state = _9_SECOND_CR_ASCII;
        else if (currentCharacter == '+')
            state = _5_FIRST_PLUS;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _9_SECOND_CR_ASCII:
    {
        if (currentCharacter == LF)
            state = _10_SECOND_LF_ASCII;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _10_SECOND_LF_ASCII:
    {
        if (currentCharacter == 'O')
            state = _3_OK_O;
        else if (currentCharacter == 'E')
            state = _4_ERROR_E;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _11_ERROR_FIRST_R:
    {
        if (currentCharacter == 'R')
            state = _12_ERROR_SECOND_R;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _12_ERROR_SECOND_R:
    {
        if (currentCharacter == 'O')
            state = _13_ERROR_O;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _13_ERROR_O:
    {
        if (currentCharacter == 'R')
            state = _14_ERROR_THIRD_R;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _14_ERROR_THIRD_R:
    {
        if (currentCharacter == CR)
            state = _15_ERROR_CR;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _15_ERROR_CR:
    {
        if (currentCharacter == LF)
        {
            state = _0_START_STATE;
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _16_ERROR_LF:
    {
        return STATE_MACHINE_READY_WITH_ERROR;
    }
    case _17_OK_K:
    {
        if (currentCharacter == CR)
            state = _18_OK_CR;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _18_OK_CR:
    {
        if (currentCharacter == LF)
        {
            state = _0_START_STATE;

            return STATE_MACHINE_READY_OK;
        }

        else
            return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case _19_OK_LF:
    {
        return STATE_MACHINE_READY_OK;
    }
    }

    return STATE_MACHINE_NOT_READY;
}
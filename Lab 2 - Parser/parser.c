#include "parser.h"

int charCounter = 0;
AT_COMMAND_DATA data;

#define LF 0x0A
#define CR 0x0D

const char *State_Machine_Enum_Char[] = {
    "0_START_STATE",
    "1_FIRST_CR",
    "2_FIRST_LF",
    "3_OK_O",
    "4_ERROR_E",
    "5_FIRST_PLUS",
    "6_NEXT_ASCII",
    "7_FIRST_CR_ASCII",
    "8_FIRST_LF_ASCII",
    "9_SECOND_CR_ASCII",
    "10_SECOND_LF_ASCII",
    "11_ERROR_FIRST_R",
    "12_ERROR_SECOND_R",
    "13_ERROR_O",
    "14_ERROR_THIRD_R",
    "15_ERROR_CR",
    "17_OK_K",
    "18_OK_CR"};

void print_data_structure(AT_COMMAND_DATA data)
{
    printf("-----Printing data structure-----\n");
    printf("Ok: %d\n", data.ok);
    printf("Line count: %d\n", data.line_count);
    for (int i = 0; i < data.line_count; i++)
    {
        printf("Line %d: %s\n", i, data.data[i]);
    }
}

STATE_MACHINE_RETURN_VALUE data_parser(uint8_t currentCharacter)
{
    static State_Machine state = _0_START_STATE;
    // printf("State: %d\n", state);
    // printf("Current character: %d \n", currentCharacter);

    if (data.line_count < AT_COMMAND_MAX_LINES && charCounter < AT_COMMAND_MAX_LINE_SIZE)
    {

        data.data[data.line_count][charCounter++] = currentCharacter;
        if (currentCharacter == LF)
        {
            data.line_count++;
            charCounter = 0;
        }
    }
    if (charCounter == AT_COMMAND_MAX_LINE_SIZE)
        data.data[data.line_count][charCounter] = '\0';

    switch (state)
    {
    case _0_START_STATE:
    {
        if (currentCharacter == CR)
            state = _1_FIRST_CR;
        else
        {
            printf("Error: Expected CR, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _1_FIRST_CR:
    {
        if (currentCharacter == LF)
            state = _2_FIRST_LF;
        else
        {
            printf("Error: Expected LF, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }

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
        {
            printf("Error: Expected O/E/+, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }

        break;
    }
    case _3_OK_O:
    {
        if (currentCharacter == 'K')
            state = _17_OK_K;
        else
        {
            printf("Error: Expected K, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _4_ERROR_E:
    {
        if (currentCharacter == 'R')
            state = _11_ERROR_FIRST_R;
        else
        {
            printf("Error: Expected R, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _5_FIRST_PLUS:
    {
        if (32 <= currentCharacter && currentCharacter <= 126)
            state = _6_NEXT_ASCII;
        else
        {
            printf("Error: Expected a char between 32 and 126 ASCII, got %d in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _6_NEXT_ASCII:
    {
        if (currentCharacter == CR)
            state = _7_FIRST_CR_ASCII;
        else if (32 <= currentCharacter && currentCharacter <= 126)
            state = _6_NEXT_ASCII;
        else
        {
            printf("Error: Expected CR, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _7_FIRST_CR_ASCII:
    {
        if (currentCharacter == LF)
            state = _8_FIRST_LF_ASCII;
        else
        {
            printf("Error: Expected LF, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _8_FIRST_LF_ASCII:
    {
        if (currentCharacter == CR)
            state = _9_SECOND_CR_ASCII;
        else if (currentCharacter == '+')
            state = _5_FIRST_PLUS;
        else
        {
            printf("Error: Expected CR, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _9_SECOND_CR_ASCII:
    {
        if (currentCharacter == LF)
            state = _10_SECOND_LF_ASCII;
        else
        {
            printf("Error: Expected LF, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _10_SECOND_LF_ASCII:
    {
        if (currentCharacter == 'O')
            state = _3_OK_O;
        else if (currentCharacter == 'E')
            state = _4_ERROR_E;
        else
        {
            printf("Error: Expected O/E, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _11_ERROR_FIRST_R:
    {
        if (currentCharacter == 'R')
            state = _12_ERROR_SECOND_R;
        else
        {
            printf("Error: Expected R, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _12_ERROR_SECOND_R:
    {
        if (currentCharacter == 'O')
            state = _13_ERROR_O;
        else
        {
            printf("Error: Expected O, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _13_ERROR_O:
    {
        if (currentCharacter == 'R')
            state = _14_ERROR_THIRD_R;
        else
        {
            printf("Error: Expected R, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _14_ERROR_THIRD_R:
    {
        if (currentCharacter == CR)
            state = _15_ERROR_CR;
        else
        {
            printf("Error: Expected CR, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _15_ERROR_CR:
    {
        if (currentCharacter == LF)
        {
            state = _0_START_STATE;
            data.ok = 1;
            print_data_structure(data);
            memset(&data, 0, sizeof(data));
            charCounter = 0;
            return STATE_MACHINE_READY_OK;
        }
        else
        {
            printf("Error: Expected LF, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _17_OK_K:
    {
        if (currentCharacter == CR)
            state = _18_OK_CR;
        else
        {
            printf("Error: Expected CR, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case _18_OK_CR:
    {
        if (currentCharacter == LF)
        {
            state = _0_START_STATE;
            data.ok = 1;

            print_data_structure(data);
            memset(&data, 0, sizeof(data));
            charCounter = 0;

            return STATE_MACHINE_READY_OK;
        }

        else
        {
            printf("Error: Expected LF, got %c in state: %s\n", currentCharacter, State_Machine_Enum_Char[state]);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    }

    return STATE_MACHINE_NOT_READY;
}

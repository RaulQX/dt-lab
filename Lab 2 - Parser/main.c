#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

FILE *get_file(char *filename)
{

    // printf("filename: %s\n", filename);
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file %s", filename);
        exit(2);
    }

    return file;
}

void state_machine(FILE *fp)
{
    uint8_t currentCharacter;
    STATE_MACHINE_RETURN_VALUE stateMachineReturnValue;

    while ((currentCharacter = fgetc(fp)) != (uint8_t)EOF)
    {
        stateMachineReturnValue = data_parser(currentCharacter);

        if (stateMachineReturnValue == STATE_MACHINE_NOT_READY)
            continue;
        else if (stateMachineReturnValue == STATE_MACHINE_READY_OK)
        {
            printf("Perfect\n");
            continue;
        }
        else if (stateMachineReturnValue == STATE_MACHINE_READY_WITH_ERROR)
        {
            printf("Not Perfect\n");
            break;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <file>", argv[0]);
        exit(1);
    }

    FILE *fp = get_file(argv[1]);
    if (fp == NULL)
    {
        printf("Error opening file %s", argv[1]);
        exit(2);
    }

    state_machine(fp);

    fclose(fp);

    return 0;
}
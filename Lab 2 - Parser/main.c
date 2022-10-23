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

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <file>", argv[0]);
        exit(1);
    }

    uint8_t currentCharacter;
    STATE_MACHINE_RETURN_VALUE stateMachineReturnValue;
    FILE *fp = get_file(argv[1]);

    while ((currentCharacter = fgetc(fp)) != (uint8_t)EOF)
    {
        stateMachineReturnValue = data_parser(currentCharacter);

        if (stateMachineReturnValue == STATE_MACHINE_NOT_READY)
            continue;
        else if (stateMachineReturnValue == STATE_MACHINE_READY_OK)
        {
            printf("Ok\n");
            continue;
        }
        else if (stateMachineReturnValue == STATE_MACHINE_READY_WITH_ERROR)
        {
            printf("Error\n");
            break;
        }
    }

    fclose(fp);

    return 0;
}
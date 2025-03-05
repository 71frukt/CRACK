#include <stdio.h>
#include "crack.h"

void CrackProg()
{
    FILE *com_file = fopen(COM_FILE_NAME, "rb+");

    fseek(com_file, COMMAND_ADDR, SEEK_SET);
    fwrite(&NEW_COMMAND, 1, 1, com_file);

    return;
}
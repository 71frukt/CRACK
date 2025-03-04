#include <stdio.h>
#include <stdlib.h>

const char *const DEST_FILE_NAME = "../progs_for_crack/args.txt";

const size_t BUFFER_ADDR       = 0x019B;
const size_t RET_IN_STACK_ADDR = 0xFFFC;                          // адрес адреса возврата
const size_t NEW_RET           = 0x0118;

const size_t ZEROS_NUM = RET_IN_STACK_ADDR - BUFFER_ADDR - 1;     // -1 = minus '$'

int main()
{
    FILE *dest = fopen(DEST_FILE_NAME, "wb");
    
    char zero_data[RET_IN_STACK_ADDR - BUFFER_ADDR - 1] = {};
    char enter_ascii = 0x0D;
    
    fwrite("$", 1, 1, dest);
    fwrite(zero_data, 1, ZEROS_NUM, dest);
    fwrite(&NEW_RET, 2, 1, dest);
    fwrite(&enter_ascii, 1, 1, dest);

    return 0;
}
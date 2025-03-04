
#include "TXLib.h"
#include <stdio.h>

#include "graphics.h"

#define BASE_EXE_FILE_NAME  "exe_file.com"



FILE *GetExeFile(const int argc, const char *argv[]);

int main(const int argc, const char *argv[])
{
    fprintf(stderr, "START OF CRACK");

    GraphEngine();
   
    fprintf(stderr, "ENF OF CRACK");
}

FILE *GetExeFile(const int argc, const char *argv[])
{
    FILE *exe_file = NULL;

    if (argc < 2)
        exe_file = fopen(BASE_EXE_FILE_NAME, "w");
    
    else
        exe_file = fopen(argv[1], "w");

    return exe_file;
}
#ifndef CRACK_H
#define CRACK_H

const char *const COM_FILE_NAME = "CRACKEME.com";

const size_t COMMAND_ADDR = 0x76;
const char   NEW_COMMAND  = 0xEB;   // jmp

void CrackProg();

#endif
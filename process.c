#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char* from, char* description){
    printf("[Error] %s : %s\n", from ,description);
    fflush(stdout);
}

void write_memory(ctx* ctx, int addr, char data){
    error("write_memory", "not implemented");
}

char read_memory(ctx* ctx, unsigned int addr){
    if (addr >= 0x100)
        error("read_memory", "invalid memory access");
    return ctx->memory[addr];
}

void load_memory(ctx* ctx, int addr, char* src, unsigned int len){
    if (addr + len >= 0x100)
        error("load_memory", "invalid memory access");
    memcpy(ctx->memory + addr, src, len);
}

void run_process(ctx* ctx, char* program, char* argv[], int argc){
    load_memory(ctx, 0, program, strlen(program));
}
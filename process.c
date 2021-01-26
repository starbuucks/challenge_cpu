#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct __L2cache* llc;

void error(char* from, char* description){
    printf("[Error] %s : %s\n", from ,description);
    fflush(stdout);
}

void write_memory(ctx* ctx, int addr, char data){
    error("write_memory", "not implemented");
}

char read_memory(ctx* ctx, unsigned int addr){
    char ret;
    char tag, idx;
    if (addr >= 0x100)
        error("read_memory", "invalid memory access");
    
    
    return ret;
}

void load_memory(ctx* ctx, int addr, char* src, unsigned int len){
    if (addr + len >= 0x100)
        error("load_memory", "invalid memory access");
    memcpy(ctx->memory + addr, src, len);
}

void run_process(ctx* ctx, llc* l2cache, char* program, char* argv[], int argc){
    int i;
    char op1, op2;

    llc = l2cache;
    ctx->l1cache = getL1cache();

    load_memory(ctx, 0, program, strlen(program));
    for(i=0; i<argc; i++)
        load_memory(ctx, 0x80 + 0x10 * i, argv[i], 0x10);

    while(op1 = read_memory(ctx, ctx->reg.pc++)){
        op2 = read_memory(ctx, ctx->reg.pc++);
        switch(op1 & 0xF0){
            case 0x00:  // exit
            return;

            case 0x10:  // mov reg, reg

            break;

            case 0x20:  // mov reg, mem

            break;

            case 0x30:  // not reg

            break;

            case 0x40:  // add r[0] reg, reg

            break;

            case 0x50:  // sub r[0] reg, reg

            break;

            case 0x60:  // xor r[0] reg, reg

            break;

            case 0x70:  // jz r[0] pc_offset

            break;

            case 0xF0:  // show register status

            break;

            default:
            error("instruction parser", "not allowed instruction");
            return;
        }
    }
}
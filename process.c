#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct __L2cache* llc;

int valid_address(unsigned char addr){
    return addr >= 0x00 && addr < 0x100;
}

void error(char* from, char* description){
    printf("[Error] %s : %s\n", from ,description);
    fflush(stdout);
}

void write_memory(ctx* ctx, unsigned char addr, char data){
    error("write_memory", "not implemented");
}

char read_memory(ctx* ctx, unsigned char addr){
    char data;
    char tag, idx;
    if (!valid_address(addr))
        error("read_memory", "invalid memory access");
    if (!read_cache(ctx->cache, llc, ctx->core, addr, &data)){
        sleep(0.5); // cache miss
        data = ctx->memory[addr];
        load_cache(ctx->cache, llc, ctx->core, addr, data);
    }

    return data;
}

void load_memory(ctx* ctx, unsigned char addr, char* src, unsigned int len){
    if (!valid_address(addr) || !valid_address(addr + len - 1))
        error("load_memory", "invalid memory access");
    memcpy(ctx->memory + addr, src, len);
}

void clflush(ctx* ctx){
    flsuhL1(ctx->cache);
    flushL2(llc);
}

void print_register(ctx* ctx){
    int i;
    printf("[Register Info]\n");
    for (i=0; i<4; i++){
        printf("r%d : %02X\n", i, (unsigned char)ctx->reg.r[i]);
    }
    printf("pc : %02X\n", (unsigned char)ctx->reg.pc);
    fflush(stdout);
}

int need_more_op(unsigned char op1){
    return (op1 >= 0x10 && op1 < 0x90) || (op1 >= 0xd0 && op1 < 0xf0);
}

void run_process(ctx* ctx, l2* l2cache, char* program, char* argv[], int argc){
    int i;
    char op1, op2;

    llc = l2cache;
    ctx->cache = getL1cache();

    load_memory(ctx, 0, program, strlen(program));
    for(i=0; i<argc; i++)
        load_memory(ctx, 0x80 + 0x10 * i, argv[i], 0x10);

    while(op1 = read_memory(ctx, ctx->reg.pc++)){

        if (need_more_op(op1)) op2 = read_memory(ctx, ctx->reg.pc++);
        
        switch(op1 & 0xF0){
            case 0x00:  // exit
            return;

            case 0x10:  // mov reg, reg
            ctx->reg.r[op1 & 0x03] = ctx->reg.r[op2 & 0x03];
            break;

            case 0x20:  // mov reg, mem
            ctx->reg.r[op1 & 0x03] = read_memory(ctx, op2);
            break;

            case 0x30:  // mov mem, reg
            write_memory(ctx, op2, ctx->reg.r[op1 & 0x03]);
            break;

            case 0x40:  // add reg, reg
            ctx->reg.r[op1 & 0x03] += ctx->reg.r[op2 & 0x03];
            break;

            case 0x50:  // sub reg, reg
            ctx->reg.r[op1 & 0x03] -= ctx->reg.r[op2 & 0x03];
            break;

            case 0x60:  // xor reg, reg
            ctx->reg.r[op1 & 0x03] ^= ctx->reg.r[op2 & 0x03];
            break;

            case 0x70: // and reg, reg
            ctx->reg.r[op1 & 0x03] &= ctx->reg.r[op2 & 0x03];
            break;

            case 0x80: // or reg, reg
            ctx->reg.r[op1 & 0x03] |= ctx->reg.r[op2 & 0x03];
            break;

            case 0x90: // clflush
            clflush(ctx);
            break;

            case 0xA0:  // not reg
            ctx->reg.r[op1 & 0x03] = ~ ctx->reg.r[op1 & 0x03];
            break;

            case 0xB0:  // enc reg
            ctx->reg.r[op1 & 0x03]++;
            break;

            case 0xC0:  // dec reg
            ctx->reg.r[op1 & 0x03]--;
            break;

            case 0xD0:  // jz r[0] pc_offset
            if (!ctx->reg.r[0])
                ctx->reg.pc += op2;
            break;

            case 0xE0:  // jnz r[0] pc_offset
            if (ctx->reg.r[0])
                ctx->reg.pc += op2;
            break;

            case 0xF0:  // show register status
            print_register(ctx);
            break;

            default:
            error("instruction parser", "not allowed instruction");
            return;
        }
    }
}
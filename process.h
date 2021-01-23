#pragma once

#pragma pack(push, 1)

struct __reg{
    char r[4];
    char pc;
};

struct __line{
    char tag;       // 0xFF if the line is not used
    char data;
};

struct __cache{
    struct __line line[0x80];
};

#pragma pack(pop)

typedef struct context{
    struct __reg reg;
    struct __cache cache;
    char memory[0x100];
} ctx;

void run_process(ctx* ctx, char* program, char* argv[], int argc);

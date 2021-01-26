#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"

struct __L2cache* getL2cache(){
    int i;
    struct __L2cache* cache = (struct __L2cache*)malloc(sizeof(struct __L2cahce));
    for(i=0; i<0x80; i++){
        cache->line[core] = '\x00';
        cache->line[tag] = '\xFF';
    }
    return cache;
}

struct __L1cache* getL1cache(){
    int i;
    struct __L1cache* cache = (struct __L1cache*)malloc(sizeof(struct __L1cahce));
    for(i=0; i<0x40; i++){
        cache->line[tag] = '\xFF';
    }
    return cache;
}

int readCache(l1* l1cache, l2* l2cache, char core, unsigned int addr, char* out){

    char tag, idx;
    // read l1cache
    tag = (addr & 0xC0) >> 6;
    idx = addr & 0x3F;
    if (l1cache->line[idx].tag == tag){ // cache hit
        *out = l1cache->line[idx].data;
        return CACHE_HIT_L1;
    }
    
    // read l2cache
    tag = (addr & 0x80) >> 7;
    idx = addr & 0x7F;
    if ((l2cache->line[idx].core & core) && (l2cache->line[idx].tag == tag)){   // cache hit
        *out = l2cache->line[idx].data;
        return CACHE_HIT_L2;
    }

    return CACHE_MISS;
}
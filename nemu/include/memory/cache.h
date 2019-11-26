#ifndef __CACHE_H__
#define __CACHE_H__
#include <stdint.h>
#include <stddef.h>
#include "nemu.h"

typedef struct 
{
    uint8_t valid;
    uint32_t sign;
    uint8_t data[64];
}CacheLine;

extern CacheLine cache[1024];   //Eight-way group connection will be implemented in the cache.c
 
void init_cache();
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine * cache);
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine * cache);

#endif
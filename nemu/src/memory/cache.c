#include "memory/memory.h"
#include "memory/cache.h"

CacheLine cache[1024];

void init_cache(){
	int i;
	for (i = 0; i < 1024; i++) {
		cache[i].valid = false;
	}
}

uint32_t cache_read(paddr_t paddr, size_t len, CacheLine * cache)
{
	static seed = 0;

	uint32_t res = 0, suf=0;
	uint32_t tag = paddr & 0xffffe000;
	uint32_t group = paddr & 0x1fc0;
	group >>= 6;
	uint32_t block_addr = paddr & 0x3f;
	uint32_t blockline = 0;


}

void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine * cache);
#include "memory/memory.h"
#include "memory/cache.h"

CacheLine cache[1024];

void init_cache(){
	int i;
	for (i = 0; i < 1024; i++) {
		cache[i].valid = false;
	}
}


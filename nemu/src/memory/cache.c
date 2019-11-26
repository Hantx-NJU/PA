#include "memory/memory.h"
#include "memory/cache.h"

CacheLine cache[1024];

void init_cache(){
	int i;
	for (i = 0; i < 1024; i++) {
		cache[i].valid = false;
	}
}

int min(int a, int b){
	return (a < b ? a : b);
}

uint32_t cache_read(paddr_t paddr, size_t len, CacheLine * cache)
{
	static uint32_t seed = 0;
	++seed;
	if(seed > 100000)	seed=0;

	uint32_t res = 0, suf=0;
	uint32_t tag = paddr & 0xffffe000, tag_suf =(paddr + len)&0xffffe000;
	uint32_t group = paddr & 0x1fc0;
	group >>= 6;
	uint32_t block_addr = paddr & 0x3f;
	uint32_t blockline = 0;

	int suf_len = len + block_addr - 64;
	bool flag_cr=false;	//if cross row then set flag_cr true
	//Cross Row
	if(tag!=tag_suf){
		flag_cr = true;
		suf = cache_read(paddr + len - suf_len, suf_len, cache);
	}

	//bool flag = false;	//if hit then set flag true

	//Judge if hit
	for(int i = 0; i < 8; ++i)
	{
		if(cache[group * 8 + i].valid){
			if(cache[group * 8 + i].sign == tag){
				//now hit
				if(flag_cr)
				{
					memcpy(&res,cache[group*8 + i].data + block_addr, len - suf_len);
					res = res << (8*suf_len) + suf;
					return res;
				}
				else{
					memcpy(&res,cache[group*8 + i].data + block_addr, len);
					return res;
				}
			}
		}
	}

	//find empty block to load new data
	for(int i = 0; i < 8; ++i)
	{
		if(!cache[group*8 + i].valid)
		{
			cache[group*8 + i].valid = true;
			cache[group*8 + i].sign = tag;
			memcpy(cache[group*8 + i].data, hw_mem + paddr, 64);
			memcpy(&res,cache[group*8 + i].data + block_addr, len);
			return res;
		}
	}

	//now we must replace one block to load new--->blockline = seed % 8
	blockline = seed % 8;
	cache[group*8 + blockline].sign = tag;
	memcpy(cache[group*8 + blockline].data, hw_mem + paddr, 64);
	memcpy(&res,cache[group*8 + blockline].data + block_addr, len);
	return res;
}

void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine * cache)
{
	uint32_t suf=0;
	uint32_t tag = paddr & 0xffffe000, tag_suf =(paddr + len)&0xffffe000;
	uint32_t group = paddr & 0x1fc0;
	group >>= 6;
	uint32_t block_addr = paddr & 0x3f;
}
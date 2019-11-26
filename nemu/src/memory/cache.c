#include "memory/memory.h"
#include "memory/cache.h"


CacheLine cache[1024];

void init_cache(){
	for (int i = 0; i < 1024; ++i) {
		cache[i].valid = 0;
	}
}



uint32_t cache_read(paddr_t paddr, size_t len, CacheLine * cache)
{
	static int seed = 0;
	++seed;
	if(seed > 100000)	seed = 0;

	uint32_t res = 0;
	//memcpy(&res, hw_mem + paddr, len);
	//return res;

	uint32_t tag = paddr & 0xffffe000;
	uint32_t group = paddr & 0x1fc0;
	group = group >> 6;
	uint32_t block_addr = (paddr & 0x3f);
	int blockline = -1;


	//bool flag = false;	//if hit then set flag true

	//Judge if hit
	for(int i = 0; i < 8; ++i)
	{
		if(cache[group * 8 + i].valid == 1){
			if(cache[group * 8 + i].sign == tag){
				//now hit
				if(block_addr + len > 64)
				{
					uint32_t suf = 0;
					memcpy(&res, (cache[group*8 + i].data + block_addr), 64-block_addr);
					suf = cache_read(paddr + 64 - block_addr, block_addr + len - 64, cache);
					res = res + (suf << (8*(64 - block_addr)));
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
		if(cache[group*8 + i].valid == 0)
		{
			blockline = i;
			break;
		}
	}

	//now we must replace one block to load new--->blockline = seed % 8
	if(blockline == -1)	blockline = seed % 8;
	cache[group*8 + blockline].valid = 1;
	cache[group*8 + blockline].sign = tag;
	memcpy(cache[group*8 + blockline].data, hw_mem + paddr - block_addr, 64);
	//memcpy(&res,cache[group*8 + blockline].data + block_addr, len);
	memcpy(&res, hw_mem + paddr, len);
	return res;
}

void cache_write(paddr_t paddr,size_t len,uint32_t data , CacheLine* cache)
{
    uint32_t group_number=(paddr>>6)&0x7f;
    uint32_t sign_number=(paddr>>13)&0x7ffff;
    uint32_t offset_number= paddr&0x3f;
    memcpy(hw_mem+paddr,&data,len);
    for(int i=0;i<8;i++)
    {
        if(cache[group_number*8+i].valid==1
           &&cache[group_number*8+i].sign==sign_number)
        {
            if(offset_number+len<=64)
            {
                memcpy(cache[group_number*8+i].data+offset_number,&data,len);
            }
            else
            {
                cache_write(paddr,64-offset_number,data,cache);
                cache_write(paddr-offset_number+64,len-64+offset_number,data>>(8*(64-offset_number)),cache);
            }
            break;
        }
    }
}
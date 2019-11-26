#include "memory/memory.h"
#include "memory/cache.h"

CacheLine cache[1024];

void init_cache(){
	for (int i = 0; i < 1024; ++i) {
		cache[i].valid = false;
	}
}



uint32_t cache_read(paddr_t paddr, size_t len, CacheLine * cache)
{
	static uint32_t seed = 0;
	++seed;
	if(seed > 100000)	seed = 0;

	uint32_t res = 0, suf = 0;
	//memcpy(&res, hw_mem + paddr, len);
	//return res;

	uint32_t tag = paddr & 0xffffe000;
	uint32_t group = paddr & 0x1fc0;
	group >>= 6;
	uint32_t block_addr = (paddr & 0x3f);
	uint32_t blockline = 0;

	int suf_len = len + block_addr - 64;
	bool flag_cr = false;	//if cross row then set flag_cr true
	//Cross Row
	if((paddr&0xffffffc0)!=((paddr+len)&0xffffffc0)){
		flag_cr = true;
		suf = cache_read(paddr + len - suf_len, suf_len, cache);
	}

	//bool flag = false;	//if hit then set flag true

	//Judge if hit
	for(int i = 0; i < 8; ++i)
	{
		if(cache[group * 8 + i].valid == true){
			if(cache[group * 8 + i].sign == tag){
				//now hit
				if(flag_cr == true)
				{
					memcpy(&res, (cache[group*8 + i].data + block_addr), len - suf_len);
					//res = (res << (8*suf_len)) + suf;
					res = res + (suf << (8*(len-suf_len)));
					//memcpy(&res, hw_mem + paddr, len);
					return res;
				}
				else{
					memcpy(&res,cache[group*8 + i].data + block_addr, len);
					//return res;
					//memcpy(&res, hw_mem + paddr, len);
					return res;
				}
			}
		}
	}
	
	//find empty block to load new data
	for(int i = 0; i < 8; ++i)
	{
		if(cache[group*8 + i].valid == false)
		{
			cache[group*8 + i].valid = true;
			cache[group*8 + i].sign = tag;
			memcpy(cache[group*8 + i].data, (hw_mem + (paddr&0xffffffc0)), 64);
			if(flag_cr)
				{
					memcpy(&res, (cache[group*8 + i].data + block_addr), len - suf_len);
					res = res + (suf << (8*(len-suf_len)));
					//res = (res << (8*suf_len)) + suf;
					return res;
					//memcpy(&res, hw_mem + paddr, len);
					//return res;
				}
			else{
					memcpy(&res, (cache[group*8 + i].data + block_addr), len);
					return res;
					//memcpy(&res, hw_mem + paddr, len);
					//return res;
				}
			//memcpy(&res,cache[group*8 + i].data + block_addr, len);
			//return res;
		}
	}
//memcpy(&res, hw_mem + paddr, len);
//	return res;
	//now we must replace one block to load new--->blockline = seed % 8
	blockline = seed % 8;
	cache[group*8 + blockline].sign = tag;
	memcpy(cache[group*8 + blockline].data, hw_mem + (paddr&0xffffffc0), 64);
	//memcpy(&res,cache[group*8 + blockline].data + block_addr, len);
	if(flag_cr == true)
		{
			memcpy(&res, (cache[group*8 + blockline].data + block_addr), len - suf_len);
			//res = (res << (8*suf_len)) + suf;
			res = res + (suf << (8*(len-suf_len)));
			return res;
		}
	else{
			memcpy(&res, cache[group*8 + blockline].data + block_addr, len);
			return res;
		}
	return res;
}

void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine * cache)
{
	uint32_t tag = paddr & 0xffffe000;
	uint32_t group = paddr & 0x1fc0;
	group >>= 6;
	uint32_t block_addr = paddr & 0x3f;

	int suf_len = len + block_addr - 64,read_len = len - suf_len;
	bool flag_cr = false;
	//Cross Row
	if((paddr&0xffffffc0)!=((paddr+len)&0xffffffc0)){
		flag_cr =true;
		if(len == 2)
		{
			cache_write(paddr + 1, 1, data & 0xf, cache);
			len = 1;
			data >>= 8;
		}
		else if(len == 4)
		{
			if(read_len == 1)
			{
				cache_write(paddr + read_len, suf_len, data & 0xfff, cache);
				len = read_len;
				data >>= 24;
			}
			else if(read_len == 2)
			{
				cache_write(paddr + read_len, suf_len, data & 0xff, cache);
				len = read_len;
				data >>= 16;
			}
			else if(read_len == 3)
			{
				cache_write(paddr + read_len, suf_len, data & 0xf, cache);
				len = read_len;
				data >>= 8;
			}
		}
	}

	//write through
	for(int i = 0; i < 8; ++i)
	{
		if(cache[group * 8 + i].valid){
			if(cache[group * 8 + i].sign == tag){
				memcpy(cache[group*8 + i].data + block_addr, &data, len);
				memcpy(hw_mem + paddr, &data, len);
			}
		}
	}
}
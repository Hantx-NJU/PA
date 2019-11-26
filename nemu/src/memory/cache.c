#include"memory/cache.h"
#include "nemu.h"
#include "memory/memory.h"
#include <memory.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
void init_cache()
{
    for(int i=0;i<1024;i++)
    {
       
            cache[i].valid=0;
        
    }
}
uint32_t cache_read(paddr_t paddr,size_t len, CacheLine* cache)
{
    uint32_t group_number=(paddr>>6)&0x7f;
    uint32_t sign_number=(paddr>>13)&0x7ffff;
    uint32_t offset_number=paddr&0x3f ;
    uint32_t rst;
    for(int i=0;i<8;i++)
    {
        if(cache[group_number*8+i].valid==1
           &&cache[group_number*8+i].sign==sign_number)
        {
            if(offset_number+len<=64)
            {
                memcpy(&rst,cache[group_number*8+i].block+offset_number,len);
                return rst;

            }
            else
            {
                uint32_t tmp1=0,tmp2=0;
                memcpy(&tmp1,cache[group_number*8+i].block+offset_number,64-offset_number);
                tmp2=cache_read(paddr-offset_number+64,len+offset_number-64,cache);
                rst=tmp1|(tmp2<<(64-offset_number)*8);
                return rst;
            }
            
        }
    }

    //miss!
    bool have_empty=false;
    memcpy(&rst,hw_mem+paddr,len);
    int j=0;
    for(;j<8;j++)
    {
        if(cache[group_number*8+j].valid==0)
        {
            have_empty=true;
            break;
        }
    }

    if(have_empty==false) 
    {
        srand((unsigned)time(0));
        j=rand()%8;
    }
    cache[group_number*8+j].valid=1;
    cache[group_number*8+j].sign=sign_number;
    memcpy(cache[group_number*8+j].block,hw_mem+paddr-offset_number,64);
    
    return rst;
    

}
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine * cache)
{
	for(int j=0;j<len;++j){
		uint32_t tag = paddr & 0xffffe000;
		uint32_t group = paddr & 0x1fc0;
		group >>= 6;
		uint32_t block_addr = paddr & 0x3f;

		//write through
		for(int i = 0; i < 8; ++i)
		{
			if(cache[group * 8 + i].valid){
				if(cache[group * 8 + i].sign == tag){
					uint32_t temp = data & 0xff;
					memcpy(cache[group*8 + i].data + block_addr, &temp, 1);
					memcpy(hw_mem + paddr, &temp, 1);
					data >>= 8;
				}
			}
		}
		++paddr;
	}
}
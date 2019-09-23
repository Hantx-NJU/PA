#include "cpu/cpu.h"

//***************ADD*********************

void set_CF_add(uint32_t result, uint32_t src, size_t data_size)
{
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result < src;
}

void set_ZF(uint32_t result, size_t data_size)
{
	result = result & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.ZF = (result == 0);
}


// SF and OF contain information relevant to signed integers
void set_SF(uint32_t result, size_t data_size)
{
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF = sign(result);
}

void set_PF(uint32_t result) { 
	int num = 0;
	for(int i = 0;i < 8; ++i){
		if((result & 0x1) == 1)
			++num;
		result = result >> 1;
	}
	if(num % 2 == 0)
		cpu.eflags.PF = 1;
	else
		cpu.eflags.PF = 0;
}

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
	switch(data_size)
       	{
		case 8:
			result = sign_ext(result & 0xFF, 8);
			src = sign_ext(src & 0xFF, 8);
			dest = sign_ext(dest & 0xFF, 8);
			break;
		case 16:
			result = sign_ext(result & 0xFFFF, 16);
			src = sign_ext(src & 0xFFFF, 16);
			dest = sign_ext(dest & 0xFFFF, 16);
			break;
		default: break;// do nothing
}
	if(sign(src) == sign(dest)) {
	if(sign(src) != sign(result))
		cpu.eflags.OF = 1;
	else
		cpu.eflags.OF = 0;
	} else {
	cpu.eflags.OF = 0;
} }

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
//#ifdef NEMU_REF_ALU
//	return __ref_alu_add(src, dest, data_size);
//#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	//return 0;
//#endif
	uint32_t res = 0;
	res = dest + src;
	
	set_CF_add(res, src, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));
}

//*******************ADC*************************

void set_CF_adc(uint32_t result, uint32_t src, size_t data_size)
{
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(cpu.eflags.CF == 1 && result <= src)
		cpu.eflags.CF = 1;
	else
		cpu.eflags.CF = result < src;
}

int set_OF_adc(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
	switch(data_size)
       	{
		case 8:
			result = sign_ext(result & 0xFF, 8);
			src = sign_ext(src & 0xFF, 8);
			dest = sign_ext(dest & 0xFF, 8);
			break;
		case 16:
			result = sign_ext(result & 0xFFFF, 16);
			src = sign_ext(src & 0xFFFF, 16);
			dest = sign_ext(dest & 0xFFFF, 16);
			break;
		default: break;// do nothing
}
	if(sign(src) == sign(dest)) {
	if(sign(src) != sign(result))
		return 1;
	else
		return 0;
	} else {
	return 0;
}
/*	if(cpu.eflags.CF == 1){
		if(data_size == 8 && src + dest ==0xff)
			cpu.eflags.OF = 1;
		else if(data_size == 16 && src + dest ==0xffff)
			cpu.eflags.OF = 1;
		else if(data_size == 32 && src + dest ==0xffffffff)
			cpu.eflags.OF = 1;
	}
*/
}


uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
        res = dest + src + cpu.eflags.CF;
//	if(cpu.eflags.CF == 1)
//		cpu.eflags.OF = set_OF_adc(src + dest, src, dest, data_size) || set_OF_adc(res, src + dest , 1, data_size);
//	else
	cpu.eflags.OF =	set_OF_adc(res, src, dest, data_size);
        set_CF_adc(res, src, data_size);
        set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

        return res & (0xFFFFFFFF >> (32 - data_size));

}

//*********************SUB******************

void set_CF_sub(uint32_t src, uint32_t dest, size_t data_size)
{
	src = sign_ext(src& (0xFFFFFFFF >> (32 - data_size)), data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF =  (src  > dest);
	
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
	switch(data_size)
       	{
		case 8:
			result = sign_ext(result & 0xFF, 8);
			src = sign_ext(src & 0xFF, 8);
			dest = sign_ext(dest & 0xFF, 8);
			break;
		case 16:
			result = sign_ext(result & 0xFFFF, 16);
			src = sign_ext(src & 0xFFFF, 16);
			dest = sign_ext(dest & 0xFFFF, 16);
			break;
		default: break;// do nothing
}
	if(sign(src) != sign(dest)) {
	if(sign(src) == sign(result))
		cpu.eflags.OF = 1;
	else
		cpu.eflags.OF = 0;
	} else {
	cpu.eflags.OF = 0;
}
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
        res = dest - src;
	
	set_OF_sub(res, src, dest, data_size);	
	set_CF_sub(src, dest, data_size);
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));

}

//**********************SBB*********************

void set_CF_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
	src = sign_ext(src& (0xFFFFFFFF >> (32 - data_size)), data_size);
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(cpu.eflags.CF)
		cpu.eflags.CF =  (src  >= dest);
	else
		cpu.eflags.CF =  (src  > dest);
	
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
        res = dest - src - cpu.eflags.CF;
//	if(cpu.eflags.CF == 0){
	set_OF_sub(res, src, dest, data_size);	
//	}
//	else{
			
//	}
	set_CF_sbb(src, dest, data_size);
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));

}


void set_CF_mul(uint64_t res, size_t data_size)
{
	res = res & (0xFFFFFFFFFFFFFFFF >> (64 - data_size * 2));
	switch(data_size)
       	{
		case 8:
			if((res & 0xFF00) == 0){
				cpu.eflags.CF = 0;
				cpu.eflags.OF = 0;
			}
			else{
				cpu.eflags.CF = 1;
				cpu.eflags.OF = 1;
			}
			break;
		case 16:
			if((res & 0xFFFF0000) == 0){
				cpu.eflags.CF = 0;
				cpu.eflags.OF = 0;
			}
			else{
				cpu.eflags.CF = 1;
				cpu.eflags.OF = 1;
			}
			break;
			
		default:
			if((res & 0xFFFFFFFF00000000) == 0){
				cpu.eflags.CF = 0;
				cpu.eflags.OF = 0;
			}
			else{
				cpu.eflags.CF = 1;
				cpu.eflags.OF = 1;
			}
			break;       
	}
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
	uint64_t s =(uint64_t)src;
	uint64_t d =(uint64_t)dest;
	uint64_t res = s * d;
//	res * = dest;
/*	for(int i = 0; i < data_size; ++i)
	{
		if((dest & 1) == 1)
			res += src * 2;
		else{}
		dest = dest >> 1;
		src = src * 2;
	}*/
	set_CF_mul(res, data_size);
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFFFFFFFFFF >> (64 - data_size*2));
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
	int64_t s =(int64_t)src;
	int64_t d =(int64_t)dest;
	int64_t res = s * d;
	
	return res;
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
	if(src == 0)
		return 0;
	else{}
	uint32_t res = dest / src;
	return res;
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
	if(src == 0)
		return 0;
	else{}
	uint32_t res = dest / src;
	return res;

}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
	return dest % src;
}

int32_t alu_imod(int64_t src, int64_t dest)
{
	return dest % src;
}

//********************AND****************
uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
        res = src & dest;

	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));

	
}

//*****************XOR******************

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
        res = src ^ dest;

	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));

}

//********************OR***********************

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
        res = src | dest;

	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));


}

//*********************SHL*********************

void set_CF_shl(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t temp = dest << (32 - data_size);
	temp = temp << (src - 1);
	cpu.eflags.CF = sign(temp);
	//cpu.eflags.CF = sign(dest << (data_size - src));
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
        res = dest << src;
	
	set_CF_shl(src, dest, data_size);
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));
}

//*******************SHR*******************
//
void set_CF_shr(uint32_t src, uint32_t dest, size_t data_size)
{
	cpu.eflags.CF = (dest >> (src - 1)) & 0x1;
	//cpu.eflags.CF = sign(dest << (data_size - src));
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
        res = dest;
	for(int i = 0;i < src; ++i)
	{
		res = res >> 1;
		switch(data_size)
       		{
		case 8:
			res = res & 0xFFFFFF7F;
			break;
		case 16:
			res = res & 0xFFFF7FFF;
						
			break;
		default:
			res = res & 0x7FFFFFFF;
			       
			break;}

	}	
	set_CF_shr(src, dest, data_size);
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));

}

//**********************SAR******************

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
	res = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
        if(sign(res))
		{
			for(int i = 0;i < src; ++i)
			{
				res = res >> 1;
			switch(data_size)
       			{
				case 8:
					res = res | 0x80;
					break;
				case 16:
					res = res | 0x8000;
						
					break;
				default:
					res = res | 0x80000000;
			       
					break;}}
		}
		else
		{
			for(int i = 0;i < src; ++i)
			{
				res = res >> 1;
			switch(data_size)
       			{
				case 8:
			
					res = res & 0x7F;
					break;
				case 16:
					res = res & 0x7FFF;
					break;
				default:
					res = res & 0x7FFFFFFF;
				       
					break;}}
		}
	
	set_CF_shr(src, dest, data_size);
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));

}

//********************SAL***********************


uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
        res = dest << src;
	
	set_CF_shl(src, dest, data_size);
	set_PF(res);
        set_ZF(res, data_size);
        set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size));

}

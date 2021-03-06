#include "cpu/instr.h"

make_instr_func(lgdt)
{
	int len = 1;
	OPERAND src;

	len += modrm_rm(eip+1, &src);
	src.data_size = 16;
	operand_read(&src);
	cpu.gdtr.limit = src.val;

	src.sreg = SREG_DS;

	src.addr += 2;

	if(data_size == 16)
	{
		src.data_size = 28;
		operand_read(&src);
		cpu.gdtr.base = src.val;
	}
	else
	{
		src.data_size = 32;
		operand_read(&src);
		cpu.gdtr.base = src.val;
	}
	return len;
}

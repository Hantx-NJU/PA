#include "cpu/instr.h"

make_instr_func(lgdt)
{
	int len = 1;
	OPERAND src;

	len += mod_rm(eip+1, &src);
	src.data_size = 16;
	operand_read(&src);
	cpu.gdrt.limit = src.val;

	src.addr += 2;

	if(data_size == 16)
	{
		src.data_size = 28;
		operand_read(&src);
		cpu.gdrt.base = src.val;
	}
	else
	{
		src.data_size = 32;
		operand_read(&src);
		cpu.gdrt.base = src.val;
	}
}

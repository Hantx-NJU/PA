#include "cpu/instr.h"

static void instr_execute_1op()
{
	OPERAND temp;
	modrm_rm(eip+1, &temp);
	temp.data_size = data_size;
	operand_read(&temp);
	temp.val = ~temp.val;
	operand_write(&temp);
}

make_instr_impl_1op(not, rm, v);

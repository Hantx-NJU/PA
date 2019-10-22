#include "cpu/instr.h"

static void instr_execute_1op()
{
	OPERAND temp;
	operand_read(&opr_src);
	cpu.esp -= data_size / 8;
	temp.type=OPR_MEM;
	temp.val = opr_src.val;
	temp.addr = cpu.esp;
	operand_write(&temp);
}

make_instr_impl_1op(push,r,v);
	

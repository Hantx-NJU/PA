#include "cpu/instr.h"

static void instr_execute_1op()
{
	OPERAND temp;
	temp.type = OPR_MEM;
	temp.data_size = data_size;
	temp.addr = cpu.esp;
	temp.sreg = SREG_SS;
	operand_read(&temp);

	temp.sreg = SREG_DS;
	opr_src.val = temp.val;
	operand_write(&opr_src);
	cpu.esp += data_size/8;
}

make_instr_impl_1op(pop, r, v);

make_instr_func(popa)
{
	
}










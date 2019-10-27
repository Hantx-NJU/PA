#include "cpu/instr.h"

static void instr_execute_1op()
{
	OPERAND temp;
	temp.type = OPR_MEM;
	temp.data_size = data_size;
	temp.addr = cpu.esp;
	operand_read(&temp);
	cpu.esp += data_size/8;
}

make_instr_impl_1op(pop, r, v);












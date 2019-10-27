#include "cpu/instr.h"

make_instr_func(leave)
{
	OPERAND temp;
	temp.val = cpu.ebp;
	temp.val = sign_ext(temp.val, data_size);
	cpu.esp = temp.val;

	temp.type = OPR_MEM;
	temp.data_size = data_size;
	temp.addr = cpu.esp;
	operand_read(&temp);
	cpu.esp += data_size/8;
	cpu.ebp = temp.val;
	return 0;
}

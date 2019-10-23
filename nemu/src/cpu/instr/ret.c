#include "cpu/instr.h"

make_instr_func(ret_near)
{
	//EIP <- POP()
	OPERAND temp;
	temp.type = OPR_MEM;
	temp.data_size = data_size;
	temp.addr = cpu.esp;
	operand_read(&temp);
	cpu.esp += data_size/8;
	cpu.eip = temp.val;
	return 1+data_size/8;
}

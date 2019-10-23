#include "cpu/instr.h"

make_instr_func(call_near)
{
	//PUSH(EIP)
	OPERAND rel;
	rel.type = OPR_IMM;
	rel.addr = cpu.eip + 1;
	rel.data_size = data_size;
	operand_read(&rel);
	cpu.esp -= data_size / 8;

	temp.val=cpu.ip;
	cpu.esp-=data_size / 8;
	temp.addr=cpu.esp;
	temp.data_size=data_size;
	temp.type=OPR_MEM;
	operand_write(&temp);
	operand_read(&opr_src);
	cpu.eip=(cpu.eip + opr_src.val)&0x0000FFFF;
	return 1+data_size/8;
}


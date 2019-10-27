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
	return 0;
}

make_instr_func(ret_near_imm16)
{
	//EIP <- POP()
	OPERAND temp;
	temp.type = OPR_MEM;
	temp.data_size = data_size;
	temp.addr = cpu.esp;
	operand_read(&temp);
	cpu.esp += data_size/8;
	cpu.eip = temp.val;

	OPERAND imm;
	modrm_rm(cpu.eip+1, &imm);
	imm.data_size = data_size;
	operand_read(&imm);
	cpu.esp += imm;
	return 0;
}

#include "cpu/instr.h"

make_instr_func(ret_near)
{
	//EIP <- POP()
	OPERAND temp;
	temp.type = OPR_MEM;
	temp.data_size = data_size;
	temp.sreg = SREG_DS;

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
	temp.sreg = SREG_DS;

	temp.data_size = data_size;
	temp.addr = cpu.esp;
	operand_read(&temp);
	cpu.esp += data_size/8;
	cpu.eip = temp.val;

	OPERAND imm;
	modrm_rm(cpu.eip+1, &imm);
	imm.data_size = data_size;
	operand_read(&imm);
	cpu.esp += imm.val;
	return 0;
}

void basic_pop(uint32_t* val)
{
    OPERAND temp;
    temp.type = OPR_MEM;
    temp.data_size = data_size;
    temp.addr = cpu.esp;
    temp.sreg = SREG_CS;
    operand_read(&temp);
    cpu.esp += data_size / 8;
    *val =  temp.val;
}

make_instr_func(iret)
{
	//EIP <- POP()
	OPERAND temp;
	temp.type = OPR_MEM;
	temp.sreg = SREG_DS;

	temp.data_size = data_size;
	temp.addr = cpu.esp;
	operand_read(&temp);
	cpu.esp += data_size/8;
	cpu.eip = temp.val;

	OPERAND imm;
	modrm_rm(cpu.eip+1, &imm);
	imm.data_size = data_size;
	operand_read(&imm);
	cpu.esp += imm.val;
	return 0;
}

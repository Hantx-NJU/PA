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

static uint32_t basic_pop()
{
    OPERAND temp;
    temp.type = OPR_MEM;
    temp.data_size = data_size;
    temp.addr = cpu.esp;
    temp.sreg = SREG_CS;
    operand_read(&temp);
    cpu.esp += data_size / 8;
    return temp.val;
}

make_instr_func(iret)
{
	cpu.eip=basic_pop();
    cpu.cs.val=basic_pop();
    cpu.eflags.val=basic_pop();

    return 0;
}

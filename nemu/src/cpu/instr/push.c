#include "cpu/instr.h"

static void instr_execute_1op()
{
	OPERAND temp;
	operand_read(&opr_src);
	cpu.esp -= data_size / 8;
	temp.type=OPR_MEM;
	temp.sreg = SREG_SS;

	temp.val = opr_src.val;
	temp.val = sign_ext(temp.val, data_size);
	temp.addr = cpu.esp;
	temp.data_size=data_size;
	operand_write(&temp);
}

make_instr_impl_1op(push,r,v);
make_instr_impl_1op(push,rm,v);
make_instr_impl_1op(push,i,b);
make_instr_impl_1op(push,i,v);

void basic_push(uint32_t val)
{
	OPERAND temp;
	//operand_read(&opr_src);
	cpu.esp -= data_size / 8;
	temp.type=OPR_MEM;
	temp.sreg = SREG_CS;

	temp.val = val;
	temp.val = sign_ext(temp.val, data_size);
	temp.addr = cpu.esp;
	temp.data_size=data_size;
	operand_write(&temp);
}
make_instr_func(pusha)
{
	OPERAND esp;
    esp.type = OPR_MEM;
    esp.data_size = data_size;
    esp.addr = cpu.esp;
    esp.sreg = SREG_CS;
    operand_read(&esp);
    basic_push(cpu.eax);
    basic_push(cpu.ecx);
    basic_push(cpu.edx);
    basic_push(cpu.ebx);
    basic_push(esp.val);
    basic_push(cpu.ebp);
    basic_push(cpu.esi);
    basic_push(cpu.edi);
	return 1;
}
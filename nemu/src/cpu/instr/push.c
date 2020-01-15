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
		
make_instr_func(pusha)
{
	OPERAND esp;
    esp.type = OPR_MEM;
    esp.data_size = data_size;
    esp.addr = cpu.esp;
    esp.sreg = SREG_CS;
    operand_read(&esp);
    push(cpu.eax);
    push(cpu.ecx);
    push(cpu.edx);
    push(cpu.ebx);
    push(esp.val);
    push(cpu.ebp);
    push(cpu.esi);
    push(cpu.edi);
	return 1;
}
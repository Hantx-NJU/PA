#include "cpu/instr.h"

static void instr_execute_1op()
{
	OPERAND temp;
	temp.type = OPR_MEM;
	temp.data_size = data_size;
	temp.addr = cpu.esp;
	temp.sreg = SREG_CS;
	operand_read(&temp);

	temp.sreg = SREG_DS;
	opr_src.val = temp.val;
	operand_write(&opr_src);
	cpu.esp += data_size/8;
}

make_instr_impl_1op(pop, r, v);

void basic_pop(uint32_t* val)
{
    OPERAND temp;
    temp.type = OPR_MEM;
    temp.data_size = data_size;
    temp.addr = cpu.esp;
    temp.sreg = SREG_CS;
    operand_read(&temp);
    cpu.temp += data_size / 8;
    val =  temp.val;
}

make_instr_func(popa)
{
	basic_pop(&cpu.edi);
	basic_pop(&cpu.esi);
	basic_pop(&cpu.ebp);
	basic_pop(&cpu.ebx);
	basic_pop(&cpu.ebx);
	basic_pop(&cpu.edx);
	basic_pop(&cpu.ecx);
	basic_pop(&cpu.eax);
	
	return 1;
}










#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void raise_intr_push(uint32_t val)
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
void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
//	printf("Please implement raise_intr()");
//	assert(0);
raise_intr_push(cpu.eflags.val);
raise_intr_push(cpu.cs.val);
raise_intr_push(cpu.eip);

uint32_t des[2];
dex[0] = laddr_read(cpu.idtr.base + (intr_no << 3), 4);

#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}

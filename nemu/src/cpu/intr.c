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
des[0] = laddr_read(cpu.idtr.base + (intr_no << 3), 4);
des[1] = laddr_read(cpu.idtr.base + (intr_no << 3) + 4, 4);

uint32_t new_cs = (des[0]&0xffff0000)>>16;
uint32_t new_eip = (des[1]&0xffff0000)|(des[0]&0xffff);

cpu.cs.val=new_cs;
cpu.eip=new_eip;

load_sreg(SREG_CS);

int type = (des[1]>>8)&0xf;
if(type==0xe)	cpu.eflags.IF = 0;
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}

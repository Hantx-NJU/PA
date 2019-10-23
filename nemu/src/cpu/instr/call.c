#include "cpu/instr.h"

/*make_instr_func(call_near)
{
		OPERAND temp;
		temp.val=cpu.ip;
		cpu.esp-=data_size / 8;
		temp.addr=cpu.esp;
		temp.data_size=data_size;
		temp.type=OPR_MEM;
		operand_write(&temp);
		operand_read(&opr_src);
		cpu.eip=(cpu.eip + opr_src.val)&0x0000FFFF;
		return 1+data_size/8;
}*/
make_instr_func(call_near)
{	OPERAND rel;
    rel.type=OPR_IMM;
    rel.sreg=SREG_CS;
    rel.data_size=data_size;
    rel.addr=eip+1;
    operand_read(&rel);

    cpu.gpr[4].val-=data_size/8;
    OPERAND mem;
    mem.type=OPR_MEM;
    mem.data_size=data_size;
    mem.val=cpu.eip+1+data_size/8;
    mem.sreg=SREG_SS;
    mem.addr=cpu.gpr[4].val;
    operand_write(&mem);
    int offset =sign_ext(rel.val,data_size);

    print_asm_1("call","",1+data_size/8,&rel);
    cpu.eip+=offset;
    return 1+data_size/8;
    }

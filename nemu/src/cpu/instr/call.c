#include "cpu/instr.h"

make_instr_func(call_near)
{
	//PUSH(EIP)
	OPERAND rel, mem;
	rel.type = OPR_IMM;
	rel.addr = cpu.eip + 1;
	rel.data_size = data_size;
	operand_read(&rel);
	cpu.esp -= data_size / 8;
	
	mem.type = OPR_MEM;
	mem.data_size = data_size;
	mem.val = cpu.eip + 1 + data_size / 8;
	mem.addr = cpu.esp;
	operand_write(&mem);

	//EIP <- EIP + rel
	int rel_s = sign_ext(rel.val, data_size);
	cpu.eip += rel_s;

	return 1 + data_size / 8;
}

make_instr_func(call_near_indirect)
{
	OPERAND rm;
    rm.data_size=data_size;
    modrm_rm(eip+1,&rm);
    operand_read(&rm);
    cpu.gpr[4].val-=data_size/8;
    OPERAND mem;
    mem.type=OPR_MEM;
    mem.data_size=data_size;
    mem.val=cpu.eip+1+data_size/8;
    mem.sreg=SREG_SS;
    mem.addr=cpu.gpr[4].val;
    operand_write(&mem);
    if(data_size==16)
    rm.val&=0xFFFF;
    print_asm_1("call","",1+data_size/8,&rm);
    cpu.eip=rm.val;
    return 0;
}


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
	//PUSH(EIP)
	OPERAND rel, mem;
	modrm_rm(eip+1,&rel);	
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
	cpu.eip = rel_s;

	return 0;
}


#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_short)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, 8);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + 8 / 8, &rel);

        cpu.eip += offset;

        return 1 + 8 / 8;
}

make_instr_func(jmp_near_indirect)
{
        OPERAND rel;
        //rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        //rel.addr = eip + 1;
	modrm_rm(eip + 1, &rel);
	
	operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip = offset;

        return 0;
}

make_instr_func(jmp_far_imm)
{
	OPERAND src, dest;
	src.type = OPR_IMM;
	dest.type = OPR_IMM;
	src.data_size = 32;
	dest.data_size = 16;
	src.addr = eip + 1;
	dest.addr = eip + 5;

        src.sreg = SREG_CS;
        dest.sreg = SREG_CS;

	operand_read(&src);
	operand_read(&dest);

	cpu.eip = src.val;
	cpu.cs.val = dest.val;

	return 0;
}



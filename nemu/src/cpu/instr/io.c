#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(in_b) {
    int len = 1;
	cpu.gpr[0]._8[0] = (uint8_t)pio_read(cpu.gpr[2]._16, 1);
	return len;
}

make_instr_func(in_v) {
    int len = 1;
	cpu.eax = pio_read(cpu.gpr[2]._16, 4);
	return len;
}

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_src.val=sign_ext(opr_src.val,opr_src.data_size);
	opr_dest.val=sign_ext(opr_dest.val,opr_dest.data_size);
	alu_sub(opr_src.val,opr_dest.val,data_size);
}
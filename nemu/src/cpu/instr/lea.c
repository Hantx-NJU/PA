#include "cpu/instr.h"

make_instr_func(lea)
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = opr_src.addr;
	opr_dest.val = sign_ext(opr_dest.val, opr_dest.data_size);
	operand_write(&opr_dest);
	return 1 + data_size / 8;
}

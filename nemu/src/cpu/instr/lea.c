#include "cpu/instr.h"

make_instr_func(lea)
{
	OPERAND r, m;
	r.data_size = data_size;
	int len = 1;
	len += modrm_rm(eip + 1, &r);
	len += modrm_rm(eip + len, &m);
	r.val = m.addr;
	r.val = sign_ext(r.val, r.data_size);
	operand_write(&r);
	return len;
}

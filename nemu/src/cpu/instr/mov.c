#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_rm2s_w) {
	int len = 1;
	uint32_t data = instr_fetch(eip +1 ,1);
	++len;
	OPERAND reg, sreg;
	reg.type = OPR_REG;
	reg.data_size = 16;
	reg.addr = data & 7;
	sreg.type = OPR_SREG;
	sreg.data_size = 16;
	sreg.addr = (data >> 3) &7;
	
	operand_read(&reg);
	sreg.val = reg.val;
	operand_write(&sreg);

	load_sreg(sreg.addr);
	return len;
}

make_instr_func(mov_c2r_l) {
	int len = 1;
	OPERAND cr, r;
	
	
	r.data_size = 32;
	cr.data_size = 32;
	len += modrm_r_rm(eip + 1, &cr ,&r);
	cr.type = OPR_CREG;
	//r.type = OPR_REG;
	operand_read(&cr);
	//uint32_t safe = 0x1;
	r.val = cr.val;
	//r.val = safe;
	operand_write(&r);
	return len;
}

make_instr_func(mov_r2c_l) {
	int len = 1;
	OPERAND cr, r;
	//c.type = OPR_CREG;
	//r.type = OPR_REG;
	cr.data_size = 32;
	r.data_size = 32;

	len += modrm_r_rm(eip + 1, &cr ,&r);
	cr.type = OPR_CREG;
	//r.type = OPR_REG;
	operand_read(&r);
	cr.val = r.val;
	//c.val = 0x1;
	operand_write(&cr);
	return len;
}

#include "cpu/instr.h"
#include "device/port_io.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
	opr_dest.val = sign_ext(opr_dest.val, opr_dest.data_size);
	opr_dest.val = alu_add(opr_src.val, opr_dest.val, data_size);
	operand_write(&opr_dest);
}
make_instr_func(in_b)
{
    int len = 1;
    OPERAND DX, AX;

    DX.addr = REG_DX;
    DX.type = OPR_REG;
    DX.data_size = data_size;

    AX.addr = REG_AX;
    AX.type = OPR_REG;
    AX.data_size = 8;

    operand_read(&DX);
    // printf("dx = %x\nRead dx val = %x\n", cpu.edx, DX.val);
    AX.val = pio_read(DX.val, DX.data_size / 8);
    operand_write(&AX);

    return len;
}

make_instr_func(in_v)
{
    int len = 1;
    OPERAND DX, AX;

    DX.addr = REG_DX;
    DX.type = OPR_REG;
    DX.data_size = data_size;

    AX.addr = REG_AX;
    AX.type = OPR_REG;
    AX.data_size = data_size;

    operand_read(&DX);
    // printf("dx = %x\nRead dx val = %x\n", cpu.edx, DX.val);
    AX.val = pio_read(DX.val, DX.data_size / 8);
    operand_write(&AX);

    return len;
}
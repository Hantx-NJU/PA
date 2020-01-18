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
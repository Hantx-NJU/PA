#include "cpu/instr.h"

make_instr_func(test_r2rm_v)
{
	return 1+data_size/8;
}

#include "cpu/instr.h"

make_instr_func(int_)
{
    uint8_t ans = instr_fetch(eip + 1, 1);
    raise_sw_intr(ans);
    return 0;
}

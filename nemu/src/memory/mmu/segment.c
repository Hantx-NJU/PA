#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */

	return cpu.segReg[sreg].base + offset;
	//return 0;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	SegDesc s;
	uint32_t base = (s.base_31_24 << 24) + (s.base_23_16 << 16) + s.base_15_0;
	uint32_t limit = (s.limit_19_16 << 16) + s.limit_15_0;

	laddr_t index = cpu.gdtr.base + cpu.segReg[sreg].index * sizeof(SegDesc);
	s.val[0] = laddr_read(index, 4);
	s.val[1] = laddr_read(index + 4, 4);

	assert(base == 0);
	assert(limit == 0);
	assert(s.present != 0);
	assert(s.granularity != 0);

	cpu.segReg[sreg].privilege_level = s.privilege_level;
	cpu.segReg[sreg].base = base;
	cpu.segReg[sreg].limit = limit;
}

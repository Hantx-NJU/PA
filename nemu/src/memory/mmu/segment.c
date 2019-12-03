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
	SegDesc des;

	assert(cpu.segReg[sreg].ti != 1);

	laddr_t gdtindex = cpu.gdtr.base + cpu.segReg[sreg].index * sizeof(SegDesc);
	des.val[0] = laddr_read(gdtindex, 4);
	des.val[1] = laddr_read(gdtindex + 4, 4);

	assert(des.base_15_0 == 0 && des.base_23_16 == 0 && des.base_31_24 == 0);
	assert(des.limit_15_0 == 0xffff && des.limit_19_16 == 0xf);
	assert(des.present != 0);
	assert(des.granularity != 0);

	cpu.segReg[sreg].type = des.type;
	cpu.segReg[sreg].privilege_level = des.privilege_level;
	cpu.segReg[sreg].soft_use = des.soft_use;
	cpu.segReg[sreg].base = (des.base_31_24 << 24) + (des.base_23_16 << 16) + des.base_15_0;
	cpu.segReg[sreg].limit = (des.limit_19_16 << 16) + des.limit_15_0;
}

#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	uint32_t dir = (laddr >> 22) & 0x3ff;
	uint32_t page = (laddr >> 12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;

	assert(PDE.present == 1);

	uint32_t pde_index = (dir<<2) + (cpu.cr3.pdbr<<12);
	uint32_t pde = paddr_read(pde_index, 14;
	

	//printf("\nPlease implement page_translate()\n");
	//assert(0);
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
	;
#endif
}

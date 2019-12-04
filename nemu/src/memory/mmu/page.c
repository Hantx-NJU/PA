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

	uint32_t pde_addr = (dir<<2) + (cpu.cr3.pdbr<<12);
	uint32_t pde = paddr_read(pde_addr, 4);
	uint32_t pte_addr = (page << 2) + (pde & 0xfffff000);
	uint32_t pte = paddr_read(pte_addr, 4);
	assert(PDE.present == 1);

	return offset + (pte & 0xfffff000);
	//printf("\nPlease implement page_translate()\n");
	//assert(0);
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
	;
#endif
}

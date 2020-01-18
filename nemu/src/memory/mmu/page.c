#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	uint32_t dir4 = (laddr & 0xffc00000) >> 20;
	uint32_t page4 = (laddr & 0x003ff000) >> 10;
	uint32_t offset = (laddr & 0x00000fff);
	uint32_t pde1 = (cpu.cr3.pdbr << 12) + dir4;
	pde1 = paddr_read(pde1, 4);
	if ((pde1 & 1) == 0)
		assert(0);
	uint32_t pte1 = (pde1 & 0xfffff000) + page4;
	pte1 = paddr_read(pte1, 4);
	if ((pte1 & 1) == 0)
		assert(0);
	uint32_t padd = (pte1 & 0xfffff000) + offset;
	return padd;
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);;
#endif
}

/*paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	uint32_t dir = (laddr >> 22) & 0x3ff;
	uint32_t page = (laddr >> 12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;

	PDE* pde = (PDE*)((dir<<2) + (cpu.cr3.pdbr<<12) + hw_mem);
	//uint32_t pde_addr = (dir<<2) + (cpu.cr3.pdbr<<12);
	//uint32_t pde = paddr_read(pde_addr, 4);
	//uint32_t pde = hw_mem_read(pde_addr, 4);
	assert(pde->present == 1);

	PTE* pte = (PTE*)((page << 2) + (pde->page_frame << 12) + hw_mem);
	//uint32_t pte_addr = (page << 2) + (pde & 0xfffff000);
	//uint32_t pte_addr = (page << 2) + (pde << 12);
	//uint32_t pte = paddr_read(pte_addr, 4);
	//uint32_t pte = hw_mem_read(pte_addr, 4);
	assert(pte->present== 1);

	return (offset + (pte->page_frame << 12));
	//printf("\nPlease implement page_translate()\n");
	//assert(0);
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
	;
#endif
}
*/
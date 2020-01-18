#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
typedef union {
	struct
	{
		uint32_t offset : 12;
		uint32_t page_index : 10;
		uint32_t dic_index : 10;
	};
	uint32_t val;
} laddr_analyse;
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	laddr_analyse l;
	l.val = laddr;
	PDE *pde_header = (void *)(hw_mem + (cpu.cr3.PDBR << 12));
	PDE target_dic = pde_header[l.dic_index];
	// assert(pde_header->present);
	PTE *pte_header = (void *)(hw_mem +
							   (target_dic.page_frame << 12));
	PTE target_tbl = pte_header[l.page_index];
	assert(target_tbl.present);
	return (target_tbl.page_frame << 12) + l.offset;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
	;
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
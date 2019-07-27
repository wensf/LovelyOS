#include <types.h>
#include <page.h>
#include <init.h>
#include <printk.h>
#include <sched.h>
#include <libc.h>
#include <traps.h>
#include <page.h>

extern struct kernel_param *kparam;


extern unsigned long vaddr;

unsigned char mem_map[PAGING_MEM_ITEM];
unsigned long low_memory_start, low_memory_end;


int do_page_map ( uint32 pgd, uint32 pa, uint32 va, int attr );

int page_attrs_set( uint32 pgd, uint32 va, int attrs )
{
	uint32 *pdir, *ptable;
	uint32 pdir_entry;
	uint32 ppte_entry;
	
	pdir = (uint32*) pgd;
	pdir_entry = va >> 22;
	ppte_entry = (va>>12) & 0x3FF;

	if ( *(pdir+pdir_entry) == 0 ){
		kernel_die("page_attrs_set() null\n");
		return (-1);
	}
	ptable = (uint32*)(*(pdir+pdir_entry) & 0xFFFFF000);
	*(ptable+ppte_entry) &= ~0x7;
	*(ptable+ppte_entry) |=  attrs;

	// update cr3
#if 1
	__asm__ __volatile__("movl %%eax,%%cr3"::"a" (pgd));
#endif	

	return 0;
}

int do_page_map ( uint32 pgd, uint32 pa, uint32 va, int attr )
{
	uint32 *pdir = (uint32 *)pgd;
	uint32 *ptable;

	uint32 pdir_entry = va >> 22;
	uint32 ppte_entry = (va >> 12 ) & 0x3FF;

	if ( *(pdir + pdir_entry ) == 0 )
	{
		ptable = (uint32 *) get_free_page();
		memset(ptable,0,PAGE_SIZE);
		*(pdir + pdir_entry )  = ((uint32)ptable & 0xFFFFF000) + attr;
	}else{
		ptable = (uint32*)(*(pdir + pdir_entry ) & 0xFFFFF000);
	}

	*(ptable+ppte_entry) = (pa & 0xFFFFF000) + attr;

#if 0
	if ( current ){
		printk("pgd=%08x,dir=[%d],pte=[%d],pa=%08x,va=%08x\n",pgd, pdir_entry,ppte_entry,pa,va);
	}
#endif

	// update cr3
#if 1
	__asm__ __volatile__("movl %%eax,%%cr3"::"a" (pgd));
#endif		

	return (0);
}


int page_map( uint32 pgd, uint32 pa, uint32 va, int size, int attr )
{
	if ( size < 4096 ) size = 4096;
	
	for ( int i = 0; i < size/4096; i++ )
	{
		do_page_map( pgd, pa+i*4096, va+i*4096, attr );
	}	
	
	return 0;
}

int do_wp_page(uint32 error_code,uint32 addr)
{
	unsigned long old_page, new_page;
	
	printk("task[%d] do_wp_page() addr = 0x%08x, error_code=0x%08x\n",current->pid,addr, error_code);

	old_page = addr & 0xFFFFF000;
	if ( old_page>=low_memory_start && mem_map[MAP_NR(addr)] == 1 )
	{	
		printk("set old page 0x%08x to [rw]\n", old_page);
		return page_attrs_set( current->pgd, old_page, P_P|P_US|P_RW);
	}
	new_page = get_free_page();
	if ( !new_page ){
		kernel_die("do_wp_page");
	}
	memcpy((char*)new_page, (char*)old_page, PAGE_SIZE);
	if ( new_page >= low_memory_start ){
		mem_map[MAP_NR(new_page)]--;
	}
	printk("map new page 0x%08x to 0x%08x, [rw]\n",new_page,old_page);
	do_page_map ( current->pgd, new_page, old_page, P_P|P_RW|P_US );	
	__asm__ __volatile__("movl %%eax,%%cr3"::"a" (current->pgd));

	return (0);
}


int bread_page(  char *b, unsigned long addr, unsigned long size )
{
	unsigned long offset = 0x800000+0*512; // 程序入口在偏移0*512处
	offset = offset + (addr - 0x2000000);
	printk("copy text segment from ram_disk offset 0x%08x\n", offset);
	memcpy(b,( unsigned char *)offset,PAGE_SIZE);
			
	return (0);
}

int do_no_page(uint32 error_code,uint32 addr)
{
	unsigned long old_page, new_page;

	printk("task[%d] do_no_page() addr = 0x%08x, error_code=0x%08x\n",current->pid,addr, error_code);	

	old_page = addr & 0xFFFFF000;
	new_page = get_free_page();
	if ( !new_page ){
		kernel_die("get_free_page()\n");
	}
	if ( old_page >= 0x02000000 && old_page < 0x04000000 )
	{
		// text & rodata & data
		printk("text & rodata & data\n");

		bread_page((char*)new_page,old_page,PAGE_SIZE);
		
	}else if ( old_page >= 0x04000000 && old_page < 0x08000000){
		// stack
		printk("stack\n");
	}else{
	//	task_dump(current);
	//	kernel_die("error address");
	}
	do_page_map ( current->pgd, new_page, old_page, P_P|P_RW|P_US );

	return (0);
}

/**
 * 页表初始化.
 * 将8MB的显存空间映射到其对应的物理地址上.
 * 显存地址一般是动态的,在boot阶段用BIOS调用获得并保存在
 * kparam->vaddr变量中.
 */
unsigned long vaddr;

void page_init(void)
{
	uint32 size;

	/**
	 * mapping the Vedio RAM addr, size = 8MB.
	 */
	 
	vaddr = kparam->vaddr;//VEDIO_VADDR;	// vedio virtual address set to 0x1000000(16MB)	 
	// vaddr = VEDIO_VADDR;	// vedio virtual address set to 0x1000000(16MB)	 
	size = kparam->yres * kparam->xres * (kparam->bpp/8);
	
	page_map( KERNEL_PAGE_DIR_ADDR, kparam->vaddr, vaddr, size, P_P|P_RW|P_US);
}


int mem_init( unsigned long mem_start, unsigned long mem_end )
{
    int i;

	low_memory_start = mem_start;
	low_memory_end = mem_end;
	
    /**
     * make all main memory eara to used firstly.
     */
    for ( i = 0; i < PAGING_MEM_ITEM; i++)
    {
        mem_map[i] = MEM_USED;
    }

    /**
     * make available memory eara to 0.
     */
    for ( i = 0; i < (mem_end-mem_start)/PAGE_SIZE; i++)
    {
        mem_map[i] = 0;
    }

    return 0;
}

/**
 * To get a free page from physical memory, and mark it
 * used. If no free pages left, return 0.
 */
unsigned long get_free_page(void)
{
    int i;

    for ( i = 0; i < PAGING_MEM_ITEM; i++)
    {
        if ( !mem_map[i] )
        {
            mem_map[i]++;
            return low_memory_start + (i* 4096);
        }
    }

    return 0;
}

unsigned long get_free_pages( int page_nr )
{
	int flg = 0, cnt = 0, pidx = 0;

	for ( int i = 0; i < PAGING_MEM_ITEM && !flg; i++ )
	{
		if ( mem_map[i] ){ continue; }

		cnt = 1;
		for ( int j = i+1, k = 1; (k < page_nr) && (j < PAGING_MEM_ITEM); j++,k++ )
		{
			if ( !mem_map[j] )
			{
				cnt++;
			}else{
                i = j;
				break;
			}

			if ( cnt == page_nr )
			{
				pidx = i; flg = 1;
				break;
			}
		}
	}

	if ( flg == 1 )
	{
		for ( int i = pidx; i < pidx+page_nr; i++ )
		{
			mem_map[i]++;
		}

//		printk("alloc page start %08x, page_nr=%d\n", MEM_LOW + (pidx* 4096), page_nr);
		return low_memory_start + (pidx* 4096);
	}else{
		kernel_die("get_free_pages() oom!!!!\n");
		return (0);
	}
}

/**
 * To free a page, make it unused.
 * @addr the page base address.
 * @return none
 */

void free_page( unsigned long addr )
{
    addr -= low_memory_start;
    addr >>= 12;

    if ( addr < PAGING_MEM_ITEM )
    {
        if ( mem_map[addr] > 0 ){ mem_map[addr]-- ;}
    }else
    {
        kernel_die("free_page() 0x%08x\n", addr);
    }
}

/**
 * To free pages, make it unused.
 * @addr the page base address.
 * @page_nr the number of page.
 * @return none
 */
void free_pages( unsigned long addr, int page_nr )
{
	unsigned long va;
	int i;

	i = 0; va = addr;
	while ( i < page_nr )
	{
		// va -= MEM_LOW;
		va -= low_memory_start;
		va >>= 12;
		if ( mem_map[va] >0 ) mem_map[va]--;
		i++;
		va = addr + i*4096;
	}
}


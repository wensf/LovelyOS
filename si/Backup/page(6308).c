#include <types.h>
#include <mm/page.h>
#include <init.h>
#include <printk.h>
#include <memory.h>
#include <sched.h>
#include <libc.h>
#include <page.h>

extern struct kernel_param *kparam;


extern unsigned long vaddr;

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
		return (-1);
	}
	ptable = (uint32*)(*(pdir+pdir_entry) & 0xFFFFF000);
	*(ptable+ppte_entry) &= ~0xFFF;
	*(ptable+ppte_entry) |=  (P_P|P_US);

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
		*(pdir + pdir_entry )  = ((uint32)ptable & 0xFFFFF000) + attr;
	}else{
		ptable = (uint32*)(*(pdir + pdir_entry ) & 0xFFFFF000);
	}

	*(ptable+ppte_entry) = (pa & 0xFFFFF000) + attr;

#if 0
	printk("pgd=%08x,dir=[%d],pte=[%d],pa=%08x,va=%08x\n",pgd, pdir_entry,ppte_entry,pa,va);
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


void do_page_fault(void)
{
	uint32 va;
	__asm__ __volatile__ ("movl %%cr2,%%eax":"=a"(va));

	printk("task[%d] page_fault va at %08x\n", current->pid, va);

	va &= 0xFFFFF000;
	// 申请一块内存

	// 判断一下va是属于哪类地址: 0内核空间(),1用户空间,2其他
    // 子类：代码，数据，栈
    // 用户空间：代码：分配新页，并从RAM_DISK读取一页
    //           数据: 分配新页，并复制其父进程的页内容
    //           栈：  分配新页


	uint32 pa = get_free_page();
	
	do_page_map( current->pgd, pa, va, P_P|P_RW|P_US );

	if ( va >= 0x1000000 ){		// 16MB

		// 用户空间缺页
		
		if ( va >= 0x4000000 ){	// >=64MB 堆栈空间
			// 从RAM_DISK内存段中载入新进程的代码.先这样做吧，后续实现文件系统以后，改为从文件系统中读取可执行文件.
			// 此处新进程由loader加载在主内存临时区8M附近.
			printk("on user stack\n");
			memset((char*)pa, 0, PAGE_SIZE);
		}else{					// >=32<64MB 代码空间(和数据空间，为简单起见)
			uint32 offset = 0x800000+0*512; // 程序入口在偏移0*512处
			offset = offset + (va - 0x2000000);
			printk("copy text segment from ram_disk offset %08x\n", offset);
			memcpy((int8*)pa,(int8*)offset,PAGE_SIZE);
 		}
	}else{
		// 内核空间缺页
		printk("on kernel\n");
		while(1);
	}
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
	 
	vaddr = VEDIO_VADDR;	// vedio virtual address set to 0x1000000(16MB)	 
	size = kparam->yres * kparam->xres * (kparam->bpp/8);
	
	page_map( KERNEL_PAGE_DIR_ADDR, kparam->vaddr, vaddr, size, P_P|P_RW|P_US);
}


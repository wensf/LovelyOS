#include <printk.h>
#include <system.h>
#include <libc.h>
#include <types.h>
#include <sched.h>

extern void _int3_trap(int cpl, unsigned long esp );
extern void system_call(void);
extern void _generic_protected_fault( int cpl, unsigned long esp );
extern void _operation_fault(int cpl, unsigned long esp);
extern void _page_fault(int cpl, unsigned long esp);
extern void _parallel_interrupt(int cpl, unsigned long esp);

extern struct gate_desc idt[]; /* IDT define at kernel/kernel.s */

void kernel_panic ( int cpl, unsigned long esp )
{
	unsigned int *p;

	p = (unsigned int *)esp;
	printk("current task pid = %d\n", current->pid); 
	printk("esp=%08x\n", (unsigned int)esp); 
	printk("eax=%08x\n", *p++); 
	printk("ebx=%08x\n", *p++); 
	printk("ecx=%08x\n", *p++); 
	printk("edx=%08x\n", *p++); 
	printk("esi=%08x\n", *p++); 
	printk("edi=%08x\n", *p++); 
	printk("ebp=%08x\n", *p++);
	printk("fs =%08x\n", *p++ & 0xFFFF); 
	printk("gs =%08x\n", *p++ & 0xFFFF); 
	printk("es =%08x\n", *p++ & 0xFFFF); 
	printk("ds =%08x\n", *p++ & 0xFFFF); 
	printk("error code=%08x\n", *p++ & 0xFFFF); 
	printk("eip=%08x\n", *p++); 
	printk("cs =%08x\n", *p++ & 0xFFFF); 
	printk("eflags=%08x\n", *p++); 
	printk("cpl=%d\n",cpl); 
	if ( cpl == 3 )
	{
		printk("esp=%08x\n", *p++); 
		printk("ss=%08x\n", *p++); 
	}

	while(1){
		__asm__ __volatile__ ("hlt");
	}
	
}

void kernel_die(const char*fmt,...)
{
	char buf[256];
	va_list ap;
	
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	printk("OSLover_Die: ");
	printk(buf);

	while(1){
		__asm__ __volatile__ ("hlt");
	}	
}



void divide_error(int cpl, unsigned long esp )
{
	printk("divide_error\n");
 	kernel_panic(cpl, esp);
}

void int3_trap(int cpl, unsigned long esp )
{
	printk("int3_trap\n");
	kernel_panic(cpl, esp);
}

void operation_fault(int cpl, unsigned long esp )
{
	printk("operation_fault\n");
	kernel_panic(cpl, esp);
}


void double_fault(int cpl, unsigned long esp )
{
	printk("double_fault\n");
	kernel_panic(cpl, esp);
}

void generic_protected_fault( int cpl, unsigned long esp )
{
	printk("generic_protected_fault\n");
	kernel_panic(cpl, esp);
}

extern void do_page_fault(void);

void page_fault(int cpl, unsigned long esp )
{ 
	do_page_fault();
}

void parallel_interrupt(int cpl, unsigned long esp)
{
	printk("parallel_interrupt\n");
	kernel_panic(cpl, esp);
}


void trap_init( void )
{
	set_trap_descriptor(0x0, 0,divide_error);
	set_trap_descriptor(0x3, 3,_int3_trap);
	set_trap_descriptor(0x6, 0,_operation_fault);
	set_trap_descriptor(0x8, 0,double_fault);
	set_trap_descriptor(0xD, 0,_generic_protected_fault);
	set_trap_descriptor(0xE, 0,_page_fault);
	set_trap_descriptor(0x27, 0,_parallel_interrupt);
	set_system_descriptor(0x80,3,system_call);
}

#include <printf.h>
#include <system.h>
#include <libc.h>

extern void int3_trap(void);
extern void system_call(void);

extern struct gate_desc idt[]; /* IDT define at kernel/kernel.s */

void divide_error( long esp, long error_code)
{
    printf("divide_error\n");
    while(1);
}
void int3_trap_c(long esp, long eror_code)
{
	printf("int3 trap\n");
	while(1);
}

void double_fault( long esp, long error_code)
{
	printf("double fault\n");
	while(1);
}
void generic_protected_fault( long esp, long error_code)
{
	printf("generic_protected_fault\n");
	while(1);
}

void trap_init( void )
{
	set_igate_descriptor(0x0, 0,divide_error);
	set_trap_descriptor(0x3, 3,int3_trap);
	set_trap_descriptor(0x8, 0,double_fault);
	set_trap_descriptor(0xD, 0,generic_protected_fault);
	set_system_descriptor(0x80,3,system_call);
}

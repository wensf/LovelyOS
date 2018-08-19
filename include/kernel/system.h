/**
 * FILE : include/kernel/system.h
 * Author : Wen Shifang
 * Date : 2016.5.2
 */
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#define sti() __asm__ ("sti"::)
#define cli() __asm__ ("cli"::)
#define nop() __asm__ ("nop"::)

struct tss_desc
{
	unsigned short limit0;
	unsigned short base0;
	unsigned short base1:8, type:4, s:1, dpl:2, p:1;
	unsigned short limit1:4, avl:1, l:1, d:1, g:1, base2:8;
}__attribute__((packed));

struct gate_desc
{
	unsigned short offset1;
	unsigned short selector;
	unsigned char  res:5,attr:3;
	unsigned char  type:4,s:1,dpl:2,p:1;
	unsigned short offset2;
}__attribute__((packed));

extern struct tss_desc _gdt[];
extern struct gate_desc _idt[];

#define number_set(gate_type,n,name,val) \
	((volatile struct gate_type *)(&_idt[n]))->name = (val)

#define set_igate_descriptor(n, _dpl,entry) \
do{\
	number_set(gate_desc,n,offset1,((unsigned long)(&entry)& 0xFFFF));\
	number_set(gate_desc,n,selector,0x8);\
	number_set(gate_desc,n,res,0x0);\
	number_set(gate_desc,n,attr,0x0);\
	number_set(gate_desc,n,type,0xE);\
	number_set(gate_desc,n,s,0);\
	number_set(gate_desc,n,dpl,_dpl);\
	number_set(gate_desc,n,p,1);\
	number_set(gate_desc,n,offset2,((((unsigned long)(&entry)))>>16) & 0xFFFF);\
}while(0)

#define set_trap_descriptor(n, _dpl,entry) \
do{\
	number_set(gate_desc,n,offset1,((unsigned long)(&entry) & 0xFFFF));\
	number_set(gate_desc,n,selector,0x8);\
	number_set(gate_desc,n,res,0x0);\
	number_set(gate_desc,n,attr,0x0);\
	number_set(gate_desc,n,type,0xF);\
	number_set(gate_desc,n,s,0);\
	number_set(gate_desc,n,dpl,_dpl);\
	number_set(gate_desc,n,p,1);\
	number_set(gate_desc,n,offset2,((((unsigned long)(&entry)))>>16) & 0xFFF);\
}while(0)

#define set_system_descriptor(n,_dpl,entry)\
do{\
	number_set(gate_desc,n,offset1,((unsigned long)(&entry) & 0xFFFF));\
	number_set(gate_desc,n,selector,0x8);\
	number_set(gate_desc,n,res,0x0);\
	number_set(gate_desc,n,attr,0x0);\
	number_set(gate_desc,n,type,0xF);\
	number_set(gate_desc,n,s,0);\
	number_set(gate_desc,n,dpl,_dpl);\
	number_set(gate_desc,n,p,1);\
	number_set(gate_desc,n,offset2,((((unsigned long)(&entry)))>>16) & 0xFFF);\
}while(0)

#define number_set1(n,name,val) \
		((volatile struct tss_desc *)(&_gdt[(n)]))->name = (val)

#define set_tss_descriptor(n, tss)\
do{\
	number_set1(n,limit0,sizeof(struct tss_struct) & 0xFFFF);\
	number_set1(n,base0,(unsigned short)((long)(&tss))&0xFFFF);\
	number_set1(n,base1,((((long)(&tss))>>16)&0xFF));\
	number_set1(n,type,9);\
	number_set1(n,s,0);\
	number_set1(n,dpl,0);\
	number_set1(n,p,1);\
	number_set1(n,limit1,(sizeof(struct tss_struct)>>16)&0xF);\
	number_set1(n,avl,0);\
	number_set1(n,l,0);\
	number_set1(n,d,1);\
	number_set1(n,g,1);\
	number_set1(n,base2,(((unsigned long)(&tss))>>24)&0xFF);\
}while(0)

struct desc_ptr
{
	unsigned short size;
	unsigned long address;
}__attribute__((packed));

#define load_idt(idt) do\
{\
	asm volatile("lidt %0\n\t"\
		::"m"(idt)\
	);\
}while(0)

#endif

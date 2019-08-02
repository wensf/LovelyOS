#ifndef __SYSCALL_H__
#define __SYSCALL_H__


#define _SYSCALL_fork  			0
#define _SYSCALL_pause 			1
#define _SYSCALL_sleep 			2
#define _SYSCALL_open  			3
#define _SYSCALL_read  			4
#define _SYSCALL_write 			5
#define _SYSCALL_close 			6
#define _SYSCALL_dup   			7
#define _SYSCALL_lseek      	8
#define _SYSCALL_idle       	9
#define _SYSCALL_get_utime     10
#define _SYSCALL_get_ktime     11
#define _SYSCALL_execve        12
#define _SYSCALL_getpid        13
#define _SYSCALL_mmap          14
#define _SYSCALL_exit          15
#define _SYSCALL_wait          16
#define _SYSCALL_ioctl		   17

#define _syscall_0(type,name) \
type name(void)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_##name)\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

#define _syscall_1(type,name,atype,a) \
type name(atype a)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_##name),"b"((long)(a))\
			:"memory"\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

#define _syscall_2(type,name,atype,a,btype,b) \
type name(atype a, btype b)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_##name),"b"((long)(a)),\
			 "b"((long)(b))\
			:"memory"\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

#define _syscall_3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a, btype b, ctype c)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_##name),"b"((long)(a)),\
			 "c"((long)(b)), "d"((long)(c))\
			:"memory"\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}


#define O_RD                    1
#define O_WR                    2
#define O_RDWR              O_RD|O_WR
#define O_CREAT                 4

#define NOBLK                 	1

#define SEEK_SET                0
#define SEEK_CUR                1
#define SEEK_END                2

static inline _syscall_0(int,fork)
static inline _syscall_0(int,pause)
static inline _syscall_1(int,sleep,int,mseconds)
static inline _syscall_3(int,open,const char *,file_name, int, mode, int, flag)
static inline _syscall_1(int,dup,int,fd)
static inline _syscall_3(int,write,int,fd,char *,__buf, int, len)
static inline _syscall_3(int,lseek,int, fd,int,offset, int, whence)
static inline _syscall_0(void,idle)
static inline _syscall_0(unsigned int, get_utime)
static inline _syscall_0(unsigned int, get_ktime)
static inline _syscall_1(unsigned int, execve, const char *, file_name)
static inline _syscall_3(unsigned char*, mmap, int, fd,unsigned long, size, unsigned long, flags)
static inline _syscall_3(int, ioctl, int, fd,unsigned long, cmd, unsigned long, arg)
static inline _syscall_3(int,read,int,fd,char *,__buf, int, len)


#endif




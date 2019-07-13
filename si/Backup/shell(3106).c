
#define _SYSCALL_write    5

#define _syscall_3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a, btype b, ctype c)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_write),"b"((long)(a)),\
			 "c"((long)(b)), "d"((long)(c))\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

inline _syscall_3(int,write,int,fd, const char *,__buf, int, len)

int printf(const char *buf)
{
	int i = write( 0, buf, 18 );

	return (i);
}

int main(void)
{
	while(1)
	{
		printf("Hello Loverly OS\n");
	}
	
	return (0);
}

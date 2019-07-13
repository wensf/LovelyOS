
#define _SYSCALL_open    3
#define _SYSCALL_write    5

#define _syscall_3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a, btype b, ctype c)\
{\
	long _res;\
	__asm__ __volatile__("int $0x80"\
			:"=a"(_res)\
			:"a"(_SYSCALL_##name),"b"((long)(a)),\
			 "c"((long)(b)), "d"((long)(c))\
	);\
	if(_res >= 0)\
		return (type)(_res);\
	else{\
		return (type)(-1);\
	}\
}

#define O_RD                        1
#define O_WR                        2
#define O_RDWR                      O_RD|O_WR
#define O_CREAT                     4


static inline _syscall_3(int,write,int,fd, const char *,__buf, int, len)
static inline _syscall_3(int,open,const char *,file_name, int, mode, int, flag)



int printf(const char *buf)
{
	int i = write( 0, buf, 18 );

	return (i);
}

int main(void)
{
	open("/dev/tty1",O_RDWR,4);

	int i = 0;
	while(1)
	{
		if ( i < 3 ){
			i++;
			printf("Hello Loverly OS\n");
		}
	}
	
	return (0);
}

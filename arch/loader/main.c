/**
 * main.c
 *
 * Author : Wen Shifang
 * Create 2018.07.18
 * Last modify : 2018.07.18
 */


/**
 * 在X86实模式下，我们使用16位代码，并且启用GCC扩展的方式，
 * 汇编子程序调用本文件定义的函数时，也应该使用同样的指令
 * 指定代码格式，否则调用出错.
 */
__asm__(".code16gcc");


#include <printf.h>
#include <fat16.h>
#include <console.h>
#include <libc.h>

struct kernel_param
{
	int xres;
	int yres;
	int bpp;
	int vaddr;
	int mem_cnt;
};

unsigned char sector[512];

struct vga_mode_struct
{
	int mode;
	int supported;
};

struct vga_mode_struct svga_mode[256] =
{
	/* {0x100, 0},
   	{0x101, 0},
  	{0x102, 0},
  	{0x103, 0},
  	{0x104, 0}, */
  	{0x105, 0},
  	{0x106, 0},
  	{0x107, 0},
  	{0x108, 0},
  	{0x109, 0},
   	{0x10a, 0},
   	{0x10b, 0},
  	{0x10c, 0},
  	{0x113, 0},
  	{0x114, 0},
   	{0x115, 0},
  	{0x116, 0},
  	{0x117, 0},
   	{0x118, 0},
   	{0x119, 0},
  	{0x11a, 0},
   	{0x11b, 0},
};

#define XRES   1920
#define YRES   1080
#define BPP       32

int ax = 0, ay = 0, bit_pixel = 0, vesa_mode = 0, video_addr;

int get_key ( int timeout )
{
	int ret = 0;
	__asm__ __volatile__ ("mov $0,%%ah\n\t"
						  "int $0x16\n\t"
						  :"=a"(ret)
						  :
						  );
	return (ret & 0xFFFF);
}

void clear_screen(void)
{
	/** 
	 * Clear screen
	 */

	__asm__ __volatile__(
		"mov $0x06, %%ah\n\t"
		 "mov $0,%%al\n\t"
		 "mov $0,%%ch\n\t"
		 "mov $0,%%cl\n\t"
		 "mov $24,%%dh\n\t"
		 "mov $79,%%dl\n\t"
		 "mov $07,%%bh\n\t"
		 "int $0x10\n\t"
	::);
}

struct kernel_param *kp;

int init_vesa( int xres, int yres, int bpp )
{
    for ( int i = 0; i < 256; i++ )
    {
    	svga_mode[i].mode = 0x110+i;
    }

    /**
     * INT 10H AX=4F00H Get the VBE controller Infomation.
     */

    int _result;
    __asm__ __volatile__ (
    	"mov $0x0, %%di\n\t"
    	"mov $0x4f00, %%ax\n\t"
    	"int $0x10\n\t"
    	:"=a"(_result)
    	:
    );

    printf("result=%04x\n", _result & 0xFFFF);
    if ( (_result & 0xFFFF) == 0x004F )
    {
    	printf("call INT 10H succeed\n");

    	char *vbe_p = 0x0;
    	printf("vbe_signature %c%c%c%c\n", vbe_p[0], vbe_p[1], vbe_p[2], vbe_p[3]);
    	printf("vbe version %04x\n", *((unsigned short*)(vbe_p+4)));
    	printf("vbe total_memory %0x4f\n", *((unsigned short*)(vbe_p+14)));
    }

    /**
	 * 下面是一些测试代码，用来查询显示器是否支持SVGA的高分辨率模式号
	 * 调用系统BIOS INT 10H AX=4F01H
	 * 返回的信息我们保存在ES:DI开始的256个字节里.
	 * 注意在C语言里，默认的数据段选择器是DS, 为了方便我们访问内存,
	 * 我们将ES设置和DS一致，为0x9000. DI设置为0X0
	 * INT 10H AX=4F01H功能调用将把信息存放在0x9000:0x8000.
	 * 这样我们在C代码中使用指针指向0地址,即0x9000:0x8000.
     */

	printf("vesa attr xres yres color vram\n");

    for ( int i = 0; i < /*sizeof(svga_mode)/sizeof(svga_mode[0])*/256; i++ )
    {
    	int _result = 0;
    	unsigned char *p = (unsigned char *)0;

    	memset( p, 0x0, 256);

    	__asm__ __volatile__ (
    	"mov $0x0, %%di\n\t"
    	"mov %1,%%cx\n\t"
    	"mov $0x4f01, %%ax\n\t"
    	"int $0x10\n\t"
    	:"=a"(_result)
    	:"m"(svga_mode[i].mode));

    	if ( (_result>>8) != 0 ) /** AH != 0 : Failed */
    	{
    		continue;
    	}else /** AH == 0 : Sucessed */
    	{
    	}

		/**
		 * 打印该模式下的分辨率和显存地址.
		 */

    	printf("%04x %02x % 4d % 4d %d %08x\n",
    					svga_mode[i].mode,
    					 *((unsigned char *)(p+0)), 	/* attr */
    					 *((unsigned short *)(p+18)), 	/* xresolution */
    					 *((unsigned short *)(p+20)),	/* yresolution */
    					 *((unsigned char *)(p+0x19)),	/* bits_per_pixel */
    					 *((unsigned int *)(p+40))     /**/
    					 );

    	if ( (*((unsigned short *)(p+18)) == xres)
    		&& (*((unsigned short *)(p+20)) == yres)
    		&&  (*((unsigned char *)(p+0x19)) == bpp) )
        {
            ax = *((unsigned short *)(p+18));
            ay = *((unsigned short *)(p+20));
            bit_pixel = *((unsigned char *)(p+0x19));
            vesa_mode = svga_mode[i].mode;
			video_addr = *((unsigned int *)(p+40));
        }

		if ( y > 24 )
		{
			x = 0; y = 0;
		//	get_key(0);

			clear_screen();
		}
    }

    if ( (ax != xres) || (ay != yres) || (bit_pixel != bpp ) )
    {
        printf("Not supported %d * %d %d\n",xres,yres,bpp);
        get_key(0);
        return (-1);
    }else{
        printf("supported x=%d,y=%d, bit_per_pixel=%d video_addr=%08x\n",ax,ay,bit_pixel,video_addr);
    }
	kp->xres  = xres;
	kp->yres  = yres;
	kp->bpp   = bpp;
	kp->vaddr = video_addr;

	// get_key(0);
	clear_screen();

	// set vesa mode.
	int mode = vesa_mode;
	__asm__ __volatile__(
	                 "mov $0x4f02, %%ax\n\t"
                     "mov %1,%%bx\n\t"
                     "addw $0x4000,%%bx\n\t"
                     "int $0x10\n\t"
                     :"=a"(_result)
                     :"m"(mode)
                     :"bx");

	return 0;

}

int load_kernel_file( const char *kernel_name )
{
	int r = driver_load_sector(0,1,sector);

	if ( r == -1 )
	{
		printf(" [ failed ]\n");
	}else{
		printf(" [ done   ]\n");

		for ( int i = 0; i < 4; i++ )

		{
			for ( int j = 0; j < 16; j++ )
			{
				printf("%02x ", sector[i*16+j]);
			}
			printf("\n");
		}

		printf("......\n");

		for ( int i = 12; i < 16; i++ )
		{
			for ( int j = 0; j < 16; j++ )
			{
				printf("%02x ", sector[i*16+j]);
			}
			printf("\n");
		}


        printf("DPT:\n");

        const unsigned char *p_dpt = sector+0x1BE;
        int partition_sector_start;
        int partition_sector_all;

        if ( p_dpt[0] == 0x80 )
        {
            printf("Active\n");
        }else{
            printf("InActive\n");
        }

        printf("Start header :%d\n", p_dpt[1]);
        printf("Start sector :%d\n", p_dpt[2]&0x3F);
        printf("Start Cylinde:%d\n", (p_dpt[2]>>6)<<8 | p_dpt[3]);

        printf("type: %02x ", p_dpt[4]);

        switch(p_dpt[4])
        {
        case 0x00:
            printf("No specied\n");
            break;
        case 0x01:
            printf("FAT12\n");
            break;
        case 0x04:
            printf("FAT16 <32MB\n");
            break;
        case 0x06:
            printf("FAT16 >=32MB\n");
            break;
        case 0x0B:
            printf("FAT32\n");
            break;
        case 0x05:
            printf("Extends\n");
            break;
        case 0x07:
            printf("NTFS\n");
            break;
        case 0x0F:
            printf("LBA\n");
            break;
        default:
            printf("Unknow\n");
            break;
        }

        printf("End header :%d\n", p_dpt[5]);
        printf("End sector :%d\n", p_dpt[6]&0x3F);
        printf("End Cylinde:%d\n", (p_dpt[6]>>6)<<8 | p_dpt[7]);

        partition_sector_start = p_dpt[11]<<24 | p_dpt[10]<<16 | p_dpt[9] << 8 | p_dpt[8];
        printf("Logic sector start:%d\n", partition_sector_start);

        partition_sector_all = p_dpt[15]<<24 | p_dpt[14]<<16 | p_dpt[13] << 8 | p_dpt[12];
        printf("All sector:%d\n", partition_sector_all);
	}

	return 0;
}

#define FLAG_CF 0x00000001

struct mem_info_struct
{
    unsigned int address_low;
    unsigned int address_high;
    unsigned int length_low;
    unsigned int length_high;
    unsigned int type;
};

int memory_range_probe( int idx, unsigned char *buf )
{
    int _ret, _cflags;
    int len;

    __asm__ __volatile__ (
        "movl $0xE820, %%eax\n\t"
        "movl %3, %%ebx\n\t"
        "movl %4, %%edi\n\t"
        "movl $0x14, %%ecx\n\t"
        "movl $0x534d4150, %%edx\n\t"
		"int $0x15\n\t"
        :"=b"(_ret),"=a"(_cflags),"=c"(len)
        :"g"(idx), "m"(buf)
        :"memory"
    );

    return (_ret);
}

struct mem_info_struct mem_info, *mem_p;

int main(void)
{
    int r;

    console_init();
    printf("console_init() completed.\n");
    mem_p = (struct mem_info_struct *)( 0x8000 + sizeof(struct kernel_param) );
	kp = (struct kernel_param *)(0x8000);
	kp->mem_cnt = 0;
	do{

		r = memory_range_probe(r,(unsigned char *)&mem_info);
		memcpy(mem_p, &mem_info, sizeof(struct mem_info_struct));
		mem_p++; kp->mem_cnt++;
	}while( r != 0 );
	printf("%d block memory found in tatal\n", kp->mem_cnt);
	r = init_vesa( XRES, YRES, BPP );
	if ( r == -1 )
	{
        get_key(0);
        clear_screen();
	}

	#if 0
	unsigned long *pvaddr = ( unsigned long *)video_addr;
	for ( int x = 0; x < 100; x++ )
	{
		for ( int y = 0; y < 100; y++ )
		{
			*pvaddr++ = 0xFFFF;
		}
	}
	
	get_key(0);
	#endif
	
	return 0;

	printf("[ disk read test ] load a sector data from disk...");
	load_kernel_file("KERNEL.BIN");

	return 0;
}

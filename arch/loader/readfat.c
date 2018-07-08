/**
 * 在X86实模式下，我们使用16位代码，并且启用GCC扩展的方式，
 * 汇编子程序调用本文件定义的函数时，也应该使用同样的指令
 * 指定代码格式，否则调用出错.
 */
__asm__(".code16gcc");

#include <libc.h>

/**
 * 使用内联汇编的方式调用BIOS中断服务设置光标位置，并且打印一个字符.
 * (x,y)光标位置, (c)欲显示的字符。
 */

void my_putchar( int x, int y, char c)
{
    __asm__ __volatile__("mov $0x02, %%ah\n\t"
             "mov $0x00, %%bh\n\t"
             "movb %0, %%dl\n\t"
             "movb %1, %%dh\n\t"
             "int $0x10\n\t"
    :
    :"g"(x), "g"(y)
    :"ax","dx"
    );

    __asm__ __volatile__("mov $0x0A, %%ah\n\t"
             "mov %0, %%al\n\t"
             "mov $0x00, %%bh\n\t"
             "mov $0x01, %%cx\n\t"
             "int $0x10\n\t"
    :
    :"g"(c)
    :"ax","cx"
    );
}

/**
 * 在指定的起始位置显示字符串。
 * (x,y) 起始行号列号，默认(80x25)
 * (str) 字符串指针
 */
int my_putstr(int x, int y, const char *str)
{
    while( str && (*str != 0) )
    {
        my_putchar( x, y, *str++);
        if ( ++x > 79 )
        {
            x = 0; y++;
        }

    /* 若y>24，可以设置页号来达到翻页的效果 */
    }

    return 0;
}

void my_cfun(void)
{
    char buffer[32];

    my_putstr( 0, 0, "we just show a demo string!" );

    //sprintf(buffer, "C function entry at %08x", (unsigned long)my_cfun);
    //sprintf(buffer, "%08x", (unsigned long)my_cfun);
    //int len = 0;

    strcpy(buffer, "test string");
    //len =
    strlen(buffer);


    my_putstr( 0, 1, buffer);
}

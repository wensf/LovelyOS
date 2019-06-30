/**
 * 在X86实模式下，我们使用16位代码，并且启用GCC扩展的方式，
 * 汇编子程序调用本文件定义的函数时，也应该使用同样的指令
 * 指定代码格式，否则调用出错.
 */
__asm__(".code16gcc");


int x = 0, y = 0;

/**
 * 使用内联汇编的方式调用BIOS中断服务设置光标位置，并且打印一个字符.
 * (x,y)光标位置, (c)欲显示的字符。
 */

void putc( int x, int y, char c)
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

int get_position( int *x, int *y )
{
    int cx = 0, cy = 0;

    __asm__ __volatile__(" mov $0x3, %%ah\n\t"
						 " mov $0x0, %%bh\n\t"
                         " int $0x10\n\t"
                         " mov %%dh, %1\n\t"
                         " mov %%dl, %0\n\t"
                         :"=m"(cx),"=m"(cy)
                         );
    *x = cx; *y = cy;

    return 0;
}

void console_init(void)
{
    get_position( &x, &y );
    x  = 0; y++;
}

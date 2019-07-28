/**
 * PureRTOS/lib/libc.c
 * some functions to deal with strings.
 * created : 2014.12.20
 * last modified : 2015.9.15
 */
#include <stdarg.h>

#define MAX_BUF_LEN   256
#define ZEROPAD       0
#define SPACEPAD      1

int strlen(const char *s)
{
	const char *sc ;

	for(sc = s; *sc != '\0'; ++sc){
		;
	}

	return sc - s;
}

void strcpy(char *dst,const char *src)
{
	while(*src != '\0'){
		*dst++ = *src++;
	}

	*dst = '\0';
}

int strncpy(char *dst,const char*src, int size)
{
	int counter = 0;

	if(!src || !dst){
		return -1;
	}

	while(counter < size){
		*dst++ = *src++;
		counter++;
	}

	*dst = '\0';

	return counter;
}

int strcmp(const char *src1,const char *src2)
{
	while( (*src1) && (*src2)){
		if ( *src1 == *src2 ){
			src1++;
			src2++;
		}
	}

	if((*src1 == '\0') && (*src2 == '\0'))
		return 0;	/* equal. */
	else
		return -1;	/* not equal. */
}

int strncmp(const char *str1, const char *str2, int len)
{
	while ( (len-- > 0) && *str1 && *str2 ){
		if(*str1 != *str2){
			return *str1-*str2;
		}

		str1++;
		str2++;
	}

	if ((*str1 == '\0') && (*str2 == '\0'))
		return 0;	/* equal. */
	else
		return -1;	/* not equal. */
}


void memcpy(void *dst,const void *src, int size)
{
	char *pdst, *psrc;

	pdst = (char *)dst;
	psrc = (char *)src;

	while(size-- > 0){
		*pdst++ = *psrc++;
	}
}

int memcmp(const char *src1,const char *src2, int size)
{
	int i;

	for(i = 0; i < size; i++){
		if(src1[i] != src2[i]){
			return -1;
		}
	}

	return 0;
}

void memset(void *dst, char val, int size)
{
	int i;
	char *dst1 = (char *)dst;

	for(i = 0; i < size; i++){
		*dst1 = val;
		++dst1;
	}

}

/**
 * Remove the whitespace at the end and first.
 */
void trim(char *buf)
{
	char *ptr1 = buf;
	char *ptr2 = buf;
	int  num   = 0,count = 0;

	while(*ptr1 != '\0'){
		if(*ptr1 == ' '){
			count++;
			ptr1++;
		}else{
			break;
		}
	}

	if(count > 0){
		ptr1 = buf + count;
		while(*ptr1 != '\0'){
		 	*ptr2++ = *ptr1++;
		}
	}

	buf[strlen(buf) - count] = '\0';

	num = strlen(buf);
	ptr1 = buf + num - 1 ;
	count = 0;

	while(num-- > 0){
		if(*ptr1 == ' '){
			count++;
			ptr1--;
		}else{
			break;
		}
	}


	buf[strlen(buf) - count] = '\0';
}

int atoi(const char * buf)
{
	int integer = 0;
	int pri = 1;
	const char *ptr = buf;

	if(strlen(buf) <= 0){
		return -1;
	}

	while(*ptr != '\0'){
		if(*ptr < '0' || *ptr > '9'){
			return -1;
		}

		ptr++;
	}

	while(--ptr >= buf){
		integer += ((*ptr - 48) * pri);
		pri *= 10;
	}

	return integer;
}

int ctoi(char c)
{
	if(c >= '0' && c <= '9'){
		return c - '0';
	}

	switch( c ){
	case 'a':	return 10;
	case 'b':	return 11;
	case 'c':	return 12;
	case 'd':	return 13;
	case 'e':	return 14;
	case 'f':	return 15;
	default:	break;
	}

	return -1;
}

int htoi(const char *buf)
{
	int integer = 0;
	const char *ptr = buf;
	int cc = 0;
	int tmp = 0;

	while(*ptr != '\0'){
		if((*ptr < 48 || *ptr > 57) &&
			(*ptr < 'a' || *ptr > 'z') &&
			(*ptr < 'A' || *ptr > 'Z')){
			return -1;
		}

		ptr++;
	}

	while(--ptr >= buf){
		tmp = ((tmp = ctoi(*ptr)) == 0) ? 0 : (tmp << ( cc << 2));
		integer += tmp;
		cc++;
	}

	return integer;
}

void bzero(char *dst, int len)
{
	while(len-- > 0){
		*dst++ = 0;
	}
}


int itoa(int num,char *str)
{
	int i,j = 0;
	char buffer[64];

	if(num < 0){
		*str++ = '-';
		num *= -1;
	}

	do{
		i = num % 10;
		num = num / 10;

		buffer[j++] = i + '0';
	}while(num > 0);

	while(j > 0){
		*str++ = buffer[--j];
	}

	*str = '\0';

	return 0;
}

/**
 * decinal to hexadecinal
 * num : destination number to changed.
 * str : buffer.
 */
static const char *table = "0123456789abcdef";

int itoh(int num,char *str)
{
	char buffer[64];
	int n = 8;

	if(!str){
		return -1;
	}

	while(--n >= 0){
		buffer[n] = table[num & 0xf];
		num >>= 4;
	}

	/* skip the '0' on start of this string. */
	n = 0;
	while(n < 8){
		if(buffer[n] != '0')
			break;
		n++;
	}

	/* copy to buffer. */
	while(n < 8){
		*str++ = buffer[n];
		n++;
	}

	*str = '\0';

	return 0;
}

/**
 * changed a byte to binary form.
 */

int itob(char ch,char *str)
{
	int i ;

	for(i = 7; i >= 0; i--){
		*str++ = ((ch >> i) & 0x1) + '0';
	}

	*str = '\0';

	return 0;
}

#define is_digit(x) (((x >= '0') && (x <= '9')))

/**
 * get integer from string and skip the number on buffer.
 */
static int skip_atoi(const char **s)
{
	int i = 0;

	while(is_digit(**s)){
		i = i * 10 + *((*s)++) - '0';
	}

	return i;
}

/**
 * modify field_width,default left aligned.
 * str: two level pointer point to a pointer that point to a buffer.
 * count:number of characters will be insert.
 * field_width : field width.
 * flags : fill by zero if 0 or blank if 1.
 */
int padden(char **str,int count,int field_width,int flags)
{
	int pad,i,c;
	char *ptr;
	char tmp[MAX_BUF_LEN];

	/* need to modify. */
	if(field_width <= 0){
		return 0;
	}

	/* calculate character number to pad.*/
	pad = field_width - count;
	if(pad <= 0){
		return 0;
	}

	/* save orignal area that will be fill. */
	ptr = *str - count;
	for(c = 0 ; c < count ; c++){
		tmp[c] = *ptr++;
	}

	/* padding by 0 or blank. */
	i = 0;
	ptr = *str - count;
	while(i < pad){
		*ptr = (flags == ZEROPAD) ? '0' : ' ';
		ptr++;
		i++;
	}

	*str = ptr;
	/**
	 * Save temporarily stored data to padding area,modify the poiner
	 * at the same time.
	 */
	i = 0;
	while(i < count){
		*((*str)++)  = tmp[i];
		i++;
	}

	return 0;
}

/**
 * To format a string.
 * buf : buffer to store the result.
 * buf_len : fmt length.
 * fmt format string.
 * ap : variable string.
 */
int vsnprintf(char *buf, int buf_len, const char *fmt, va_list ap)
{
	char tmp[MAX_BUF_LEN];
	char *str,/**end,*/c;
	int  i,flags,field_width,count;

	//end = buf + buf_len - 1;

	for(; *fmt != '\0'; fmt++){

		count = 0;

		if(*fmt != '%'){
			*buf = *fmt;
			++buf;
			continue;
		}

		/* skip '%' .*/
		++fmt;

		flags = -1;
		/* get padding way */
		switch(*fmt){
			case '0':
				/* padding by 0. */
				flags = ZEROPAD;
				break;
			case ' ':
				/* padding by blank. */
				flags = SPACEPAD;
				break;
		}

		if(flags != -1){
			fmt++;
		}

		/* get field_width and modify fmt. */
		field_width = -1;
		if(is_digit(*fmt)){
			field_width = skip_atoi(&fmt);
		}

		str = tmp;		

		/* get a specific varible from fmt. */
		switch(*fmt){
			case 'c':
				/* get a char. */
				c = va_arg(ap, int);
				*buf++ = c;
				count++;
				break;
			case 's':
				/* get a string from stack. */
				str = va_arg(ap,char*);
				while(*str != '\0'){
					*buf++ = *str++;
					count++;
				}
				break;
			case 'd':
				/* get a integer from stack,then to string format. */
				i = va_arg(ap,int);
				itoa(i,str);
				while(*str != '\0'){
					*buf++ = *str++;
					count++;
				}
				break;
			case 'x':
				i = va_arg(ap, int);
				itoh(i,str);
				while(*str != '\0'){
					*buf++ = *str++;
					count++;
				}
				break;
			case 'b':
				/* get a byte from stack,then to binary format. */
				i = va_arg(ap, int);
				itob(i,str);
				while(*str != '\0'){
					*buf++ = *str++;
					count++;
				}
				break;
			case 'u':
				break;
		}

		padden(&buf, count, field_width, flags);
	}

	*buf = '\0';

	return 0;
}

int sprintf(char *buf, const char *fmt,...)
{
	char fmt_buf[MAX_BUF_LEN];
	va_list ap;
	int fmt_len;

	va_start(ap, fmt);
	fmt_len = vsnprintf(fmt_buf, sizeof(fmt_buf), fmt, ap);
	strcpy(buf, fmt_buf);

	return fmt_len;
}

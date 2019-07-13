/**
 * This mkmbr is using to make a MBR on the floppy image file.
 * Wen Shifang
 * 2016.4.11
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
 
#define MBR_SECTOR_NR      1
#define LOADER_SECTOR_NR  32
#define KERNEL_SECTOR_NR 128
#define LOGO_SECTOR_NR   120
#define SHELL_SECTOR_NR    4

#define SECTOR_NR (MBR_SECTOR_NR+LOADER_SECTOR_NR+KERNEL_SECTOR_NR+LOGO_SECTOR_NR+SHELL_SECTOR_NR)


int get_file_size(const char *path)
{
	struct stat stbuffer;

	if ( stat(path, &stbuffer ) < 0 ){
        perror("stat");
		return (-1);
	}

	return stbuffer.st_size;
}

int main( int argc, char **argv )
{
	unsigned char *fptr;
	FILE *fp;
	int cnt;
	int offset;

 	if ( argc != 7 ){

        printf("Usage : ./build xxx.img mbr.bin loader.bin kernel.bin logo.bin sh.bin\n");
        exit(1);
    }

	fptr = (unsigned char *)malloc(sizeof(unsigned char)*(SECTOR_NR)*512);
	if ( !fptr ){
		perror("malloc");
		exit(1);
	}
	printf("%d bytes buffer malloed\n",SECTOR_NR*512);


	// 读取boot.bin(MBR),写入扇区: 0x00000000, 扇区数: MBR_SECTOR_NR

    fp = fopen(argv[2], "rb");
    if ( !fp ){
        perror("fopen");
        goto error;
    }

	offset = 0;
    cnt = fread( fptr+offset, 1, MBR_SECTOR_NR*512, fp );
    if ( cnt == 0 ){
        printf("fread %s error, size not currected!\n", argv[2]);
        goto error;
    }
    fclose( fp );
	printf("read %s %d byte(s)\n", argv[2], cnt);
	printf("write offset at %08x\n",offset);	

	// 读取load.bin，     写入扇区: 0x00000200， 扇区数: LOADER_SECTOR_NR
    fp = fopen( argv[3], "rb" );
    if ( !fp ){
       	perror("fopen");
        goto error;
    }

	offset = MBR_SECTOR_NR*512;
    cnt = fread( fptr+offset, 1, 512*LOADER_SECTOR_NR, fp );
	if ( cnt < 0 ){
        printf("fread %s error, size not currected!\n", argv[3]);
		goto error;
    }
	fclose(fp);
	printf("read %s %d byte(s)\n", argv[3], cnt);
	printf("write offset at %08x\n",offset);

	// 读取kernel.bin，    写入扇区: 0x00004200, 扇区数: KERNEL_SECTOR_NR 
	fp = fopen( argv[4], "rb");
	if ( !fp ){
		perror("fopen");
		goto error;
	}

	offset = (MBR_SECTOR_NR+LOADER_SECTOR_NR)*512;
	cnt = fread( fptr+offset, 1, KERNEL_SECTOR_NR*512, fp );
	if ( cnt < 0 ){
		perror("fread");
		goto error;
	}
	fclose( fp );
	printf("read %s %d byte(s)\n", argv[4], cnt);
	printf("write offset at %08x\n",offset);

	// 读取logo.bin，     写入扇区: 0x00014200, 扇区数: LOGOL_SECTOR_NR 

	fp = fopen( argv[5], "rb");
	if ( !fp ){
		perror("fopen");
		goto error;
	}
	offset = (MBR_SECTOR_NR+LOADER_SECTOR_NR+KERNEL_SECTOR_NR)*512;
    cnt = fread( fptr+offset, 1, LOGO_SECTOR_NR*512, fp );
    if ( cnt <= 0 ){
		perror("fread");
		goto error;
	}
	fclose( fp );
	printf("read %s %d byte(s)\n", argv[5], cnt);	
	printf("write offset at %08x\n",offset);	

	// 读取sh.bin，     写入扇区: 0x0001B200, 扇区数: SHELL_SECTOR_NR 
	fp = fopen( argv[6], "rb");
	if ( !fp ){
		perror("fopen");
		goto error;
	}
	offset = (MBR_SECTOR_NR+LOADER_SECTOR_NR+KERNEL_SECTOR_NR+LOGO_SECTOR_NR)*512;
    cnt = fread( fptr+offset, 1, SHELL_SECTOR_NR*512, fp );
    if ( cnt <= 0 ){
		perror("fread");
		goto error;
	}
	fclose( fp );
	printf("read %s %d byte(s)\n", argv[6], cnt);	
	printf("write offset at %08x\n",offset);

	// 缓冲区写入IMG文件
	FILE *p_img;
	p_img = fopen(argv[1], "wb");
	if ( !p_img ){
		perror("fopen");
		goto error;
	}

	cnt = fwrite(fptr,1,SECTOR_NR*512,p_img);
	printf("IMG %d Byte(s) Copy to %s In Total\n", cnt,argv[1]);
	
error:
    fclose( fp );
	free( fptr );

    return 0;
}

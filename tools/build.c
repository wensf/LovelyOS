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

#define KERNEL_SECTOR_NR 128 
#define MBR_SECTOR_NR     1
#define LOADER_SECTOR_NR  32

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

 	if ( argc != 5 ){

        printf("Usage : ./build xxx.bin xxx.img xxx_device\n");
        exit(1);
    }

	int ksize = get_file_size(argv[3]);
	printf("kernel image size %d\n", ksize);

	fptr = (unsigned char *)malloc(sizeof(unsigned char)*((MBR_SECTOR_NR+LOADER_SECTOR_NR+KERNEL_SECTOR_NR))*512);
	if ( !fptr ){
		perror("malloc");
		exit(1);
	}
	printf("%d bytes buffer malloed\n", (MBR_SECTOR_NR+LOADER_SECTOR_NR+KERNEL_SECTOR_NR)*512);

    fp = fopen(argv[1], "rb");
    if ( !fp ){
        perror("fopen");
        exit(1);
    }

    cnt = fread( fptr, 1, MBR_SECTOR_NR*512, fp );
    if ( cnt == 0 ){
        printf("fread %s error, size not currected!\n", argv[1]);
        exit(1);
    }

	printf("read %s %d byte(s)\n", argv[1], cnt);

    fclose( fp );

    fp = fopen( argv[2], "rb" );
    if ( !fp ){
         perror("fopen");
         exit(1);
    }

    cnt = fread( fptr + MBR_SECTOR_NR*512, 1, 512*LOADER_SECTOR_NR, fp );
	if ( cnt < 0 ){
        printf("fread %s error, size not currected!\n", argv[2]);
        fclose( fp );
        exit(1);
    }

	printf("read %s %d byte(s)\n", argv[2], cnt);
	fclose( fp );

	fp = fopen( argv[3], "rb");
	if ( !fp ){
		perror("fopen");
		exit(1);
	}

	cnt = fread( fptr + (MBR_SECTOR_NR+LOADER_SECTOR_NR)*512, 1, ksize, fp );
	if ( cnt < 0 ){
		perror("fread");
		exit(1);
	}
	fclose( fp );
	printf("read %s %d byte(s)\n", argv[3], cnt);

	fp = fopen( argv[4], "wb");
	if ( !fp ){
		perror("fopen");
		exit(1);
	}

    cnt = fwrite( fptr, 1, (MBR_SECTOR_NR+LOADER_SECTOR_NR)*512 + ksize, fp );
    if ( cnt != ksize+(MBR_SECTOR_NR+LOADER_SECTOR_NR)*512 ){
        perror("fwrite");
        fclose( fp );
		free( fptr );
        exit(1);
    }

    printf("ok %d bytes mbr writed to the first sector.\n", 512 *(MBR_SECTOR_NR+LOADER_SECTOR_NR)+ksize);

    int fill_byte = (MBR_SECTOR_NR+LOADER_SECTOR_NR+KERNEL_SECTOR_NR) * 512 - cnt;

    printf("fill %d bytes to the file\n", fill_byte);

    char fill = 0;
    while(fill_byte-->0)
    {
        fwrite(&fill, 1,1,fp);
    }

    fclose( fp );
	free( fptr );

    return 0;
}

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>




unsigned long utl_str2int(void *str)
{
    unsigned long rst=0;
    unsigned long tmp = 0;
    int  i, len;
    int sign,val;
    char *ptr = str;

    len = strlen((char *)ptr);
    if((ptr[1]=='x')||(ptr[1]=='X')){//HEX
        ptr +=2;
        for (i=2;i<len;i++) {
            tmp = *ptr++;
            if ((tmp<='9') && (tmp>='0')) {
                tmp = tmp -'0';
                rst = (rst<<4) + tmp;
            }
            else if ((tmp<='F') && (tmp>='A')) {
                tmp = tmp -'A' + 10;
                rst = (rst<<4) + tmp;
            }
            else if ((tmp<='f') && (tmp>='a')) {
                tmp = tmp -'a' + 10;
                rst = (rst<<4) + tmp;
            }
            else break;
            //rst = (rst<<4) + tmp;
        }
        printf("rst = %lx\n", rst);
        return rst;
    }
    else{
        for (i = 0;; i++) {
            if (ptr[i] != ' ') {
                break;
            }
        }
        sign = (ptr[i] == '-' ?  - 1: 1);
        if (ptr[i] == '+' || ptr[i] == '-') {
            i++;
        }
        for (val = 0;; i++) {
            if (ptr[i] >= '0' && ptr[i] <= '9') {
                val = 10 * val + (ptr[i] - '0');
            } else {
                break;
            }
        }
        return  sign*val;
    }
}



int main(int argc, char *argv[])
{
    if(argc != 3){
        printf("args invalid\n");
        return 0;
    }

    unsigned long addr = utl_str2int(argv[1]);
    int size = utl_str2int(argv[2]);

    printf("read from 0x%lX, size %d\n", addr, size);


    int fd = open("/dev/phy_mem", O_RDONLY);
    if(fd < 0){
        printf("phy_mem open fail\n");
        return 0;
    }

    addr = addr & 0xFFFFFFFFFFFFFFF8L;
    if(size > 256){
        size = 256;
    }
    if(size == 0){
        size = 8;
    }
    size = (size + 7) & 0xFFFFFFF8;

    __off64_t off = lseek64(fd, (__off64_t)addr, SEEK_SET);
    if(off < 0){
        printf("lseek fail\n");
        return 0;
    }

    unsigned int buf[256 / 4];
    int read_size = read(fd, (char*)buf, size);
    if(read_size < 0){
        printf("read failed\n");
        return 0;
    }

    int i;
    for(i = 0; i< size / 8; i++){
        printf("%#lX:\t\t %08x  %08x \n", addr + i * 8, buf[2 * i], buf[2 * i + 1]);
    }

    close(fd);


    return 0;
}




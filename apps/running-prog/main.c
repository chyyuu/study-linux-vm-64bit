#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



static void print_sys_reg_info()
{
    char *buf;
    int rd_size;

    int fd = open("/proc/sys_reg", O_RDONLY);
    if(fd < 0){
        printf("open failed\n");
        return;
    }

    buf = (char*)malloc(4096);
    if(buf == NULL){
        printf("malloc failed\n");
        return;
    }

    rd_size = read(fd, buf, 4096 - 1);
    if(rd_size < 0){
        printf("read failed\n");
        return;
    }

    buf[rd_size] = 0;

    printf("%s", buf);

    close(fd);
}


unsigned long a;


int main()
{
    a = 0xA5A5AA550000FFFF;

    printf("a = %lX, addr: 0x%p \n", a, &a);


    print_sys_reg_info();


    while(1){
        sleep(100);
    }

    return 0;
}




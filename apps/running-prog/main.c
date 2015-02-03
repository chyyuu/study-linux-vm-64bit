#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




typedef unsigned short  u16;
typedef signed short    s16;
typedef int             s32;
typedef unsigned int    u32;
typedef unsigned long long u64;




u16 cs, ds, ss, es, fs, gs;



static void get_segment_registers_info()
{
    asm volatile("mov %%cs, %0" : "=r" (cs));
    asm volatile("mov %%ds, %0" : "=r" (ds));
    asm volatile("mov %%ss, %0" : "=r" (ss));
    asm volatile("mov %%es, %0" : "=r" (es));
    asm volatile("mov %%fs, %0" : "=r" (fs));
    asm volatile("mov %%gs, %0" : "=r" (gs));

    printf("\n----  Segment Registers ----\n");
    printf("cs :%04x, TI = %s, Index = %d, RPL = %d\n", cs, (cs & 0x04) ? "LDT" : "GDT", cs >> 3, cs & 0x03);
    printf("ds :%04x, TI = %s, Index = %d, RPL = %d\n", ds, (ds & 0x04) ? "LDT" : "GDT", ds >> 3, ds & 0x03);
    printf("ss :%04x, TI = %s, Index = %d, RPL = %d\n", ss, (ss & 0x04) ? "LDT" : "GDT", ss >> 3, ss & 0x03);
    printf("es :%04x, TI = %s, Index = %d, RPL = %d\n", es, (es & 0x04) ? "LDT" : "GDT", es >> 3, es & 0x03);
    printf("fs :%04x, TI = %s, Index = %d, RPL = %d\n", fs, (fs & 0x04) ? "LDT" : "GDT", fs >> 3, fs & 0x03);
    printf("gs :%04x, TI = %s, Index = %d, RPL = %d\n", gs, (gs & 0x04) ? "LDT" : "GDT", gs >> 3, gs & 0x03);
}


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


int a;


int main()
{
    a = 0x013579BB;
    printf("a = %08X, addr: 0x%08X \n", a, &a);

    get_segment_registers_info();

    print_sys_reg_info();


    while(1){
        sleep(100);
    }

    return 0;
}




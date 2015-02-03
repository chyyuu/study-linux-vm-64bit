#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/desc.h>
#include <asm/pgtable.h>
#include <asm/desc.h>

#include <linux/seq_file.h>
#include <asm/processor.h>

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef signed short    s16;
typedef int             s32;
typedef unsigned int    u32;
typedef unsigned long long u64;


typedef struct{
   short limit;
   unsigned int address;
}__attribute__((packed)) gdtr_t;

gdtr_t gdtr;

static int sys_reg_show(struct seq_file *m, void *v)
{
    int entry_num;
    u32 cr0, cr3, cr4;

    seq_printf(m, "\n----  GDTR ----\n");
    asm(" sgdt gdtr");

    entry_num = (gdtr.limit + 1) / 8;
    seq_printf(m, "addr: 0x%08X, limit:%d, entry:%d\n", gdtr.address, gdtr.limit, entry_num);


    seq_printf(m, "\n----  Control Registers ----\n");

    cr0 = read_cr0();
    seq_printf(m, "CR0 = 0x%08X\n", cr0);
    seq_printf(m, "CR0.PG = %d\n", cr0 >> 31);

    cr4 = read_cr4();
    seq_printf(m, "CR4 = 0x%08X\n", cr4);
    seq_printf(m, "CR4.PAE = %d\n", (cr4 >> 5) & 0x1);

    cr3 = read_cr3();
    seq_printf(m, "cr3 = 0x%08X\n", cr3);
    seq_printf(m, "CR3 PT Base = 0x%08X\n", cr3 & 0xFFFFF000);;

    return 0;
}

static int sys_reg_open(struct inode *inode, struct file *file)
{
    return single_open(file, sys_reg_show, NULL);
}



static const struct file_operations sys_reg_fops =
{
    .owner      = THIS_MODULE,
    .open       = sys_reg_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};


static int __init sys_reg_init(void) {
    proc_create("sys_reg", 0, NULL, &sys_reg_fops);
    return 0;
}

static void __exit sys_reg_exit(void) {
    remove_proc_entry("sys_reg", NULL);
}


MODULE_LICENSE("GPL");
module_init(sys_reg_init)
module_exit(sys_reg_exit)







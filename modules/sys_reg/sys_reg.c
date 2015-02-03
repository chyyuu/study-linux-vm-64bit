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


static int sys_reg_show(struct seq_file *m, void *v)
{
    unsigned long cr0, cr3, cr4;

    seq_printf(m, "\n----  Control Registers ----\n");

    cr0 = read_cr0();
    seq_printf(m, "CR0 = 0x%lX\n", cr0);
    seq_printf(m, "CR0.PG = %ld\n", cr0 >> 31);

    cr4 = read_cr4();
    seq_printf(m, "CR4 = 0x%lX\n", cr4);
    seq_printf(m, "CR4.PAE = %ld\n", (cr4 >> 5) & 0x1);

    cr3 = read_cr3();
    seq_printf(m, "cr3 = 0x%lX\n", cr3);
    seq_printf(m, "CR3 PT Base = 0x%lX\n", cr3 & 0xFFFFFFFFFFFFF000);;

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







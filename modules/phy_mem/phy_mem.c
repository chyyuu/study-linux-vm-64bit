#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/highmem.h>



#define PHY_DEV_NAME "phy_mem"


typedef struct{
    struct cdev cdev;

    dev_t dev;
    struct class* class;
}phy_mem_cdev_t;

phy_mem_cdev_t phy_mem_cdev;

loff_t phy_mem_lseek(struct file *filp, loff_t off, int whence)
{
    loff_t newpos;

    printk("lseek: off:%lld, whence:%d\n", off, whence);

    switch(whence) {
    case 0: /* SEEK_SET */
        newpos = off;
        break;

    case 1: /* SEEK_CUR */
        newpos = filp->f_pos + off;
        break;

    default: /* can't happen */
        printk("lseek default\n");
        return -EINVAL;
    }
    if (newpos < 0){
        printk("newpos < 0, \n");
        return -EINVAL;
    }
    filp->f_pos = newpos;
    return newpos;
}

ssize_t phy_mem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    struct page *page;
    int page_num, page_offset;
    char *from;
    int ret;

    page_num = *f_pos / PAGE_SIZE;
    page_offset = *f_pos % PAGE_SIZE;
    if(page_offset + count > PAGE_SIZE){//can't beyond one page
        count = PAGE_SIZE - page_offset;
    }

    printk("read addr %16llX (%d:%d)\n", *f_pos, page_num, page_offset );

    page = pfn_to_page(page_num);
    from = (char*)kmap(page) + page_offset;

    ret = copy_to_user(buf, from, count);

    if(ret){
        printk("copy fail ret %d, %p, %p, %d\n", ret, buf, from, (int)count);
        kunmap(page);
        return -EFAULT;
    }
    kunmap(page);

    *f_pos += count;
    return count;
}

static const struct file_operations phy_mem_fops =
{
    .owner      = THIS_MODULE,
    .llseek     = phy_mem_lseek,
    .read       = phy_mem_read,
};


static int __init phy_mem_init(void) {
    int ret;

    ret = alloc_chrdev_region(&phy_mem_cdev.dev, 0, 1, PHY_DEV_NAME);
    if(ret < 0){
        printk("alloc fail\n");
        return ret;
    }

    cdev_init(&phy_mem_cdev.cdev, &phy_mem_fops);
    phy_mem_cdev.cdev.owner = THIS_MODULE;


    ret = cdev_add(&phy_mem_cdev.cdev, phy_mem_cdev.dev, 1);
    if(ret < 0){
        printk("cdev add failed \n");
        return ret;
    }

    phy_mem_cdev.class = class_create(THIS_MODULE, "phy_mem");
    device_create(phy_mem_cdev.class, NULL, phy_mem_cdev.dev, NULL, "phy_mem");

    printk("phy_mem dev init done\n");
    return 0;
}

static void __exit phy_mem_exit(void) {
    device_destroy(phy_mem_cdev.class, phy_mem_cdev.dev);
    class_destroy(phy_mem_cdev.class);

    cdev_del(&phy_mem_cdev.cdev);

    unregister_chrdev_region(phy_mem_cdev.dev, 1);

    printk("phy_mem dev exit done\n");
}


MODULE_LICENSE("GPL");
module_init(phy_mem_init)
module_exit(phy_mem_exit)







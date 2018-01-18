#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>

#define VRAM_BASE 0xe0000000
#define VRAM_SIZE 0x00000200

static void __iomem *vram;
static dev_t first;
static struct class *c1;
static struct cdev c_dev;

static int vram_open (struct inode *i, struct file *f)
{
    printk (KERN_INFO "vram: open()");
    return 0;
}
static int vram_close (struct inode *i, struct file *f)
{
    printk (KERN_INFO "vram: close()");
    return 0;
}
static ssize_t vram_read (struct file *f, char __user *buf,
        size_t len, loff_t *off)
{
    int i;
    u8 byte;

    if (*off >= VRAM_SIZE)
    {
        return 0;
    }
    if (*off + len > VRAM_SIZE)
    {
        len = VRAM_SIZE - *off;
    }
    for (i=0; i<len; i++)
    {
        byte = ioread8((u8*)vram + *off + i);
        if (copy_to_user(buf + i , &byte, 1))
        {
            return -EFAULT;
        }
    }
    *off += len;
    return len;

}
static ssize_t vram_write (struct file *f, const char __user *buf,
        size_t len, loff_t *off)
{
    int i;
    u8 byte;

    if (*off >= VRAM_SIZE)
    {
        return 0;
    }
    if (*off + len > VRAM_SIZE)
    {
        len = VRAM_SIZE - *off;
    }
    for (i=0; i<len; i++)
    {
        if (copy_from_user(&byte, buf+i, 1))
        {
            return -EFAULT;
        }
        iowrite8(byte, (u8*)vram + *off + i);
    }
    *off += len;
    return len;

}
static struct file_operations vram_fops =
{
    .owner = THIS_MODULE,
    .open = vram_open,
    .release = vram_close,
    .write = vram_write,
    .read = vram_read

};
static int __init vram_init(void)
{
    printk (KERN_INFO "vram: registered");
    if ((vram = ioremap (VRAM_BASE, VRAM_SIZE)) == NULL)
    {
        printk (KERN_INFO "Mapping video RAM failed!!");
        return -1;
    }

    if (alloc_chrdev_region(&first, 0, 1, "vram")  < 0)
    {
        return -1;
    }
    if ((c1 = class_create(THIS_MODULE, "chrdev")) == NULL)
    {
        unregister_chrdev_region(first, 1);
    }
    if ((device_create(c1, NULL, first, NULL , "vram")) == NULL)
    {
        class_destroy(c1);
        unregister_chrdev_region (first, 1);
        return -1;
    }
    cdev_init (&c_dev, &vram_fops);
    if (cdev_add (&c_dev, first, 1) == -1)
    {
        device_destroy (c1, first);
        class_destroy(c1);
        unregister_chrdev_region (first, 1);
        return -1;
    }
    return 0;
}

static void __exit vram_exit(void)
{
    printk (KERN_INFO "vram: unregistered");
    cdev_del (&c_dev);
    device_destroy (c1, first);
    class_destroy(c1);
    unregister_chrdev_region (first, 1);
}

module_init (vram_init);
module_exit (vram_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sebin S Philip <sebinsphilip_at_gmail_dot_com>");
MODULE_DESCRIPTION("DeviceDriver v0.5");

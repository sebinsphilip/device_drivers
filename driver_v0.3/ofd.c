#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

static dev_t first;
static struct class *c1;
static struct cdev c_dev;

static int ofd_open (struct inode *i, struct file *f)
{
    printk (KERN_INFO "ofd: open()");
    return 0;
}
static int ofd_close (struct inode *i, struct file *f)
{
    printk (KERN_INFO "ofd: close()");
    return 0;
}
static ssize_t ofd_read (struct file *f, char __user *buf,
        size_t len, loff_t *off)
{
    printk (KERN_INFO "ofd: read()");
    return 0;
}
static ssize_t ofd_write (struct file *f, const char __user *buf,
        size_t len, loff_t *off)
{
    printk (KERN_INFO "ofd: write()");
    return len;
}
static struct file_operations ofd_ops =
{
    .owner = THIS_MODULE,
    .open = ofd_open,
    .release = ofd_close,
    .write = ofd_write,
    .read = ofd_read

};

static int __init ofd_init(void)
{
    printk (KERN_INFO "ofd: registered");
    if (alloc_chrdev_region(&first, 0, 1, "CustomChrDev")  < 0)
    {
        return -1;
    }
    if ((c1 = class_create(THIS_MODULE, "chrdev")) == NULL)
    {
        unregister_chrdev_region(first, 1);
    }
    if ((device_create(c1, NULL, first, NULL , "mynull")) == NULL)
    {
        class_destroy(c1);
        unregister_chrdev_region (first, 1);
        return -1;
    }
    cdev_init (&c_dev, &ofd_ops);
    if (cdev_add (&c_dev, first, 1) == -1)
    {
        device_destroy (c1, first);
        class_destroy(c1);
        unregister_chrdev_region (first, 1);
        return -1;
    }

    return 0;
}

static void __exit ofd_exit(void)
{
    printk (KERN_INFO "ofd: unregistered");
    cdev_del (&c_dev);
    device_destroy (c1, first);
    class_destroy(c1);
    unregister_chrdev_region (first, 1);
}

module_init (ofd_init);
module_exit (ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sebin S Philip <sebinsphilip_at_gmail_dot_com>");
MODULE_DESCRIPTION("DeviceDriver v0.1");

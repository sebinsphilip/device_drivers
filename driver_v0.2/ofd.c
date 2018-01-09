#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

static dev_t first_device;

static int __init ofd_init(void)
{
        printk (KERN_INFO "Hello: ofd registered!! ");
	if (alloc_chrdev_region(&first_device, 0, 3, "CustomCharDev") < 0)
	{
		return -1;
	}
	printk (KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first_device), MINOR(first_device));
        return 0;
}

static void __exit ofd_exit(void)
{
	unregister_chrdev_region (first_device, 3);
        printk (KERN_INFO "Goodbye: ofd exited");
}

module_init (ofd_init);
module_exit (ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sebin S Philip <sebinsphilip_at_gmail_dot_com>");
MODULE_DESCRIPTION("DeviceDriver v0.1");

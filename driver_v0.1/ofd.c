#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

static int __init ofd_init(void)
{
        printk (KERN_INFO "Hello: ofd registered!! ");
        return 0;
}

static void __exit ofd_exit(void)
{
        printk (KERN_INFO "Goodbye: ofd exited");
}

module_init (ofd_init);
module_exit (ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sebin S Philip <sebinsphilip_at_gmail_dot_com>");
MODULE_DESCRIPTION("DeviceDriver v0.1");

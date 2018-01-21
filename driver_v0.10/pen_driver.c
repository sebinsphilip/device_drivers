#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/module.h>


#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x02 
#define BULK_EP_IN 0x81
#define MAX_PKT_SIZE 512

static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

static int pen_open(struct inode *i, struct file *f)
{
	return 0;
}

static int pen_close(struct inode *i, struct file *f)
{
	return 0;
}

static ssize_t pen_read(struct file *f, char __user *buf, int cnt, loff_t *off)
{
	int iReadCount = 0, iRetVal = 0;
	iRetVal = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_EP_IN),
			bulk_buf, MAX_PKT_SIZE, &iReadCount, 5000);
	if (iRetVal)
	{
		printk (KERN_INFO "[+++++PEN_DRIVER+++++]usb_bulk_msg() returned: %d\n", iRetVal);
	}
	if (copy_to_user(buf, bulk_buf, MIN(iReadCount, cnt)))
	{
		return -EFAULT;
	}
	return MIN(iReadCount, cnt);
}

static ssize_t pen_write(struct file *f, const char __user *buf, int cnt, loff_t *off)
{
	int iWroteCount = MIN(cnt, MAX_PKT_SIZE);
	int iRetVal = 0;
	if (copy_from_user(bulk_buf, buf, MIN(cnt, MAX_PKT_SIZE)))
	{
		return -EFAULT;
	}
	iRetVal = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT),
			bulk_buf, MIN(cnt,MAX_PKT_SIZE), &iWroteCount, 5000);
	if (iRetVal)
	{
		printk (KERN_INFO "[+++++PEN_DRIVER+++++]usb_bulk_msg() returned :%d\n", iRetVal);
		return iRetVal;
	}
	return iWroteCount;
}

static struct file_operations usb_fops = 
{
	.open = pen_open,
	.release = pen_close,
	.read = pen_read,
	.write = pen_write
};

static int pen_probe(struct usb_interface *interface, struct usb_device_id *id)
{
	int iRetVal = 0;
	device = interface_to_usbdev(interface);
	class.name = "usb/pen%d";
	class.fops = &usb_fops;
	if ((iRetVal = usb_register_dev(interface, &class)) < 0)
	{
		printk (KERN_INFO "[+++++PEN_DRIVER+++++] not able to get minor for this device\n");

	}
	else
	{
		printk (KERN_INFO "[+++++PEN_DRIVER+++++] Minor obtained :%d\n", interface->minor);
	}
	return iRetVal;
}

static void pen_disconnect(struct usb_interface *interface)
{
	usb_deregister_dev(interface, &class);

}
static struct usb_device_id pen_table[] =
{
	{USB_DEVICE(0x0781, 0x5567)},
	{}
};

static struct usb_driver pen_driver = 
{
	.name = "pen_driver",
	.probe = pen_probe,
	.disconnect = pen_disconnect,
	.id_table = pen_table
};

static int __init pen_init(void)
{
	int iRet = 0;
	if ((iRet = usb_register(&pen_driver)) < 0)
	{
		printk (KERN_INFO "[+++++PEN_DRIVER+++++] usb_register() failed error code:%d\n", iRet);
	}
	return iRet;
}

static void __exit pen_exit(void)
{
	usb_deregister(&pen_driver);
}
module_init(pen_init);
module_exit(pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sebin S Philip");
MODULE_DESCRIPTION("USB device driver to simulate bulk data transfer operations");

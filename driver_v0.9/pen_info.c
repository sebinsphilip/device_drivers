#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

static struct usb_device *device;

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct usb_host_interface *iface_desc = interface->cur_altsetting;
	struct usb_endpoint_descriptor *endpoint;
	int i;


	printk (KERN_INFO "[++++PEN_INFO++++] Pen i/f %d now probed: (%04X:%04X)\n",
			iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
	printk (KERN_INFO "[++++PEN_INFO++++] ID->bNumEndpoints : %02X\n",
			iface_desc->desc.bNumEndpoints);
	printk (KERN_INFO "[++++PEN_INFO++++] ID->bInterfaceClass : %02X\n",
			iface_desc->desc.bInterfaceClass);
	for (i=0; i < iface_desc->desc.bNumEndpoints; i++)
	{
		endpoint = &iface_desc->endpoint[i].desc;

		printk (KERN_INFO "[++++PEN_INFO++++] ED[%d]->bEndpointAddress : %02X\n",
				i, endpoint->bEndpointAddress);
		printk (KERN_INFO "[++++PEN_INFO++++] ED[%d]->bmAttributes : %02X\n",
				i, endpoint->bmAttributes);
		printk (KERN_INFO "[++++PEN_INFO++++] ED[%d]->wMaxPacketSize : %04X (%d)\n",
				i, endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);

	}

	device = interface_to_usbdev(interface);
	return 0;
}

static void pen_disconnect(struct usb_interface *interface)
{
	printk (KERN_INFO "[++++PEN_INFO++++] Pendrive i/f %d disconnected\n",
			interface->cur_altsetting->desc.bInterfaceNumber);
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

MODULE_DEVICE_TABLE(usb, pen_table);

static int __init pen_init(void)
{
	return usb_register(&pen_driver);
}

static void __exit pen_exit(void)
{
	usb_deregister(&pen_driver);
}

module_init (pen_init);
module_exit (pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sebin S Philip");
MODULE_DESCRIPTION("usb driver for i/c registration and printing i/c info");

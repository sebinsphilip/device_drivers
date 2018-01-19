# device_drivers
linux device driver experiments

note:
* 'cat /proc/devices' will contain the device (<major,minor>) which
   is registered using alloc_chrdev_region()

* udev daemon will automatically create a device node after device class
  creation and device creation using class_create() and device_create()
    The class can be observed under /sys/classes/

* cat /proc/iomem will list the memory map 
  (Physical address - RAM; bus address - device map)

* cat /proc/meminfo will list the RAM/System config info.

* driverv_0.5 : lspci -> lspci -v -s 00:02.0 (00:02.0 is the param of VGA)
  we'll get the Video RAM mem info and address space.

* In x86 systems, we can also access the h/w device through direct I/O mapping(no
  virtual memory mapping needed).
  Through port addresses, using inb() inw() inl() and outb() outw() outl().
  The port address info can be found by 'cat /proc/ioports'.

* driverv_0.6 : serial port is controlled by UART/USART (PC16550D)
    Base address is : 0x3F8
    Contains 8 registers
    <linux/serial_reg.h>

* driverv_0.7: ioctl() is a common operation/system call available in most drivers
    for device specific io controls (command, argument) ;both command and argument type
    information need to be shared across userspace and kernel space.


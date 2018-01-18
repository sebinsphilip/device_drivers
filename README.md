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

* cat /proc/meminfo will list the RAM/System config info

* driverv_0.5 : lspci -> lspci -v -s 00:02.0 (00:02.0 is the param of VGA)
  we'll get the Video RAM mem info and address space


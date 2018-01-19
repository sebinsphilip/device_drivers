#include <linux/module.h>
#include <linux/version.h>
#include <linux/serial_reg.h>
#include <linux/delay.h>
#include <asm/io.h>

#define SERIAL_BASE_ADDRESS 0x03F8

int __init init_module_uart (void)
{
    u8 byte;
    int i;
    byte = inb(SERIAL_BASE_ADDRESS + UART_LCR);
    for (i=0; i<5; i++)
    {
        byte |= UART_LCR_SBC;
        outb(byte, SERIAL_BASE_ADDRESS + UART_LCR);
        byte &= ~UART_LCR_SBC;
        outb(byte, SERIAL_BASE_ADDRESS + UART_LCR);
        msleep(500);
    }
    return 0;
}

void __exit cleanup_module_uart (void)
{

}

module_init (init_module_uart);
module_exit (cleanup_module_uart);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sebin S Philip");
MODULE_DESCRIPTION("LED UART driver");

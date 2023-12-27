#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "typedefs.h"
#include "idt.h"
#include "ports.h"
#include "disk.h"

extern unsigned char keypressed_buff;

void kmain()
{
    kbmap_init();
    init_idt();

    uint16 identify_command_vals[256];
    uint8 read_buffer[2048];
    volatile char status = 0;

    asm volatile ("sti");

    clear_screen();

    char* read = (char*)read_buffer;

    int s = 1040;
    ATA28_PIO_read(read, 1450, s);

    for (int i = 0; i < s; i++)
    {
        printf("%c", read[i]);
    }

    // the most simple OS system task!
    while (1)
    {
        if (keypressed_buff)
        {
            putchar(keypressed_buff);
            keypressed_buff = 0;
        }
    }

}
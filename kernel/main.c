#include "screen.h"
#include "keyboard.h"
#include "typedefs.h"
#include "idt.h"
#include "ports.h"

extern unsigned char keypressed_buff;

void kmain()
{
    kbmap_init();
    init_idt();

    asm volatile ("sti");

    clear_screen();

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
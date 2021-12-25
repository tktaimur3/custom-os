#include "screen.h"
#include "keyboard.h"
#include "typedefs.h"
#include "idt.h"
#include "ports.h"

void kmain()
{
    init_idt();

    asm volatile ("sti");

    clear_screen();
    while(1);
}
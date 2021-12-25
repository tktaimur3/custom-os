#include "keyboard.h"
#include "screen.h"
#include "idt.h"

void keyboard_callback (uint8 scancode)
{
    printf("%d", scancode);
}
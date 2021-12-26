#include "keyboard.h"
#include "screen.h"
#include "idt.h"

unsigned char keypressed_buff = 0;

unsigned char scancode2key[256] = {0};

unsigned char caps = 0;
unsigned shiftpressed = 0;

void kbmap_init()
{
    scancode2key[0x1E] = 'a';
    scancode2key[0x30] = 'b';
    scancode2key[0x2E] = 'c';
    scancode2key[0x20] = 'd';
    scancode2key[0x12] = 'e';
    scancode2key[0x21] = 'f';
    scancode2key[0x22] = 'g';
    scancode2key[0x23] = 'h';
    scancode2key[0x17] = 'i';
    scancode2key[0x24] = 'j';
    scancode2key[0x25] = 'k';
    scancode2key[0x26] = 'l';
    scancode2key[0x32] = 'm';
    scancode2key[0x31] = 'n';
    scancode2key[0x18] = 'o';
    scancode2key[0x19] = 'p';
    scancode2key[0x10] = 'q';
    scancode2key[0x13] = 'r';
    scancode2key[0x1F] = 's';
    scancode2key[0x14] = 't';
    scancode2key[0x16] = 'u';
    scancode2key[0x2F] = 'v';
    scancode2key[0x11] = 'w';
    scancode2key[0x2D] = 'x';
    scancode2key[0x15] = 'y';
    scancode2key[0x2C] = 'z';

    scancode2key[0x0B] = '0';
    scancode2key[0x02] = '1';
    scancode2key[0x03] = '2';
    scancode2key[0x04] = '3';
    scancode2key[0x05] = '4';
    scancode2key[0x06] = '5';
    scancode2key[0x07] = '6';
    scancode2key[0x08] = '7';
    scancode2key[0x09] = '8';
    scancode2key[0x0A] = '9';

    scancode2key[0x29] = '`';
    scancode2key[0x0C] = '-';
    scancode2key[0x0D] = '=';
    scancode2key[0x2B] = '\\';

    scancode2key[0x39] = ' ';

    // backspace
    scancode2key[0x0E] = 0x0E;

    // enter
    scancode2key[0x1C] = '\n';

    // tab
    scancode2key[0x0F] = '\t';
}

void keyboard_callback (uint8 scancode)
{
    // check for caps lock and lshift rshift
    if (scancode == 0x3A)
    {
        caps = ~caps;
    }

    // If shift is pressed (if it wasnt already pressed) switch the caps
    if (!shiftpressed && ( scancode == 0x2A || scancode == 0x36 ))
    {
        shiftpressed = 1;
        caps = ~caps;
    }

    // If shift is released (if it wasnt already released) switch the caps
    if (shiftpressed && ( scancode == 0xAA || scancode == 0xB6 ))
    {
        shiftpressed = 0;
        caps = ~caps;
    }

    keypressed_buff = scancode2key[scancode];

    if (caps)
    {
        if (keypressed_buff >= 'a' && keypressed_buff <= 'z')
        {
            keypressed_buff -= 32;
        }
        else if (keypressed_buff >= 'A' && keypressed_buff <= 'Z')
        {
            keypressed_buff += 32;
        }
    }
}
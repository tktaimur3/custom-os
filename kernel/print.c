#include "print.h"

short* vga = (short*)0xb8000;

void set_background()
{
	for (int i = 0; i < 80*25; i++)
		vga[i] = BACKGROUND_COLOUR |  ' ';
}

int strlen(const char *_string)
{
    int length = 0;

    while (_string[length] != 0)
        length++;

    return length;
}

void print(const char *_message)
{
    int string_length = strlen(_message);
	
	for (int i = 0; i < string_length; i++)
		vga[i] = BACKGROUND_COLOUR |  _message[i];
}
#include "screen.h"
#include "ports.h"

char *convert(unsigned int num, unsigned int base);
void puts(char *string);
void putchar(char character);
int get_cursor();
void set_cursor(int offset);

// Code adapted from: https://stackoverflow.com/a/28581294
void printf(char *format, ...)
{
    char *traverse; 
    unsigned int i = 0; 
    char *s; 

    va_list arg; 
    va_start(arg, format); 

    for(traverse = format; *traverse != '\0';) 
    { 
        while( *traverse != '%' && *traverse != '\0' ) 
        { 
            putchar(*traverse++);
        } 

        if (*traverse)
        {
            traverse++; 

            switch(*(traverse)++) 
            { 
                case 'c' : i = va_arg(arg,int);     //Fetch char argument
                    putchar(i);
                    break; 

                case 'd' : i = va_arg(arg,int);         //Fetch Decimal/Integer argument
                    if(i<0) 
                    { 
                        i = -i;
                        putchar('-'); 
                    } 
                    puts(convert(i,10));
                    break;

                case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
                    puts(convert(i,8));
                    break; 

                case 's': s = va_arg(arg,char *);       //Fetch string
                    puts(s); 
                    break;

                case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                    puts(convert(i,16));
                    break; 
            }   
        }
    } 

    va_end(arg); 
} 

void puts(char *string)
{
    int string_length = strlen(string);
    int offset = get_cursor();

    for (int i = 0; i < string_length; i++)
    {
        putchar(string[i]);
    }
}

void putchar(char character)
{
    unsigned short* vga = (short*) VIDEO_ADDR;
    int offset = get_cursor();

    // Calculate where to put cursor when newline is there
    if (character == '\n')
    {
        offset = ((offset/SCREEN_WIDTH)+1)*SCREEN_WIDTH;
    } // Calculate next tab stop, code from: https://stackoverflow.com/a/13094746
    else if (character == '\t')
    {
        offset = (offset + 8) / 8 * 8;
    }
    else
    {
        vga[offset++] = BACKGROUND_COLOUR |  character;
    }

    set_cursor(offset);
}

// Code entirely yoinked from: https://stackoverflow.com/a/28581294
char *convert(unsigned int num, unsigned int base) 
{ 
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50]; 
    char *ptr; 

    ptr = &buffer[49]; 
    *ptr = '\0'; 

    do 
    { 
        *--ptr = Representation[num%base]; 
        num /= base; 
    }while(num != 0); 

    return(ptr); 
}

int get_cursor()
{
    int pos;

    outb(REG_SCREEN_CTRL, 0x0E);      // set index register to address 0x0E to get MS byte of cursor
    pos = inb(REG_SCREEN_DATA) << 8;  // get the MS byte of cursor then move it accordingly
    outb(REG_SCREEN_CTRL, 0x0F);      // set index register to address 0x0F to get LS byte of cursor
    pos = pos | inb(REG_SCREEN_DATA); // get the LS byte of cursor then OR it to get cursor position

    return pos;
}

void set_cursor(int offset)
{
    outb(REG_SCREEN_CTRL, 0x0E);
    outb(REG_SCREEN_DATA, (unsigned char) (offset >> 8));

    offset = (0xFF) & offset;
    outb(REG_SCREEN_CTRL, 0x0F);
    outb(REG_SCREEN_DATA, offset);
}

void clear_screen()
{
    unsigned short* vga = (short*) VIDEO_ADDR;
    int pixels = SCREEN_WIDTH*SCREEN_HEIGHT;

	for (int i = 0; i < pixels; i++)
		vga[i] = BACKGROUND_COLOUR |  ' ';

    set_cursor(0);
}

int strlen(const char *string)
{
    int length = 0;

    while (string[length] != 0)
        length++;

    return length;
}
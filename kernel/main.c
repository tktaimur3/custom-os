#include "print.h"

char* hello = "Hello world!";

// Add in support for ports (inline assembly stuff)
// From here: http://www.osdever.net/tutorials/view/mixing-assembly-c

void kmain()
{
    set_background();
    print(hello);
}
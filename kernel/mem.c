#include "mem.h"

void* memcpy(char* dest, const char* src, size_t n)
{
    for (int i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }

    return dest;
}
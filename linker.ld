OUTPUT_FORMAT("binary")
ENTRY(boot)

OUTPUT
(
    kernel.bin
)

SECTIONS {
    . = 0x7c00;
    .text :
    {
        *(.boot)
        *(.text)
    }
    .rodata :
    {
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}
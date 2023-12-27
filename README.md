# Custom OS from scratch

A side project that I'm doing on my own time: trying to create a custom OS from scratch. This includes creating the bootloader from scratch.

It's a 32-bit OS targeted towards x86 CPUs. My main goal with this is to learn as much as I can about how modern operating systems work and also learn how the x86 architecture works under the hood.

So far here are some of the things I've implemented:

- Booted via custom bootloader created in assembly from scratch, switching from Real Mode (16 bit) to Protected Mode (32 bit)
- Jumping to C code (the kernel)
- Implemented basic video driver to handle video output
- Set up and enabled hardware interrupts (keyboard, timer)
- Handled keyboard scancodes to output correct keystrokes
- Created ATA PIO mode driver for disk read/write

To build this you would need to download the i686 cross compiler. In addition you would need to edit some things in the Makefile very slightly as it has been tailored for my system. All you need in the end is the kernel.bin file, which can be run on Bochs or QEMU.

Cross-compiler download guide: https://wiki.osdev.org/GCC_Cross-Compiler

Credit to the following resources for their amazing tutorials/documentation (also linked in places throughout the code):
- https://github.com/cfenollosa/os-tutorial
- http://www.jamesmolloy.co.uk/tutorial_html/
- http://www.osdever.net/tutorials/
- https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf

In addition, you must run qemu like so:

```qemu-system-i386 kernel.bin -drive file=file.img,format=raw,media=disk```

This runs QEMU with the built kernel file as a floppy disk and with ```file.img``` is a second drive
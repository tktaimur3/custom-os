# Custom OS from scratch

Just a little side project that I'm doing on my own time: trying to create a custom OS from scratch. This includes creating the bootloader from scratch and not relying on something like GRUB

It's a 32-bit OS targeted towards x86 CPUs. My main goal with this is to learn as much as I can about how modern operating systems work. I hope to at least have a command-line interface and maybe some basic graphics. I also hope to learn how the x86 architecture works under the hood.

So far some things that have been added are as follows:

- Booted via custom bootloader created in assembly from scratch, switching from Real Mode (16 bit) to Protected Mode (32 bit)
- Jumping to C code (the kernel)
- Implemented basic video driver to handle video output
- Set up and enabled hardware interrupts (keyboard, timer)
- Handled keyboard scancodes to output correct keystrokes
- Created ATA PIO mode driver for disk read/write

To build this you would need to download the i686 cross compiler. In addition you would need to edit some things in the Makefile very slightly as it has been tailored for my system. All you need in the end is the bootloader.flp file, which can be run on Bochs or QEMU.

Cross-compiler download guide: https://wiki.osdev.org/GCC_Cross-Compiler

Credit to the following resources for their amazing tutorials/documentation (also linked in places throughout the code):
- https://github.com/cfenollosa/os-tutorial
- http://www.jamesmolloy.co.uk/tutorial_html/
- http://www.osdever.net/tutorials/
- https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf


-drive file=test.img,format=raw,media=disk
# Custom OS from scratch

Just a little side project that I'm doing on my own time: trying to create a custom OS from scratch. This includes creating the bootloader from scratch and not relying on something like GRUB

It's a 32-bit OS targeted towards x86 CPUs. My main goal with this is to learn as much as I can about how modern operating systems work. I hope to at least have a command-line interface and maybe some basic graphics.

So far some things that have been added are as follows:

- Booted via custom bootloader created in assembly from scratch
- Jumping to C code (the kernel)
- Implemented basic video driver to handle video output
- Enabled interrupts (keyboard, timer)
- Handled keyboard scancodes to output correct keystrokes
- Implemented basic timing functions

Credit to the following resources (also linked in places throughout the code):
- https://github.com/cfenollosa/os-tutorial
- http://www.jamesmolloy.co.uk/tutorial_html/
- http://www.osdever.net/tutorials/
- https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf

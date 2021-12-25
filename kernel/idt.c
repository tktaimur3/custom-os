#include "idt.h"
#include "ports.h"
#include "screen.h"
#include "keyboard.h"

static void idt_install();
static void idt_set_gate(uint8 num, uint32 base);

extern void keyboard_callback(uint8 scancode);

idt_entry idt_entries[256]; // the 256 entries that make up our IDT
idt_ptr _idt_ptr;            // the pointer to our idt

void init_idt()
{
    _idt_ptr.limit = sizeof(idt_entry)*256 - 1;
    _idt_ptr.base = (uint32)&idt_entries;

    // from: https://github.com/cfenollosa/os-tutorial
    // for now, just using the keyboard and timer IRQ, could have gone by with just those in my code
    // most likely will refactor later to account for that

    idt_set_gate(0, (uint32)isr0);
    idt_set_gate(1, (uint32)isr1);
    idt_set_gate(2, (uint32)isr2);
    idt_set_gate(3, (uint32)isr3);
    idt_set_gate(4, (uint32)isr4);
    idt_set_gate(5, (uint32)isr5);
    idt_set_gate(6, (uint32)isr6);
    idt_set_gate(7, (uint32)isr7);
    idt_set_gate(8, (uint32)isr8);
    idt_set_gate(9, (uint32)isr9);
    idt_set_gate(10, (uint32)isr10);
    idt_set_gate(11, (uint32)isr11);
    idt_set_gate(12, (uint32)isr12);
    idt_set_gate(13, (uint32)isr13);
    idt_set_gate(14, (uint32)isr14);
    idt_set_gate(15, (uint32)isr15);
    idt_set_gate(16, (uint32)isr16);
    idt_set_gate(17, (uint32)isr17);
    idt_set_gate(18, (uint32)isr18);
    idt_set_gate(19, (uint32)isr19);
    idt_set_gate(20, (uint32)isr20);
    idt_set_gate(21, (uint32)isr21);
    idt_set_gate(22, (uint32)isr22);
    idt_set_gate(23, (uint32)isr23);
    idt_set_gate(24, (uint32)isr24);
    idt_set_gate(25, (uint32)isr25);
    idt_set_gate(26, (uint32)isr26);
    idt_set_gate(27, (uint32)isr27);
    idt_set_gate(28, (uint32)isr28);
    idt_set_gate(29, (uint32)isr29);
    idt_set_gate(30, (uint32)isr30);
    idt_set_gate(31, (uint32)isr31);
    
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0); 

    idt_set_gate(32, (uint32)irq0);
    idt_set_gate(33, (uint32)irq1); // keyboard IRQ
    idt_set_gate(34, (uint32)irq2);
    idt_set_gate(35, (uint32)irq3);
    idt_set_gate(36, (uint32)irq4);
    idt_set_gate(37, (uint32)irq5);
    idt_set_gate(38, (uint32)irq6);
    idt_set_gate(39, (uint32)irq7);
    idt_set_gate(40, (uint32)irq8);
    idt_set_gate(41, (uint32)irq9);
    idt_set_gate(42, (uint32)irq10);
    idt_set_gate(43, (uint32)irq11);
    idt_set_gate(44, (uint32)irq12);
    idt_set_gate(45, (uint32)irq13);
    idt_set_gate(46, (uint32)irq14);
    idt_set_gate(47, (uint32)irq15);

    idt_install((uint32)&_idt_ptr);
}

static void idt_set_gate(uint8 num, uint32 base_addr)
{
    idt_entries[num].base_lo = base_addr & 0xFFFF;
    idt_entries[num].base_hi = (base_addr >> 16) & 0xFFFF;

    idt_entries[num].selector = 0x08;
    idt_entries[num].reserved = 0;
    idt_entries[num].flags = 0x8e;
}

void irq_handler(registers_t regs)
{
    uint8 scancode;
    // Check what interrupt handler to call
    switch (regs.int_no)
    {
        case 33:
            scancode = inb(0x60);
            keyboard_callback(scancode);
            break;
    }

    outb(0xA0, 0x20);
    outb(0x20, 0x20); 
}

void isr_handler(registers_t regs)
{
    // todo : implement
}

static void idt_install(uint32 idt)
{
    __asm__ __volatile__ ("lidtl (%0)" : : "r" (&_idt_ptr));
}

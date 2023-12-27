section .boot
[BITS 16]
global boot

boot:
    mov [disk], dl ; BIOS stores the drive we booted from in dl, copy that somewhere (will be floppy disk in our case)

    ; load more than 512 bytes into memory (i.e., the rest of the program)
    mov ah, 0x2     ; read sector function - set ah to 0x02
    mov al, 32      ; loading 32 sectors from disk (512 bytes each, more than we need)
    mov ch, 0       ; HDD specific - read starting cylinder 0
    mov dh, 0       ; HDD specific - read starting on head 0 of this cylinder
    mov cl, 2       ; HDD specific - read starting second sector on the track, this is indexed starting from 1
                    ; giving 1 would mean loading these first 512 bytes again, not what we want
    mov dl, [disk]  ; what drive to read from, we established that this is the boot drive
    mov bx, target  ; pointer to the address in RAM of where we're loading the data (as in from disk to this address in RAM)
    int 0x13        ; BIOS interrupt to load the data

    ; switch to protected mode (32-bit mode)
    cli             ; disable interrupts (interrupts occuring during this process can cause problems)
    lgdt [gdt_desc] ; load gdt by giving gdt_desc pointer

    ; set the first bit of cr0 register to to enter protected mode
    ; cannot do it directly so we:
    mov eax, cr0    ; get cr0 reg and store it
    or eax, 1       ; flip the first bit to 1
    mov cr0, eax    ; put that back into cr0

    ; pipeline may contain incorrect 16-bit prefetched instructions so we jump somwhere to clear the pipeline
    jmp codeseg:clear_pipe

%include "gdt.asm"

times 510-($-$$) db 0
dw 0xAA55

target:

[BITS 32]
[extern kmain]
clear_pipe:
    ; set all segment register to dataseg, its old values were for 16-bit mode
    mov ax, dataseg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; set the stack accordingly to the top of the stack we created below
    mov esp, kernel_stack_top
    mov ebp, esp

    ; call the kernel main function
    call kmain

    cli
    hlt

section .bss
align 4
kernel_stack_end: equ $
    resb 16384
kernel_stack_top:

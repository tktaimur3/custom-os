section .boot
[BITS 16]
global boot

boot:
    mov [disk], dl

    ; load more than 512 bytes into memory

    mov ah, 0x2
    mov al, 32      ; loading 32 sectors (512 bytes)
    mov ch, 0       ; HDD specific
    mov dh, 0       ; HDD specific
    mov cl, 2       
    mov dl, [disk] 
    mov bx, target ; pointer to what we're loading into RAM
    int 0x13

    cli

    lgdt [gdt_desc] ; load gdt

    mov eax, cr0 
    or eax, 1
    mov cr0, eax ; set CR0 register to enter protected mode

    jmp codeseg:clear_pipe

%include "gdt.asm"

times 510-($-$$) db 0
dw 0xAA55

target:

[BITS 32]
[extern kmain]
clear_pipe:
    mov ax, dataseg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

call halt

halt:
    mov esp, kernel_stack_top
    call kmain
    cli
    hlt

section .bss
align 4
kernel_stack_bottom: equ $
    resb 16384
kernel_stack_top:
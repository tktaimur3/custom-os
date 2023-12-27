; Global Descriptor Table
; defines our segments in memory to the CPU to ensure safe memory accesses between kernel and user processes
; in this case we throw that out the window and just have a flat model where user/kernel data and code segments both overlap entirely

gdt_null:
    dq 0                ; all zeroes

gdt_code:
    dw 0xFFFF           ; limit[0:15]
    dw 0                ; base[0:15]
    db 0                ; base[16:23]
    db 10011010b        ; present/priv level[0:1]/descriptor type/segment type/type[0:2]
    db 11001111b        ; granularity/operation size/64-bit flag/AVL/limit[16:19] 
    db 0                ; base[24:31]

gdt_data:
    dw 0xFFFF           ; limit[0:15]
    dw 0                ; base[0:15]
    db 0                ; base[16:23]
    db 10010010b        ; present/priv level[0:1]/descriptor type/segment type/type[0:2]
    db 11001111b        ; granularity/operation size/64-bit flag/AVL/limit[16:19]
    db 0                ; base[24:31]

gdt_ucode:              ; user code segment (when running things in user mode)
    dw 0xFFFF           ; limit[0:15]
    dw 0                ; base[0:15]
    db 0                ; base[16:23]
    db 10011010b        ; present/priv level[0:1]/descriptor type/segment type/type[0:2]
    db 11001111b        ; granularity/operation size/64-bit flag/AVL/limit[16:19]
    db 0                ; base[24:31]

gdt_udata:              ; user data segment (when running things in user mode)
    dw 0xFFFF           ; limit[0:15]
    dw 0                ; base[0:15]
    db 0                ; base[16:23]
    db 10010010b        ; present/priv level[0:1]/descriptor type/segment type/type[0:2]
    db 11001111b        ; granularity/operation size/64-bit flag/AVL/limit[16:19]
    db 0                ; base[24:31]

gdt_end:

gdt_desc:
    dw gdt_end - gdt_null - 1
    dd gdt_null
disk:
    db 0x0
codeseg equ gdt_code - gdt_null
dataseg equ gdt_data - gdt_null
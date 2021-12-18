; Global Description Table
gdt_null:
    dq 0                ; all null

gdt_code:
    dw 0xFFFF           ; 1st double word - limit[0:15]
    dw 0                ; 1st double word - base[0:15]
    db 0                ; 2nd double word - base[16:23]
    db 10011010b        ; 2nd double word - type/priv level/present
    db 11001111b        ; 2nd double word - limit[16:19]/granularity 
    db 0                ; 2nd double word - base[24:31]

gdt_data:
    dw 0xFFFF           ; 1st double word - limit[0:15]
    dw 0                ; 1st double word - base[0:15]
    db 0                ; 2nd double word - base[16:23]
    db 10010010b        ; 2nd double word - type/priv level/present
    db 11001111b        ; 2nd double word - limit[16:19]/granularity 
    db 0                ; 2nd double word - base[24:31]

gdt_end:

gdt_desc:
    dw gdt_end - gdt_null - 1
    dd gdt_null
disk:
    db 0x0
codeseg equ gdt_code - gdt_null
dataseg equ gdt_data - gdt_null
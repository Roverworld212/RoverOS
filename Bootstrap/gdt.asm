[BITS 32]

%include "./Bootstrap/base_header.inc"

[GLOBAL asm_ldgdt32]

section .rodata

GDT32_LOAD db "Loading GDT32", 0
GDT32_LOADED db "Loaded GDT32", 0
GDT64_LOAD db "Loading GDT64", 0
GDT64_LOADED db "Loaded GDT64", 0

gdt32:
.segment_null: equ $-gdt32
dq 0
.segment_code: equ $-gdt32
dw 0xffff
dw 0
db 0
db 0x9A
db 11001111b
db 0
.segment_data: equ $-gdt32
dw 0xffff
dw 0
db 0
db 0x92
db 11001111b
db 0
.pointer:
dw $-gdt32-1
dq gdt32

GDT64_DESC equ 1 << 44
GDT64_PRES equ 1 << 47
GDT64_REWR equ 1 << 41
GDT64_EXEC equ 1 << 43
GDT64_64B equ 1 << 53

gdt64:
.segment_null: equ $-gdt64
dq 0
.segment_code: equ $-gdt64
dq GDT64_DESC | GDT64_PRES | GDT64_REWR | GDT64_EXEC | GDT64_64B
.segment_data: equ $-gdt64
dq GDT64_DESC | GDT64_PRES | GDT64_REWR | GDT64_64B
.pointer:
dw $-gdt64-1
dq gdt64

section .text

asm_ldgdt32:
debug GDT32_LOAD, COM_INFO_ASM
lgdt [gdt32.pointer]
mov ax, 0x10
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
jmp gdt32.segment_code:.igdt
.igdt:
debug GDT32_LOADED, COM_INFO_ASM
ret

[GLOBAL asm_ldgdt64]

;The code above just loads the 32 bit gdt
;This loads the 64 bit GDT and jumps to the x64 kernel entry

asm_ldgdt64:
debug GDT64_LOAD, COM_INFO_ASM
lgdt [gdt64.pointer]
jmp gdt64.segment_code:mode64

[BITS 64]
[EXTERN kmain]

mode64:
debug64 GDT64_LOADED, COM_INFO_ASM
debug64 MODE64_LOADED, COM_INFO_ASM
debug64 KSTACKDB, COM_INFO_ASM
mov rsp, kstack ;Load kernel stack
mov rdi, rsi ;Arg 1 (Pointer to list of multiboot tag pointers
mov rsi, 0x0
debug64 KJUMP, COM_INFO_ASM
xchg bx, bx
lea rbx, [rel kmain]
jmp rbx ;Jump to kmain() in C
jmp $

[GLOBAL init_gdt64]
;[EXTERN gdtPtr]

init_gdt64:
;lgdt [gdtPtr]
mov ax, 0x10
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
ret

section .rodata

MODE64_LOADED db "Entered long mode (x86_64/64 bit)", 0
KJUMP db "Jumping to kmain", 0
KSTACKDB db "Loading kstack into rsp", 0

section .bss

kstack_end:
resq 64
kstack:
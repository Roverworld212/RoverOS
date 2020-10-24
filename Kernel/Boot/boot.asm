MAGIC equ 0xE85250D6 ;We use multiboot2 now
ARCH equ 0
HEADER_LEN equ -(mboot-mbootend)
CHECK equ -(MAGIC+ARCH+HEADER_LEN)

[GLOBAL mboot]
[GLOBAL start]
[extern code]
[extern bss]
[extern end]
[extern main]

section .multiboot

mboot:
dd MAGIC
dd ARCH
dd HEADER_LEN
dd CHECK
;VBE tag
dw 5
align 8
;ACPI 2 tag
dw 15
align 8
;Network info
dw 16
align 8
;Mboot Console
dw 4
align 8
;APM
dw 10
align 8
;EFI32
dw 11
align 8
;SMBIOS
dw 13
align 8
;Base Load
dw 21
align 8
;Terminating tag
dw 0
dw 0
dd 8
mbootend:

section .data

[EXTERN kstack]

start:
push ebx
push eax
cli
call main
jmp $

[GLOBAL gdtf]
gdtf:
mov eax, [esp+4]
lgdt [eax]
mov ax, 0x10
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
jmp 0x08:.igdt
.igdt:
ret

[GLOBAL idtf]
idtf:
mov eax, [esp+4]
lidt [eax]
ret

section .bss
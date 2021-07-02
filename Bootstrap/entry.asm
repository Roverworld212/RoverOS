;Multiboot2 magic
MAGIC equ 0xE85250D6
ARCH equ 0
HEADER_LEN equ -(mtag_start-mtag_end)
CHECK equ -(MAGIC+ARCH+HEADER_LEN)

section .multiboot

;Multiboot tags
mtag_start:
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
mtag_end:

[BITS 32]
[GLOBAL bootstrap_start]
[EXTERN start_com_asm]
[EXTERN asm_ldgdt32]
[EXTERN asm_ldgdt64]
[EXTERN asm_ldpaging]
[EXTERN read_multiboot]

%include "./Bootstrap/base_header.inc"

section .text

bootstrap_start:
;EAX has the GRUB2 magic/my bootloader magic
;EBX has the pointer to the tag start
call start_com_asm
debug BOOTSTRAP_LOADED, COM_INFO_ASM
;Check for long mode
push eax
push ebx
mov eax, 0x80000001
cpuid
and edx, 0x8
cmp edx, 0x0
je .no_lm
pop ebx
pop eax 
call read_multiboot
push esi
call asm_ldgdt32
call asm_ldpaging
pop esi
call asm_ldgdt64
jmp .lp
.no_lm:
debug NO_LME, COM_INFO_ASM
.lp:
hlt
jmp .lp

section .rodata

BOOTSTRAP_LOADED db "Bootstrap loaded", 0
NO_LME db "This CPU does not support long mode, the OS cannot continue", 0
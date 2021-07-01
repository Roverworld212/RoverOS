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
;VBE
dw 5
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
[EXTERN asm_ldpaging]

%include "./Bootstrap/base_header.inc"

%macro debug 0-2
    push esi
    push edi
    mov esi, %1
    mov edi, %2
    call write_debug
    pop esi
    pop edi
%endmacro

section .text

bootstrap_start:
;EAX has the GRUB2 magic/my bootloader magic
;EBX has the pointer to the tag start
call start_com_asm
debug BOOTSTRAP_LOADED, COM_INFO_ASM
push eax
push ebx
call asm_ldgdt32
call asm_ldpaging
jmp $

section .rodata

BOOTSTRAP_LOADED db "Bootstrap loaded", 0
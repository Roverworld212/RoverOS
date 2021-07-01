%include "./Bootstrap/base_header.inc"
[BITS 32]

section .text

%macro prep 0
    push read_multiboot.ntag
    push esi
%endmacro

%macro cleanup 0
    pop esi
    ret
%endmacro

parseVesa:
prep
debug READ_VBE, COM_INFO_ASM
mov edi, mstruct.vbe
add esi, 8
mqword
add esi, 8
add edi, 8
mqword
add esi, 8
add edi, 8
mdword
add esi, 4
mword
cleanup

[GLOBAL read_multiboot]

read_multiboot:
;Check magic
cmp eax, 0x36d76289
jne .err_end
debug READ_MULTIBOOT, COM_INFO_ASM
;Setup tag
mov esi, ebx
add esi, 8
jmp .rtag
;Read tags
.ntag: ;Next tag
mov dword edx, [esi+4]
add edx, 7
add esi, edx
and esi, ~7
.rtag: ;Read tag
mov dword edx, [esi]
;This was the last tag
cmp edx, 0
je .end
cmp edx, 8
je parseVesa
;No matching value found
jmp .ntag
;Invalid magic
.err_end:
debug INVALID_MAGIC, COM_ERROR_ASM
;Loop forever
.elp:
hlt
jmp .elp
;Cleanup and return
.end:
mov esi, mstruct
debug MULTIBOOT_DONE, COM_INFO_ASM
ret

section .rodata

INVALID_MAGIC db "Invalid multiboot2 magic, the OS will not continue", 0
READ_MULTIBOOT db "Reading multiboot tags", 0
MULTIBOOT_DONE db "Finished reading tags", 0
READ_VBE db "Reading VBE tag", 0

section .bss

;For only the data I want
mstruct:
.vbe:
resq 1 ;Phys address
resd 3 ;Pitch, width, height
resw 1 ;bpp, type
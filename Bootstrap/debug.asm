[BITS 32]
;For nasm debug text

COM_INFO_ASM db "asm[I]: ", 0
COM_WARN_ASM db "asm[W]: ", 0
COM_ERROR_ASM db "asm[E]: ", 0

[GLOBAL start_com_asm]
[GLOBAL write_debug]
[GLOBAL COM_INFO_ASM]
[GLOBAL COM_WARN_ASM]
[GLOBAL COM_ERROR_ASM]

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

COM1 equ 0x3F8

start_com_asm:
;Preserve registers
push eax
push edx
;CLI
mov dx, COM1+1
mov al, 0x0
out dx, al
;DLAB
mov dx, COM1+3
mov al, 0x80
out dx, al
;DIVISOR
mov dx, COM1
mov al, 0x3
out dx, al
;HBITS
mov dx, COM1+1
mov al, 0x0
out dx, al
;8BITS
mov dx, COM1+3
mov al, 0x3
out dx, al
;FIFO
mov dx, COM1+2
mov al, 0xC7
out dx, al
;STI
mov dx, COM1+4
mov al, 0x0B
out dx, al
;Cleanup
pop edx
pop eax
ret

is_empty:
mov dx, COM1+5
in ax, dx
and ax, 0x20
cmp ax, 0x0
jne is_empty
ret

write_byte_asm:
call is_empty
mov dx, COM1
mov byte al, [esi]
cmp al, 0x0
je .end
out dx, al
.end:
add esi, 1
ret

write_debug:
push ecx
push edx
;Write edi first
push esi
mov esi, edi
mov ecx, 0x1
.wbi:
.wbi_loop:
call write_byte_asm
cmp al, 0x0
je .wbi_end
jne .wbi_loop
.wbi_end:
;Check where to jmp to
cmp ecx, 0x1
je .c1
cmp ecx, 0x2
je .c2
;Nothing found
jmp .end
;Write non hex text
.c1:
pop esi
mov edi, 0x0
cmp eax, 0x0
je .nhx
.nhx:
;Causes wbi to jump to .end after writing text
mov ecx, 0x88
jmp .wbi
.hx:
;Write hex after current text
mov ecx, 0x2
jmp .wbi
;Write hex text
.c2:
.end:
;Write \n
mov dx, COM1
mov al, 0x0a
out dx, al
;Cleanup, the %macros will pop the rest
pop edx
pop ecx
ret
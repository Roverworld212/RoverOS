[EXTERN write_debug]
[EXTERN COM_INFO_ASM]
[EXTERN COM_WARN_ASM]
[EXTERN COM_ERROR_ASM]

;Debug macros

%macro debug_hex64 0-3
    push rsi
    push rdi
    push rax
    mov esi, %1
    mov edi, %2
    mov eax, %3
    call write_debug
    pop rax
    pop rdi
    pop rsi
%endmacro

%macro debug_hex 0-3
    push esi
    push edi
    push eax
    mov esi, %1
    mov edi, %2
    mov eax, %3
    call write_debug
    pop eax
    pop edi
    pop esi
%endmacro

%macro debug64 0-2
    push rsi
    push rdi
    push rax
    mov rax, 0x0
    mov esi, %1
    mov edi, %2
    mov eax, 0x0
    call write_debug
    pop rax
    pop rdi
    pop rsi
%endmacro

%macro debug 0-2
    push esi
    push edi
    push eax
    mov eax, 0x0
    mov esi, %1
    mov edi, %2
    call write_debug
    pop eax
    pop edi
    pop esi
%endmacro

;Memory manipulation macros
;Useful for reading Multiboot2 tags

;ESI = Read from
;EDI = Write to
;EDX = temp register

;For use in 32 bit mode
%macro mqword 0
    push edx
    mov dword edx, [esi]
    mov dword [edi], edx
    mov dword edx, [esi+4]
    mov dword [edi+4], edx
    pop edx
%endmacro

%macro mdword 0
    push edx
    mov dword edx, [esi]
    mov dword [edi], edx
    pop edx
%endmacro

%macro mword 0
    push edx
    mov word dx, [esi]
    mov word [edi], dx
    pop edx
%endmacro

%macro mbyte 0
    push edx
    mov byte dh, [esi]
    mov byte [edi], dh
    pop edx
%endmacro
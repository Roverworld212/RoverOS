[EXTERN write_debug]
[EXTERN COM_INFO_ASM]
[EXTERN COM_WARN_ASM]
[EXTERN COM_ERROR_ASM]

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
    pop esi
    pop edi
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
    pop esi
    pop edi
%endmacro
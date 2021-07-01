%include "./Bootstrap/base_header.inc"

[BITS 32]

section .text

clr_tbls:
mov edi, pageMap
xor eax, eax
mov ecx, 0x1000
rep stosd
mov edi, pageMap
ret

map_tbls:
mov dword [edi], pageDirPointer+3
add edi, 0x1000
mov dword [edi], pageDir+3
add edi, 0x1000
mov dword [edi], pageTable+3
add edi, 0x1000
mov ebx, 0x00000003
mov ecx, 1024
.map:
mov dword [edi], ebx
add ebx, 0x1000
add edi, 8
loop .map
ret

[GLOBAL asm_ldpaging]

asm_ldpaging:
push eax
push ebx
debug PAGING_LOADING, COM_INFO_ASM 
call clr_tbls
call map_tbls
debug PAGING_LOADED, COM_INFO_ASM
;Set CR3
mov edi, pageMap
mov cr3, edi
debug SET_CR3, COM_INFO_ASM
;Set PAE
mov eax, cr4
or eax, 1<<5
mov cr4, eax
debug SET_PAE, COM_INFO_ASM
;Set LME
mov ecx, 0xC0000080
rdmsr
or eax, 1<<8
wrmsr
debug SET_LME, COM_INFO_ASM
;Set CR0
mov eax, cr0
or eax, 1<<31
mov cr0, eax
debug SET_CR0, COM_INFO_ASM
pop ebx
pop eax
ret

section .bss

[GLOBAL pageMap]
[GLOBAL pageDirPointer]
[GLOBAL pageDir]
[GLOBAL pageTable]

paging:
align 0x1000
pageMap:
resq 512
align 0x1000
pageDirPointer:
resq 512
align 0x1000
pageDir:
resq 512
align 0x1000
pageTable:
resq 512

section .rodata

PAGING_LOADING db "Loading pml4e", 0
PAGING_LOADED db "Loaded pml4e", 0
SET_CR3 db "Set CR3", 0
SET_PAE db "Set PAE", 0
SET_LME db "Set LME", 0
SET_CR0 db "Set CR0", 0
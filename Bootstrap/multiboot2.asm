%include "./Bootstrap/base_header.inc"
[BITS 32]

section .text

%macro prep 0
    push read_multiboot.ntag
    push esi
    add esi, 8
    mov edx, 0x0
%endmacro

%macro cleanup 0
    pop esi
    ret
%endmacro

%macro forward 0-1
    add esi, %1
    add edi, %1
%endmacro

module:
prep
debug READ_MOD, COM_INFO_ASM
mov edi, mstruct.modules
mqword
cleanup

parseVesa:
prep
debug READ_FBUFF, COM_INFO_ASM
mov edi, mstruct.vesa
mqword
forward 8
mqword
forward 8
mdword
forward 4
mword
cleanup

parseLBA:
prep
debug READ_LBA, COM_INFO_ASM
mov edi, mstruct.lba
mdword
cleanup

parseSmbios:
prep
debug READ_SMB, COM_INFO_ASM
mov edi, mstruct.smbios
mword
forward 2
mqword
cleanup

parseEfi:
prep
debug READ_EFI, COM_INFO_ASM
mov edi, mstruct.efi32
mdword
cleanup

parseApm:
prep
debug READ_APM, COM_INFO_ASM
mov edi, mstruct.apm
mqword
forward 8
mqword
forward 8
mword
cleanup

parseNetwork:
prep
debug READ_NET, COM_INFO_ASM
mov edi, mstruct.network
mbyte
cleanup

parseAcpiOld:
prep
debug READ_ACPI_OLD, COM_INFO_ASM
mov edi, mstruct.acpi
add edi, 1
mov dword [edi], dword esi
cleanup

parseAcpiNew:
prep
debug READ_ACPI_NEW, COM_INFO_ASM
cleanup

parseBootdev:
prep
debug READ_BDEV, COM_INFO_ASM
cleanup

parseVBE:
prep
debug READ_VBE, COM_INFO_ASM
mov edi, mstruct.vbe
mdword
mdword
cleanup

parseMmap:
prep
debug READ_MMAP, COM_INFO_ASM
mov edi, mstruct.mmap
mqword
cleanup

parseEfi64:
prep
debug READ_EFI64, COM_INFO_ASM
mov edi, mstruct.efi64
mqword
cleanup

[GLOBAL read_multiboot]

read_multiboot:
;Check magic
cmp eax, 0x36d76289
jne .err_end
debug READ_MULTIBOOT, COM_INFO_ASM
;Setup tag
mov ecx, 0x0
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
mov dword ecx, [esi]
;This was the last tag
cmp ecx, 0
je .end
;VBE
cmp ecx, 7
je parseVBE
;MMAP
cmp ecx,6
je parseMmap
;FBUFFER
cmp ecx, 8
je parseVesa
;LBA
cmp ecx, 21
je parseLBA
;smbios
cmp ecx, 13
je parseSmbios
;efi
cmp ecx, 11
je parseEfi
;efi64
cmp ecx, 12
je parseEfi64
;apm
cmp ecx, 10
je parseApm
;network
cmp ecx, 16
je parseNetwork
;acpi old
cmp ecx, 14
je parseAcpiOld
;acpi new
cmp ecx, 15
je parseAcpiNew
;boot device
cmp ecx, 5
je parseBootdev
;Mods
cmp ecx, 3
je module
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
READ_FBUFF db "Reading Frame buffer", 0
READ_LBA db "Reading LBA", 0
READ_SMB db "Reading SMBIOS", 0
READ_EFI db "Reading EFI32", 0
READ_APM db "Reading APM", 0
READ_NET db "Reading NET", 0
READ_ACPI_OLD db "Reading ACPI", 0
READ_ACPI_NEW db "Reading ACPI 2", 0
READ_BDEV db "Reading BOOT DEVICE", 0
READ_MOD db "Reading MOD", 0
READ_MMAP db "Reading Mmap", 0
READ_EFI64 db "Reading EFI64", 0
READ_VBE db "Reading VBE", 0

section .bss

mcount:
resd 1

;For only the data I want
mstruct:
.vesa:
resq 1 ;Phys address
resd 3 ;Pitch, width, height
resw 1 ;bpp, type
.vbe:
resq 3
.lba:
resd 1 ;Base
.smbios:
resw 1 ;Major, Minor
resw 3 ;reserved
.mmap:
resd 3
.efi32:
resd 1 ;Pointer
.efi64:
resq 1
.apm:
resd 1 ;version, cseg
resd 1 ;offset
resd 1 ;cseg16, dseg
resd 1 ;flags cseglen
resw 1 ;dseglen
.network:
resb 1 ;dhc
.modules:
resq 20 ;10 mods max
.acpi:
resb 1 ;0 if old
resd 1 ;address
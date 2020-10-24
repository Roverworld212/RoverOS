[BITS 32]

[GLOBAL getCPUINF]
[EXTERN cpur]

getCPUINF: ;This will need to be redone later on
mov eax, [esp+4]
cpuid
mov eax, [esp+4]
push edx
push ecx
push ebx
push eax
call cpur
pop eax
pop ebx
pop ecx
pop edx
ret
[BITS 64]

[GLOBAL CPUIDint]
[GLOBAL hasCPUID]

[EXTERN cpuidRet]

hasCPUID:
;Need to add detection later
ret

CPUIDint:
push rax
push rbx
push rdx
push rcx
push rdi
mov rax, rdi
push rax
cpuid
push rax
push rdx
push rcx
lea rax, [rel cpuidRet]
jmp rax
pop rdi
pop rcx
pop rdx
pop rbx
pop rax
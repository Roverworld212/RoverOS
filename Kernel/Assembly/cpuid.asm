[BITS 64]

[GLOBAL cpuidInt]
[EXTERN cpuidRet]

cpuidInt:
mov rax, rdi
mov r8, rax
cpuid
mov rsi, rbx
call cpuidRet
ret
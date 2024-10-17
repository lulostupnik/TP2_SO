global canary_activator
section .text:

canary_activator:
    mov dword [rbp-0xFF0], 10
    jmp canary_activator

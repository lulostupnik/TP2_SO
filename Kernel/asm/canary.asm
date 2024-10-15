global canary_activator
section .text:

canary_activator:
    pop rax
    mov qword [rsp], 10
    jmp canary_activator

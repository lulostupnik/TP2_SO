section .text
global op_code

op_code:
    push rbp
    mov rbp, rsp

    jmp 0

    mov rsp, rbp
    pop rbp
    ret

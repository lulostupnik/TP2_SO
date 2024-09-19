section .text
global infinite_stack

infinite_stack2:
    push rdx            ; armado de stack frame usando rdx
    mov rdx, rsp

    push rbp            ; backup
    push rcx

    mov rbp, 0xBEBECAFE

    sub rsp, 16         ; es decir rsp es rdx - 32
    mov rcx, 0x00000000080000000
.loop:
    mov rax, 0x1234567890ABCDEF
    mov [rsp], rax
    dec rcx
    jnz .loop

    pop rcx             ; desbackup
    pop rbp

    mov rsp, rdx
    pop rdx

    ret



infinite_stack:
    push rdx            ; armado de stack frame usando rdx
    mov rdx, rsp

    push rbp            ; backup
    push rcx

    mov rbp, 0xBEBECAFE
    mov rsp, 0x00005ABA

    mov rcx, 0x00000000080000000
.loop:
    dec rcx
    jnz .loop

    pop rcx             ; desbackup
    pop rbp

    mov rsp, rdx
    pop rdx

    ret



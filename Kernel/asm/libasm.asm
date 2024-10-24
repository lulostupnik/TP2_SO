GLOBAL cpu_vendor
GLOBAL get_key
GLOBAL inb
GLOBAL outb
GLOBAL rtc
GLOBAL acquire
GLOBAL release


section .text

acquire:
    mov al, 0
.retry:
    xchg byte [rdi], al
    test al, al
    jnz .retry
    ret

release:
    mov byte [rdi], 1
    ret



cpu_vendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

;https://wiki.osdev.org/%228042%22_PS/2_Controller
; uint8_t get_key();
get_key:
    push rbp
    mov rbp, rsp

    in al, 60h

    leave
    ret

rtc:
    push rbp
    mov rbp, rsp

    mov al, dil
    out 70h, al
    in al, 71h

    mov rsp, rbp
    pop rbp
    ret


inb:
	push rbp
    mov rbp, rsp

	mov rdx, rdi
	in al, dx

	mov rsp, rbp
	pop rbp
	ret
outb:
	push rbp
    mov rbp, rsp

 	mov rax, rsi
	mov rdx, rdi
	out dx, al

	mov rsp, rbp
	pop rbp
	ret



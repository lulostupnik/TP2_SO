
GLOBAL _cli
GLOBAL _sti
GLOBAL pic_master_mask
GLOBAL pic_slave_mask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL timer_tick

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL _exception13Handler

GLOBAL regs_shot
GLOBAL exception_regs
GLOBAL regs_shot_available

EXTERN irq_dispatcher
EXTERN exception_dispatcher
EXTERN sys_call_handler
EXTERN should_take_reg_shot
EXTERN get_stack_base
EXTERN scheduler

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro pushStateMinusRax 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popStateMinusRax 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 
	call irq_dispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

timer_tick:
    int 0x20
    ret

_hlt:
	sti
	hlt
	ret

haltcpu:
	cli
	hlt
	ret


_cli:
	cli
	ret


_sti:
	sti
	ret

pic_master_mask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn


pic_slave_mask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  	; ax = 16-bit mask
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	pushState

	mov rdi, 0 
	call irq_dispatcher

    mov rdi, rsp
    call scheduler
    mov rsp, rax

	
	mov al, 20h
	out 20h, al

	popState
	iretq

;Keyboard
_irq01Handler:
    pushState

	mov rdi, 1
	call irq_dispatcher

	call should_take_reg_shot
	cmp rax, 1

	jne .keyboard_end
	popState
	pushState
    mov [regs_shot + 8 * 0 ], rax
    mov [regs_shot + 8 * 1 ], rbx
    mov [regs_shot + 8 * 2 ], rcx
    mov [regs_shot + 8 * 3 ], rdx
    mov [regs_shot + 8 * 4 ], rsi
    mov [regs_shot + 8 * 5 ], rdi
    mov [regs_shot + 8 * 6 ], rbp

    mov rax, [rsp + 18 * 8]


    mov [regs_shot + 8 * 7 ], rax          

    mov [regs_shot + 8 * 8 ], r8
    mov [regs_shot + 8 * 9 ], r9
    mov [regs_shot + 8 * 10], r10
   	mov [regs_shot + 8 * 11], r11
   	mov [regs_shot + 8 * 12], r12
   	mov [regs_shot + 8 * 13], r13
   	mov [regs_shot + 8 * 14], r14
   	mov [regs_shot + 8 * 15], r15
   	mov rax, [rsp+15*8]    
   	mov [regs_shot + 8 * 16], rax

   	mov rax, 1
    mov [regs_shot_available], rax          

.keyboard_end:
	mov al, 20h
	out 20h, al

	popState
	iretq

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;SYSCALL
_irq80Handler:
	pushState
	mov rdi, rsp ; Pasaje de Registros

	call sys_call_handler
	


	popStateMinusRax
	add rsp, 8 ; Restore the stack pointer

	iretq

;/////////////////////////////////EXCEPTIONS//////////////////////////////////////////////////////////////////


%macro exceptionHandler 1
    cli
	pushState
	mov [exception_regs + 8*0 ], rax
	mov [exception_regs + 8*1 ], rbx
	mov [exception_regs + 8*2 ], rcx
	mov [exception_regs + 8*3 ], rdx
	mov [exception_regs + 8*4 ], rsi
	mov [exception_regs + 8*5 ], rdi
	mov [exception_regs + 8*6 ], rbp
    mov rax, [rsp + 18 * 8]
	mov [exception_regs + 8*7 ], rax	
	mov [exception_regs + 8*8 ], r8
	mov [exception_regs + 8*9 ], r9
	mov [exception_regs + 8*10], r10
	mov [exception_regs + 8*11], r11
	mov [exception_regs + 8*12], r12
	mov [exception_regs + 8*13], r13
	mov [exception_regs + 8*14], r14
	mov [exception_regs + 8*15], r15
	mov rax, [rsp+15*8]                     ;RIP of the previous context
	mov [exception_regs + 8*16], rax
	mov rax, [rsp+17*8]                     ; RFLAGS
	mov [exception_regs + 8*17], rax

	mov rdi, %1                             ; Parameters for exception_dispatcher
	mov rsi, exception_regs

	call exception_dispatcher

	popState
    call get_stack_base
	mov [rsp+24], rax 						; StackBase
    mov rax, userland
    mov [rsp], rax 							; OVERWRITE the return address

    sti
    iretq
%endmacro

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid operation code exception
_exception6Handler:
	exceptionHandler 6

_exception13Handler:
	exceptionHandler 13





SECTION .bss
	aux resq 1


SECTION .data
    regs_shot dq 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 			; 17 zeros
    regs_shot_available dq 0 												; flag to check if a regs_shot is available
    exception_regs dq 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 	; 18 zeros


SECTION .rodata
userland equ 0x400000

GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL regs_shot
GLOBAL exception_regs
GLOBAL regs_shot_available

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN sysCallHandler
EXTERN should_take_reg_shot
EXTERN getStackBase

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

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn


picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
    pushState

	mov rdi, 1
	call irqDispatcher

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

    ;Descomentar para ver que hay en la dir apuntada por RSP:
    ;mov rbx, [rsp + 18* 8]
    ;mov rax, [rbx]

    mov [regs_shot + 8 * 7 ], rax            ;rsp

    mov [regs_shot + 8 * 8 ], r8
    mov [regs_shot + 8 * 9 ], r9
    mov [regs_shot + 8 * 10], r10
   	mov [regs_shot + 8 * 11], r11
   	mov [regs_shot + 8 * 12], r12
   	mov [regs_shot + 8 * 13], r13
   	mov [regs_shot + 8 * 14], r14
   	mov [regs_shot + 8 * 15], r15
   	mov rax, [rsp+15*8]    ; posicion en el stack de la dir. de retorno (valor del rip previo al llamado de la interrupcion)
   	mov [regs_shot + 8 * 16], rax

   	mov rax, 1
    mov [regs_shot_available], rax          ; tenemos un snapshot de los registros

.keyboard_end:
	; signal pic EOI (End of Interrupt)
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

	call sysCallHandler
	


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
	; mov rax, rsp
    ; add rax, 16 * 8                     ; RSP del contexto anterior
    mov rax, [rsp + 18 * 8]
	mov [exception_regs + 8*7 ], rax	;
	mov [exception_regs + 8*8 ], r8
	mov [exception_regs + 8*9 ], r9
	mov [exception_regs + 8*10], r10
	mov [exception_regs + 8*11], r11
	mov [exception_regs + 8*12], r12
	mov [exception_regs + 8*13], r13
	mov [exception_regs + 8*14], r14
	mov [exception_regs + 8*15], r15
	mov rax, [rsp+15*8]                     ;RIP del contexto anterior
	mov [exception_regs + 8*16], rax
	mov rax, [rsp+17*8]                     ; RFLAGS
	mov [exception_regs + 8*17], rax

	mov rdi, %1                             ; Parametros para exceptionDispatcher
	mov rsi, exception_regs

	call exceptionDispatcher

	popState
    call getStackBase
	mov [rsp+24], rax ; El StackBase
    mov rax, userland
    mov [rsp], rax ; PISO la dirección de retorno

    sti
    iretq
%endmacro

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid operation code exception
_exception6Handler:
	exceptionHandler 6


haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1


SECTION .data
    regs_shot dq 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ; 17 zeros
    regs_shot_available dq 0 ; flag para saber si hay un regs_shot disponible
    exception_regs dq 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ; 18 zeros
	; %define REGS_AMOUNT 17

SECTION .rodata
userland equ 0x400000
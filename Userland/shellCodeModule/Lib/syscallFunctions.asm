GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_get_register_snapshot
GLOBAL sys_beep
GLOBAL sys_set_font_size
GLOBAL sys_clear_screen
GLOBAL sys_put_pixel
GLOBAL sys_put_rectangle
GLOBAL sys_draw_letter
GLOBAL sys_set_mode
GLOBAL sys_get_screen_information
GLOBAL sys_ticks_sleep
GLOBAL sys_get_time
GLOBAL sys_free
GLOBAL sys_malloc
GLOBAL sys_get_pid
GLOBAL sys_create_process
GLOBAL sys_block
GLOBAL sys_yield
GLOBAL sys_unblock
GLOBAL sys_nice
GLOBAL sys_kill
GLOBAL sys_wait
GLOBAL sys_sem_open
GLOBAL sys_sem_wait
GLOBAL sys_sem_post
GLOBAL sys_sem_close
GLOBAL sys_ps
GLOBAL sys_free_ps
GLOBAL sys_get_status
GLOBAL sys_pipe_open
GLOBAL sys_pipe_open_free
GLOBAL sys_pipe_read
GLOBAL sys_pipe_write
GLOBAL sys_pipe_close
GLOBAL sys_pipe_reserve
GLOBAL sys_sem_open_get_id
GLOBAL sys_get_my_fds
GLOBAL sys_mem_info

section .text


%macro pushState 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
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
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro


%macro pushStateMinusRax 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi

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
    pop rdi
    pop rsi
%endmacro


%macro sys_interrupt 1
        mov rax, %1
        int 80h
%endmacro


%macro simple_sys_handler 1
    push rbp
    mov rbp, rsp
    mov rax, %1
    int 80h
    mov rsp, rbp
    pop rbp
    ret
%endmacro


sys_read:
    simple_sys_handler 0

sys_write:
    simple_sys_handler 1

sys_get_register_snapshot:
    simple_sys_handler 2

sys_beep:
    simple_sys_handler 3

sys_set_font_size:
    simple_sys_handler 4

sys_clear_screen:
    simple_sys_handler 5

sys_put_pixel:
    simple_sys_handler 6

sys_put_rectangle:
    simple_sys_handler 7

sys_draw_letter:
    simple_sys_handler 8

sys_set_mode:
    simple_sys_handler 9

sys_get_screen_information:
    simple_sys_handler 10

sys_ticks_sleep:
    simple_sys_handler 11

sys_get_time:
    simple_sys_handler 12

sys_malloc:
    simple_sys_handler 13

sys_free:
    simple_sys_handler 14

sys_get_pid:
    simple_sys_handler 15

sys_create_process:
    simple_sys_handler 16

sys_block:
    simple_sys_handler 17

sys_unblock:
    simple_sys_handler 18

sys_yield:
    simple_sys_handler 19

sys_nice:
    simple_sys_handler 20

sys_kill:
    simple_sys_handler 21

sys_wait:
    simple_sys_handler 22

;sys_ps:
;    simple_sys_handler 21

sys_sem_open:
    simple_sys_handler 23

sys_sem_wait:
    simple_sys_handler 24

sys_sem_post:
    simple_sys_handler 25

sys_sem_close:
    simple_sys_handler 26

sys_ps:
    simple_sys_handler 27

sys_free_ps:
    simple_sys_handler 28

sys_get_status:
    simple_sys_handler 29

sys_pipe_open:
    simple_sys_handler 30

sys_pipe_open_free:
    simple_sys_handler 31

sys_pipe_read:
    simple_sys_handler 32

sys_pipe_write:
    simple_sys_handler 33

sys_pipe_close:
    simple_sys_handler 34

sys_pipe_reserve:
    simple_sys_handler 35

sys_sem_open_get_id:
    simple_sys_handler 36

sys_get_my_fds:
    simple_sys_handler 37

sys_mem_info:
    simple_sys_handler 38
#include <stdint.h>
#include <video.h>
#include <keyboard.h>
#include <syscalls.h>
#include <speaker.h>
#include <time.h>
#include <rtc.h>

typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} Registers;

extern uint64_t regs_shot[17];
extern uint64_t regs_shot_available;


int64_t sysCallHandler(Registers * regs) {
    switch(regs->rax){
        case 0: return sys_read(regs->rdi, (uint16_t *) regs->rsi, regs->rdx); break;
        case 1: return sys_write(regs->rdi, (char *) regs->rsi, regs->rdx); break;
        case 2: return sys_get_register_snapshot((Snapshot *) regs->rdi); break;
        case 3: return sys_beep(regs->rdi, regs->rsi); break;
        case 4: return sys_set_font_size(regs->rdi); break;
        case 5: return sys_clear_screen(); break;
        case 6: return sys_put_pixel(regs->rdi, regs->rsi, (Color *) regs->rdx); break;
        case 7: return sys_put_rectangle(regs->rdi, regs->rsi, regs->rdx, regs->rcx, (Color *) regs->r8); break;
        case 8: return sys_draw_letter(regs->rdi, regs->rsi, (char *) regs->rdx, (Color *) regs->rcx, regs->r8); break;
        case 9: return sys_set_mode(regs->rdi); break;
        case 10: return sys_get_screen_information((ScreenInformation *) regs->rdi); break;
        case 11: return sys_nano_sleep(regs->rdi); break;
        case 12: return sys_get_time((time_struct*)regs->rdi);break;
        default: return NOT_VALID_SYS_ID;

    }
}


int64_t sys_read(uint64_t fd, uint16_t * buffer, uint64_t amount){
    uint64_t i = 0;

    while(i < amount && bufferHasNext()){
        buffer[i] = getCurrent();
        i++;
    }
    return i;
}


//Modo texto:
int64_t sys_write(uint64_t fd, const char * buffer, uint64_t amount){
    return vdriver_text_write(fd, buffer, amount);
}

//Modo texto:
int64_t sys_set_font_size(uint64_t size){
    return vdriver_text_set_font_size(size);
}

int64_t sys_beep(uint32_t freq, uint32_t time){
    beep(freq, time);
    return 0;
}

//Modo video:
int64_t sys_put_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, Color * color){
    return vdriver_video_draw_rectangle(x, y, width, height, *color);
}

//Modo video:
int64_t sys_draw_letter(uint64_t x, uint64_t y, char * letter, Color * color, uint64_t fontSize){
    return vdriver_video_draw_font(x, y, *letter, *color, fontSize);
}

//Modo video:
int64_t sys_put_pixel(uint64_t x, uint64_t y, Color * color){
    return vdriver_video_draw_pixel(x, y, *color);
}

int64_t sys_get_screen_information(ScreenInformation * screen_information){
    return vdriver_get_screen_information(screen_information);
}

int64_t sys_set_mode(uint64_t mode){
    return vdriver_set_mode(mode, (Color) {0,0,0});
}


int64_t sys_get_register_snapshot(Snapshot * snapshot){

    if(!regs_shot_available) {
        return -1;
    }

    snapshot->rax = regs_shot[0];
    snapshot->rbx = regs_shot[1];
    snapshot->rcx = regs_shot[2];
    snapshot->rdx = regs_shot[3];
    snapshot->rsi = regs_shot[4];
    snapshot->rdi = regs_shot[5];
    snapshot->rbp = regs_shot[6];
    snapshot->rsp = regs_shot[7];
    snapshot->r8 = regs_shot[8];
    snapshot->r9 = regs_shot[9];
    snapshot->r10 = regs_shot[10];
    snapshot->r11 = regs_shot[11];
    snapshot->r12 = regs_shot[12];
    snapshot->r13 = regs_shot[13];
    snapshot->r14 = regs_shot[14];
    snapshot->r15 = regs_shot[15];
    snapshot->rip = regs_shot[16];
    return 0;
}


int64_t sys_clear_screen(){
    return vdriver_clear_screen((Color) {0,0,0});
}

int64_t sys_nano_sleep(uint32_t ns){
    nano_sleep(ns);
    return 0;
}



int64_t sys_get_time(time_struct * time){
    time->seconds = getRTCSeconds();
    time->minutes =  getRTCMinutes();
    time->hour =  getRTCHours();
    time->day = getRTCDayOfMonth();
    time->month = getRTCMonth();
    time->year = getRTCYear();
    return 0;
}

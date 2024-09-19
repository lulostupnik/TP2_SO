#ifndef SYSCALLFUNCTIONS_H
#define SYSCALLFUNCTIONS_H
#include <stdint.h>


/**
 * Structure representing all the register snapshot
 */
typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
} Snapshot;

/**
 * Structure representing a color
 */
typedef struct {
    uint8_t r; // Red component
    uint8_t g; // Green component
    uint8_t b; // Blue component
} Color;


/**
 * Structure representing the dimensions of the screen.
 */
typedef struct{
    int64_t width;
    int64_t height;
} ScreenInformation;

/**
 * Structure representing current time
 */
typedef struct time {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;
} time_struct;


int64_t sys_read(uint64_t fd, uint16_t * buffer, uint64_t amount);


int64_t sys_write(uint64_t fd, const char * buffer, uint64_t amount);


int64_t sys_get_register_snapshot(Snapshot * snapshot);


int64_t sys_beep(uint32_t frequency, uint32_t duration);


int64_t sys_set_font_size(uint64_t size);


int64_t sys_clear_screen(void);


int64_t sys_put_pixel(uint64_t x, uint64_t y, Color * color);


int64_t sys_put_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, Color * color);


int64_t sys_draw_letter(uint64_t x, uint64_t y, char * letter, Color * color, uint64_t fontSize);


int64_t sys_set_mode(uint64_t mode);


int64_t sys_get_screen_information(ScreenInformation * screen_information);


int64_t sys_nano_sleep(uint32_t ticks);


int64_t sys_get_time(time_struct * time);


#endif 
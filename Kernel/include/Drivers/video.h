#include <stdint.h>
#include <font.h>
#include <driversInformation.h>
#include <shared_defs.h>
#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H




/**
 * Structure representing the VBE mode information.
 */
struct vbe_mode_info_structure {
	uint16_t attributes;	// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;		// deprecated
	uint8_t window_b;		// deprecated
	uint16_t granularity;	// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;	// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;		// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		    // physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ( ( packed ) );






typedef struct vbe_mode_info_structure * vbe_info_ptr;




int64_t vdriver_text_write ( uint64_t fd, char * buffer, int64_t amount );
int64_t vdriver_text_set_font_size ( uint64_t size );
void vdriver_set_font_color ( color c );

int64_t vdriver_video_draw_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color color );
int64_t vdriver_video_draw_font ( uint64_t x, uint64_t y, uint8_t ascii, color color, uint64_t fontSize );
int64_t vdriver_video_draw_pixel ( uint64_t x, uint64_t y, color color );


int64_t vdriver_get_screen_information ( screen_information * screen_information );
int64_t vdriver_set_mode ( uint64_t mode, color c );
int64_t vdriver_clear_screen ( color color );







#endif // VIDEODRIVER_H
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <video.h>


#define SCREEN_WIDTH vbe_mode_info->width
#define SCREEN_HEIGHT vbe_mode_info->height

#define CHAR_BUFFER_ROWS  48         // Numero de caracteres disponibles con la fuente en 1 (en x).
#define CHAR_BUFFER_COLS  128        // Numero de caracteres disponibles con la fuente en 1 (en y).
#define Y_FONT_OFFSET (FONT_HEIGHT * font_size)
#define X_FONT_OFFSET (FONT_WIDTH * font_size)

typedef struct {
	uint64_t x;
	uint64_t y;
} point;

typedef struct {
	uint8_t c;
	uint8_t fd;
} char_buffer_type;

static vbe_info_ptr vbe_mode_info = ( vbe_info_ptr ) 0x0000000000005c00;
extern uint8_t font_bitmap[4096];
static color background_color = {0, 0, 0};
static color font_color = {255, 255, 255};
static color stderr_color = {255, 0, 0};
static uint64_t font_size = 1;
static uint8_t driver_mode = TEXT_MODE;
static point current_screen_point = {0, 0};
static char_buffer_type char_buffer[CHAR_BUFFER_ROWS * CHAR_BUFFER_COLS];
static uint64_t buffer_index = 0;

static uint8_t override_mode = 0;
static uint64_t CHAR_BUFFER_ROWS_zoomed = CHAR_BUFFER_ROWS;
static uint64_t CHAR_BUFFER_COLS_zoomed = CHAR_BUFFER_COLS;



static void libc_clear_screen();
static uint64_t in_text_mode();
static uint64_t in_video_mode();
static void add_char_to_buffer ( uint8_t c, uint8_t fd );
static void print_font ( char_buffer_type letter );
static void libc_clear_screen();
static void new_line_print();
static void new_line_buff();
static void new_line();
static void back_space_buffer();
static void back_space();
static void back_space_print();
static void tabulator();
static void re_buffer_print();
static void print_buffer();


void vdriver_set_font_color ( color c )
{
	font_color = c;
}

int64_t vdriver_text_set_font_size ( uint64_t size )
{
	if ( !in_text_mode() ) {
		return -1;
	}

	if ( SCREEN_WIDTH < size * FONT_WIDTH || SCREEN_HEIGHT < size * FONT_HEIGHT ) {
		return -1;
	}
	if ( size == font_size ) {
		return 0;
	}
	font_size = size;
	CHAR_BUFFER_ROWS_zoomed = ( SCREEN_HEIGHT / ( font_size * FONT_HEIGHT ) );
	CHAR_BUFFER_COLS_zoomed = ( SCREEN_WIDTH / ( font_size * FONT_WIDTH ) );
	libc_clear_screen();
	return 0;
}


int64_t vdriver_get_screen_information ( screen_information * screen_information )
{
	screen_information->width = SCREEN_WIDTH;
	screen_information->height = SCREEN_HEIGHT;
	return 1;
}

int64_t vdriver_clear_screen ( color color )
{
	if ( driver_mode == TEXT_MODE ) {
		background_color = color;
		current_screen_point.x = current_screen_point.y = 0;
		buffer_index = 0;
	}
	override_mode = 1;
	vdriver_video_draw_rectangle ( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color );
	override_mode = 0;
	return 0;
}



int64_t vdriver_set_mode ( uint64_t mode, color c )
{
	if ( ! ( mode == TEXT_MODE || mode == VIDEO_MODE ) ) {
		return -1;
	}
	background_color = c;
	if ( mode == driver_mode ) {
		return 0;
	}
	driver_mode = mode;


	if ( mode == TEXT_MODE ) {
		print_buffer();
	} else {
		vdriver_clear_screen ( c );
	}

	return 0;

}


int64_t vdriver_text_write ( uint64_t fd, const char * buffer, int64_t amount )
{
	if ( !in_text_mode() ) {
		return 0;
	}
	if ( ! ( fd == STDOUT || fd == STDERR ) ) {
		return 0;
	}

	uint64_t i = 0;
	while ( i < amount && buffer[i] != 0 ) {
		switch ( buffer[i] ) {
			case '\n':
				new_line();
				break;
			case '\b':
				back_space();
				break;
			case '\t':
				tabulator();
				break;
			default:
				if ( buffer[i] >= FIRST_ASCII_FONT && buffer[i] <= LAST_ASCII_FONT ) {
					char_buffer_type c = {buffer[i], fd};
					add_char_to_buffer ( buffer[i], fd );
					print_font ( c );
				}
				break;
		}
		i++;
	}
	return i;
}

int64_t vdriver_video_draw_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color color )
{
	if ( x + width > SCREEN_WIDTH || y + height > SCREEN_HEIGHT ) {
		return -1;
	}
	if ( !in_video_mode() ) {
		return -1;
	}
	for ( uint64_t i = 0; i < width; i++ ) {
		for ( uint64_t j = 0; j < height; j++ ) {
			vdriver_video_draw_pixel ( x + i, y + j, color );
		}
	}
	return 0;
}
int64_t vdriver_video_draw_font ( uint64_t x, uint64_t y, uint8_t ascii, color color, uint64_t font_size )
{
	if ( ascii < FIRST_ASCII_FONT || ascii > LAST_ASCII_FONT ) {
		return -1;
	}
	if ( !in_video_mode() ) {
		return -1;
	}
	int letter = ( ascii - ' ' ) * 16;
	for ( uint64_t i = 0; i < 16; i++ ) {
		for ( uint64_t j = 0; j < 8; j++ ) {
			if ( ( font_bitmap[letter + i] >> ( 7 - j ) ) & 0x1 ) {
				vdriver_video_draw_rectangle ( ( x + j * font_size ), ( y + i * font_size ), font_size, font_size, color );
			}
		}
	}
	return 0;
}





int64_t vdriver_video_draw_pixel ( uint64_t x, uint64_t y, color color )
{

	if ( x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT ) {
		return -1;
	}
	if ( !in_video_mode() ) {
		return -1;
	}

	uint8_t * framebuffer = ( uint8_t * ) vbe_mode_info->framebuffer;
	uint64_t offset = ( x * ( ( vbe_mode_info->bpp ) / 8 ) ) + ( y * vbe_mode_info->pitch );
	framebuffer[offset]     =  color.b;
	framebuffer[offset + 1]   =  color.g;
	framebuffer[offset + 2]   =  color.r;
	return 0;
}


//funciones static:
static uint64_t in_text_mode()
{
	return ( ( driver_mode == TEXT_MODE ) || override_mode );
}
static uint64_t in_video_mode()
{
	return ( ( driver_mode == VIDEO_MODE ) || override_mode );
}

#define space_per_tab 3
static void tabulator()
{
	char_buffer_type c = {' ', STDOUT};
	for ( int i = 0; i < space_per_tab ; i++ ) {
		add_char_to_buffer ( ' ', STDOUT );
		print_font ( c );
	}
}

static void add_char_to_buffer ( uint8_t c, uint8_t fd )
{
	if ( buffer_index  >= CHAR_BUFFER_ROWS_zoomed * CHAR_BUFFER_COLS_zoomed ) {
		re_buffer_print();
	}
	char_buffer_type aux = {c, fd};
	char_buffer[buffer_index] = aux;
	buffer_index ++;

}



static void print_font ( char_buffer_type letter )
{

	if ( current_screen_point.x + FONT_WIDTH * font_size - FONT_WIDTH >= SCREEN_WIDTH ) {
		current_screen_point.y += FONT_HEIGHT * font_size;
		current_screen_point.x = 0;
	}
	if ( current_screen_point.y + FONT_HEIGHT * font_size - FONT_HEIGHT >= SCREEN_HEIGHT ) {
		re_buffer_print();
	}
	override_mode = 1;
	color col;
	if ( letter.fd == STDOUT ) {
		col = font_color;
	} else {
		col = stderr_color;
	}
	vdriver_video_draw_font ( current_screen_point.x, current_screen_point.y, letter.c, col, font_size );
	override_mode = 0;
	current_screen_point.x += FONT_WIDTH * font_size;
}

static void libc_clear_screen()
{
	vdriver_clear_screen ( background_color );
}

static void new_line_print()
{
	current_screen_point.x = 0;
	current_screen_point.y = current_screen_point.y + FONT_HEIGHT * font_size;
	if ( current_screen_point.y + FONT_HEIGHT * font_size - FONT_HEIGHT >= SCREEN_HEIGHT ) {
		re_buffer_print();
	}
}

static void new_line_buff()
{
	int remaining_on_row = CHAR_BUFFER_COLS_zoomed - ( buffer_index % CHAR_BUFFER_COLS_zoomed );
	for ( int i = 0 ; i < remaining_on_row; i++ ) {
		add_char_to_buffer ( ' ', STDOUT );
	}
}


static void new_line()
{
	new_line_buff();
	new_line_print();
}

static void back_space_print()
{
	if ( current_screen_point.x == 0 && current_screen_point.y == 0 ) {
		return;
	}
	if ( current_screen_point.x == 0 ) {

		current_screen_point.y -= font_size * FONT_HEIGHT; // y es dif a 0
		current_screen_point.x = SCREEN_WIDTH - font_size * FONT_WIDTH;
	} else {
		current_screen_point.x -= font_size * FONT_WIDTH;
	}
	override_mode = 1;
	vdriver_video_draw_rectangle ( current_screen_point.x, current_screen_point.y, X_FONT_OFFSET, FONT_HEIGHT * font_size, background_color );
	override_mode = 0;
}


static void back_space_buffer()
{
	if ( buffer_index != 0 ) {
		char_buffer[buffer_index].c = 0;
		char_buffer[--buffer_index].fd = STDOUT;
	}

}

static void back_space()
{
	back_space_print();
	back_space_buffer();
}



#define rows_to_rebuffer(rows_in_screen) (((rows_in_screen)/(2))+1)     // hace que el re_buffer me imprima la ultima mitad de la pantalla
static void re_buffer_print()
{
	uint64_t aux = buffer_index; // con el clear screen se setea en 0
	libc_clear_screen();
	uint64_t j = 0;
	for ( uint64_t i = CHAR_BUFFER_COLS_zoomed * rows_to_rebuffer ( CHAR_BUFFER_ROWS_zoomed ) ; i < aux; i++, j++ ) {
		char_buffer[j] = char_buffer[i];
		print_font ( char_buffer[j] );
	}
	buffer_index = j;
}

static void print_buffer()
{
	uint64_t aux = buffer_index; // con clear screen se borra
	libc_clear_screen();
	for ( int i = 0 ; i < aux ; i++ ) {
		vdriver_text_write ( char_buffer[i].fd, ( char * ) &char_buffer[i].c, 1 );
	}
}
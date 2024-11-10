// this is a personal academic project. dear pvs-studio, please check it.
// pvs-studio static code analyzer for c, c++ and c#: http://www.viva64.com

#include <keyboard.h>
#include <libasm.h>


/*
 * buffer --> es "circular". si se llena, pisa lo que primero se puso.
 */
#define cant_function_keys 12
#define special_key_pressed_map_idx(code) ((code) -FIRST_SPECIAL_KEY)


static uint8_t buffer[BUFFER_SIZE];
static uint64_t buffer_dim = 0;
static uint64_t buffer_current = 0;
static uint8_t reg_shot_flag = 0;
extern uint16_t pressed_key_shift_map[][2];
static void f1key ( void );
static function_key function_key_fun_array[cant_function_keys] = {f1key, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static uint16_t special_key_pressed_map[SPECIAL_KEYS_CANT] = {0};
static PCB * blocked;

static void f1key ( void )
{
	reg_shot_flag = 1;
}

void set_keyboard_blocked_null(){
	blocked = NULL;
}
PCB * get_keyboard_blocked(){
	return blocked;
}

int64_t stdin_read (uint8_t * buff, uint64_t amount )
{
	if(blocked != NULL){	// un proceso ya esta esperando...
		return -1;
	}

	uint64_t i = 0;
	
	if(!buffer_has_next()){
		blocked = get_running();
		block_current();
	}
	
	//if not buffer_has_next block. 
	while ( i < amount && buffer_has_next() && buffer[buffer_current] != EOF) {
		buff[i] = get_current();
		i++;
	}

	if(buffer[buffer_current] == EOF){
		get_current();
	}
	return i;
}


void set_f_key_function ( uint64_t key_number, function_key f )
{
	if ( key_number == 0 || key_number > cant_function_keys ) {
		return;
	}
	key_number--;
	function_key_fun_array[key_number] = f;
}

static void function_key_handler ( uint64_t code )
{
	int64_t i = -1;
	switch ( code ) {
		case F1:
			i = 0;
			break;
		case F2:
			i = 1;
			break;
		case F3:
			i = 2;
			break;
		case F4:
			i = 3;
			break;
		case F5:
			i = 4;
			break;
		case F6:
			i = 5;
			break;
		case F7:
			i = 6;
			break;
		case F8:
			i = 7;
			break;
		case F9:
			i = 8;
			break;
		case F10:
			i = 9;
			break;
		case F11:
			i = 10;
			break;
		case F12:
			i = 11;
			break;
	}
	if ( i != -1 && function_key_fun_array[i] != 0 ) {
		function_key_fun_array[i]();
	}

}


static uint8_t is_released ( uint8_t key )
{
	return ( key & 0x80 );
}
static uint8_t is_pressed ( uint8_t key )
{
	return !is_released ( key );
}

static int is_special_key ( uint16_t code )
{
	return ( code >= FIRST_SPECIAL_KEY ) && ( code <= LAST_SPECIAL_KEY );
}

static int special_key_pressed ( uint16_t code )
{
	if ( !is_special_key ( code ) ) {
		return -1;
	}
	return special_key_pressed_map[special_key_pressed_map_idx ( code )];
}


static int caps_lock_pressed()
{
	return special_key_pressed ( CAPS_LOCK );
}
static int shift_pressed()
{
	return ( special_key_pressed ( LEFT_SHIFT ) || special_key_pressed ( RIGHT_SHIFT ) ) ? 1 : 0;
}
static int shift_caps_lock_pressed()
{
	return ( shift_pressed() ^caps_lock_pressed() ); //xor
}

static uint8_t released_key_to_pressed_mask ( uint8_t key )
{
	return key & 0x7f;
}



uint64_t buffer_has_next()
{
	return ( buffer_dim > 0 ) && ( buffer_current < buffer_dim );
}

uint64_t get_current()
{
	if ( buffer_has_next() ) {
		return buffer[buffer_current++];
	}
	return 0;
}

void keyboard_handler()
{
	reg_shot_flag = 0;
	uint8_t key = get_key();


	uint8_t key_is_pressed = is_pressed ( key ) ? 1 : 0;

	if ( !key_is_pressed ) {
		key = released_key_to_pressed_mask ( key ); //la tabla es para pressed !
	}

	uint16_t code = pressed_key_shift_map[key][shift_caps_lock_pressed()];


	if ( is_special_key ( code ) ) {
		if ( code != CAPS_LOCK && code != NUM_LOCK && code != SCROLL_LOCK ) {
			special_key_pressed_map[special_key_pressed_map_idx ( code )] = key_is_pressed;
		} else if ( key_is_pressed ) {
			special_key_pressed_map[special_key_pressed_map_idx ( code )] = 1 - special_key_pressed_map[special_key_pressed_map_idx ( code )];
		}
	}

	if ( !key_is_pressed ) {
		return;
	}

	function_key_handler ( code );
	
	if(!is_special_key(code) && special_key_pressed(LEFT_CONTROL)){
		if(code == 'D' || code == 'd'){
			code = EOF;
		}else if(code == 'c' || code == 'C'){
			ctrl_c_handler();
			return;
		}
	}
	if(is_special_key(code) || code > LAST_ASCII){
		return;
	}

	buffer[buffer_dim] = code;
	if ( buffer_dim < BUFFER_SIZE ) {
		buffer_dim++;
	} else if (buffer_current == buffer_dim){
		buffer[0] = code;
		buffer_dim = 1;
		buffer_current = 0;
	}else{
		return;
	}
	if(blocked != NULL){
		ready(blocked);
		blocked = NULL;
	}

}

uint8_t should_take_reg_shot()
{
	return reg_shot_flag;
}
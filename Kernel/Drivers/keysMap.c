// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <keyboard.h>

//Tabla de SCANCODES:
//https://stanislavs.org/helppc/make_codes.html

const uint16_t pressedKeyShiftMap[][2] = {
        {00, 00},               // El scanCode empieza en 0x01, lo pongo para poder entrar directamente en lo que me dan.
        {0x1B, 0x1B},           // Escape key
        {'1', '!'},
        {'2', '@'},
        {'3', '#'},
        {'4', '$'},
        {'5', '%'},
        {'6', '^'},
        {'7', '&'},
        {'8', '*'},
        {'9', '('},
        {'0', ')'},
        {'-', '_'},
        {'=', '+'},
        {'\b', '\b'},          // Backspace
        {'\t', '\t'},          // Tab
        {'q', 'Q'},
        {'w', 'W'},
        {'e', 'E'},
        {'r', 'R'},
        {'t', 'T'},
        {'y', 'Y'},
        {'u', 'U'},
        {'i', 'I'},
        {'o', 'O'},
        {'p', 'P'},
        {'[', '{'},
        {']', '}'},
        {'\n', '\n'},          // Enter
        {LEFT_CONTROL, LEFT_CONTROL},
        {'a', 'A'},
        {'s', 'S'},
        {'d', 'D'},
        {'f', 'F'},
        {'g', 'G'},
        {'h', 'H'},
        {'j', 'J'},
        {'k', 'K'},
        {'l', 'L'},
        {';', ':'},
        {'\'', '\"'},
        {'`', '~'},
        {LEFT_SHIFT, LEFT_SHIFT},
        {'\\', '|'},
        {'z', 'Z'},
        {'x', 'X'},
        {'c', 'C'},
        {'v', 'V'},
        {'b', 'B'},
        {'n', 'N'},
        {'m', 'M'},
        {',', '<'},
        {'.', '>'},
        {'/', '?'},
        {RIGHT_SHIFT, RIGHT_SHIFT},
        {KEYPAD_STAR_PRTSC, KEYPAD_STAR_PRTSC},
        {LEFT_ALT, LEFT_ALT},
        {' ', ' '},
        {CAPS_LOCK, CAPS_LOCK},
        {F1, F1},
        {F2, F2},
        {F3, F3},
        {F4, F4},
        {F5, F5},
        {F6, F6},
        {F7, F7},
        {F8, F8},
        {F9, F9},
        {F10, F10},
        {NUM_LOCK, NUM_LOCK},
        {SCROLL_LOCK, SCROLL_LOCK},
        {HOME_KEYPAD, '7'},
        {UP_ARROW_KEYPAD, '8'},
        {PG_UP_KEYPAD, '9'},
        {'-', '-'},
        {LEFT_ARROW_KEYPAD, '4'},
        { '5',  '5'},
        {RIGHT_ARROW_KEYPAD, '6'},
        {PLUS_KEYPAD, PLUS_KEYPAD},
        {END_KEYPAD, '1'},
        {DOWN_ARROW_KEYPAD, '2'},
        {PG_DN_KEYPAD, '3'},
        {INS_KEYPAD, '0'},
        {DEL_KEYPAD, '.'},
        {SYS_REQ_AT, SYS_REQ_AT},
        {0,0},                          //scan code (0x55) vacio
        {0,0},                          //scan code (0x56) vacio
        {F11, F11},
        {F12, F12}
};

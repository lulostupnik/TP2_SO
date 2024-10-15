// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ipod.h"

Note happy_birthday_notes[] = {
	{262, 500}, // C4, quarter note
	{262, 500}, // C4, quarter note
	{294, 500}, // D4, quarter note
	{262, 500}, // C4, quarter note
	{349, 500}, // F4, quarter note
	{330, 500}, // E4, quarter note
	{262, 500}, // C4, quarter note
	{262, 500}, // C4, quarter note
	{294, 500}, // D4, quarter note
	{262, 500}, // C4, quarter note
	{392, 500}, // G4, quarter note
	{349, 500}, // F4, quarter note
	{262, 500}, // C4, quarter note
	{262, 500}, // C4, quarter note
	{523, 500}, // C5, quarter note
	{392, 500}, // G4, quarter note
	{349, 500}, // F4, quarter note
	{330, 500}, // E4, quarter note
	{294, 500}, // D4, quarter note
	{494, 500}, // B4, quarter note
	{494, 500}, // B4, quarter note
	{440, 500}, // A4, quarter note
	{349, 500}, // F4, quarter note
	{392, 500}, // G4, quarter note
	{349, 500}, // F4, quarter note
};

Note arcade_song_notes[] = {
	{262, 250}, // C4, eighth note
	{330, 250}, // E4, eighth note
	{392, 250}, // G4, eighth note
	{523, 250}, // C5, eighth note
	{392, 250}, // G4, eighth note
	{330, 250}, // E4, eighth note
	{262, 250}, // C4, eighth note
	{294, 250}, // D4, eighth note
	{349, 250}, // F4, eighth note
	{440, 250}, // A4, eighth note
	{294, 250}, // D4, eighth note
	{440, 250}, // A4, eighth note
	{349, 250}, // F4, eighth note
	{294, 250}, // D4, eighth note
	{262, 250}, // C4, eighth note
	{330, 250}, // E4, eighth note
	{392, 250}, // G4, eighth note
	{523, 250}, // C5, eighth note
	{392, 250}, // G4, eighth note
	{330, 250}, // E4, eighth note
	{262, 250}, // C4, eighth note
};

Note twinkle_twinkle_notes[] = {
	{262, 500}, // C4, quarter note
	{262, 500}, // C4, quarter note
	{392, 500}, // G4, quarter note
	{392, 500}, // G4, quarter note
	{440, 500}, // A4, quarter note
	{440, 500}, // A4, quarter note
	{392, 1000}, // G4, half note
	{349, 500}, // F4, quarter note
	{349, 500}, // F4, quarter note
	{330, 500}, // E4, quarter note
	{330, 500}, // E4, quarter note
	{294, 500}, // D4, quarter note
	{294, 500}, // D4, quarter note
	{262, 1000}, // C4, half note
	{392, 500}, // G4, quarter note
	{392, 500}, // G4, quarter note
	{349, 500}, // F4, quarter note
	{349, 500}, // F4, quarter note
	{330, 500}, // E4, quarter note
	{330, 500}, // E4, quarter note
	{294, 1000}, // D4, half note
	{392, 500}, // G4, quarter note
	{392, 500}, // G4, quarter note
	{349, 500}, // F4, quarter note
	{349, 500}, // F4, quarter note
	{330, 500}, // E4, quarter note
	{330, 500}, // E4, quarter note
	{294, 1000}, // D4, half note
	{262, 500}, // C4, quarter note
	{262, 500}, // C4, quarter note
	{392, 500}, // G4, quarter note
	{392, 500}, // G4, quarter note
	{440, 500}, // A4, quarter note
	{440, 500}, // A4, quarter note
	{392, 1000}, // G4, half note
	{349, 500}, // F4, quarter note
	{349, 500}, // F4, quarter note
	{330, 500}, // E4, quarter note
	{330, 500}, // E4, quarter note
	{294, 500}, // D4, quarter note
	{294, 500}, // D4, quarter note
	{262, 1000}, // C4, half note
};


Song happy_birthday = {
	.note_count = 25,
	.notes = happy_birthday_notes,
	.name = "Happy Birthday"
};

Song arcade_song = {
	.note_count = 21,
	.notes = arcade_song_notes,
	.name = "Arcade Song"
};

Song twinkle_twinkle = {
	.note_count = 42,
	.notes = twinkle_twinkle_notes,
	.name = "Twinkle Twinkle"
};

Song * my_songs[SONG_AMOUNT] = {
	&happy_birthday,
	&arcade_song,
	&twinkle_twinkle
};


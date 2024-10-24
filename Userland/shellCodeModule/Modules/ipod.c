// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <ipod.h>


void ipod_menu()
{
	libc_puts ( "Which of the following songs would you like to hear:\n" );
	for ( int i = 0; i < SONG_AMOUNT; i++ ) {
		libc_printf ( "%c. %s\n", i + '0', my_songs[i]->name );
	}

	uint64_t song_number;
	song_number = ( uint64_t ) ( libc_get_char() - '0' );
	if ( song_number >= SONG_AMOUNT ) {
		libc_fprintf ( STDERR, "Invalid song id\n" );
		return;
	}
	play_song ( my_songs[song_number] );
}

void play_song ( Song * song )
{
	libc_printf ( "Playing song: %s\n", song->name );
	for ( int i = 0; i < song->note_count; i++ ) {
		Note note = song->notes[i];
		sys_beep ( note.frequency, note.duration / MS_PER_TICK );
	}
}



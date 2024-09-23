#ifndef IPOD_H
#define IPOD_H

#include <stdint.h>
#include <libc.h>


#define MS_PER_TICK 55
#define SONG_AMOUNT 3

typedef struct {
    uint32_t frequency;
    int duration; // duration in ms
} Note;

typedef struct {
    uint64_t note_count;
    Note* notes;
    char* name;
} Song;

extern Song* my_songs[SONG_AMOUNT];


void initialize_songs();
void ipod_menu();
void play_song(Song* song);


#endif
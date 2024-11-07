
#ifndef _DRIVERSINFORMATION_H
#define _DRIVERSINFORMATION_H

#define STDOUT 0
#define STDERR 1
#define STDIN 2

#define CANT_FDS 3
#define MAX_COMMON_FD 2

#define TEXT_MODE 0
#define VIDEO_MODE 1

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color;



#endif

#include <shared_libc.h>

void * shared_libc_memset ( void * destination, int32_t c, uint64_t length )
{
	uint8_t chr = ( uint8_t ) c;
	char * dst = ( char * ) destination;

	while ( length-- )
		dst[length] = chr;

	return destination;
}

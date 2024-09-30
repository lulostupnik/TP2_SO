#include "test_mm.h"

typedef struct MM_rq {
	void *address;
	uint32_t size;
} mm_rq;

uint64_t test_mm ( uint64_t argc, char *argv[] )
{

	mm_rq mm_rqs[MAX_BLOCKS];
	uint64_t rq;
	uint32_t total;
	uint64_t max_memory;

	if ( argc != 1 )
		return -1;

	if ( ( max_memory = satoi ( argv[0] ) ) <= 0 )
		return -1;

	while ( 1 ) {
		rq = 0;
		total = 0;

		// request as many blocks as we can
		while ( rq < MAX_BLOCKS && total < max_memory ) {
			mm_rqs[rq].size = get_uniform ( max_memory - total - 1 ) + 1;
			mm_rqs[rq].address = my_malloc ( mm_rqs[rq].size );

			if ( mm_rqs[rq].address ) {
				total += mm_rqs[rq].size;
				rq++;
			}
		}

		// set
		uint32_t i;
		for ( i = 0; i < rq; i++ )
			if ( mm_rqs[i].address )
				memset ( mm_rqs[i].address, i, mm_rqs[i].size );

		// check
		for ( i = 0; i < rq; i++ )
			if ( mm_rqs[i].address )
				if ( !memcheck ( mm_rqs[i].address, i, mm_rqs[i].size ) ) {
					printf ( "test_mm error\n" );
					return -1;
				}

		// free
		for ( i = 0; i < rq; i++ )
			if ( mm_rqs[i].address )
				my_free ( mm_rqs[i].address );
	}
}

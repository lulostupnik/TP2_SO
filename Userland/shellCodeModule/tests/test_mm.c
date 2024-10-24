// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include <test_mm.h>


typedef struct MM_rq {
	void * address;
	uint64_t size;
} mm_rq;

uint64_t test_mm ( char * argv[],  uint64_t argc )
{

	mm_rq mm_rqs[MAX_BLOCKS];
	uint64_t rq;
	uint64_t total;
	uint64_t max_memory;

	if ( argc != 2 ) {
		libc_fprintf(STDERR, "Error: argc must be 2" );
		return -1;
	}


	if ( ( max_memory = satoi ( argv[1] ) ) <= 0 ) {
		libc_fprintf(STDERR, "Error: could not read from argv" );
		return -1;
	}


	libc_printf ( "\nRunning test....\n" );
	while (1) {
		rq = 0;
		total = 0;

		// Request as many blocks as we can
		while ( rq < MAX_BLOCKS && total < max_memory ) {
			mm_rqs[rq].size = get_uniform ( max_memory - total - 1 ) + 1;
			mm_rqs[rq].address = libc_malloc ( mm_rqs[rq].size );


			if ( mm_rqs[rq].address ) {
				total += mm_rqs[rq].size;
				rq++;
			}
		}

		// Set
		uint32_t i;
		for ( i = 0; i < rq; i++ )
			if ( mm_rqs[i].address )
				shared_libc_memset ( mm_rqs[i].address, i, mm_rqs[i].size );

		// Check
		for ( i = 0; i < rq; i++ )
			if ( mm_rqs[i].address )
				if ( !memcheck ( mm_rqs[i].address, i, mm_rqs[i].size ) ) {
					libc_fprintf(STDERR, "test_mm ERROR\n" );
					return -1;
				}

		// Free
		for ( i = 0; i < rq; i++ )
			if ( mm_rqs[i].address )
				libc_free ( mm_rqs[i].address );
	}
	return 0;
}
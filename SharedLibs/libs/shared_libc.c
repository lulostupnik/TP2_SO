#include <shared_libc.h>

void * shared_libc_memset ( void * destination, int32_t c, uint64_t length )
{
	uint8_t chr = ( uint8_t ) c;
	char * dst = ( char * ) destination;

	while ( length-- )
		dst[length] = chr;

	return destination;
}



void * shared_libc_memcpy ( void * destination, const void * source, uint64_t length )
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ( ( uint64_t ) destination % sizeof ( uint32_t ) == 0 &&
	        ( uint64_t ) source % sizeof ( uint32_t ) == 0 &&
	        length % sizeof ( uint32_t ) == 0 ) {
		uint32_t * d = ( uint32_t * ) destination;
		const uint32_t * s = ( const uint32_t * ) source;

		for ( i = 0; i < length / sizeof ( uint32_t ); i++ )
			d[i] = s[i];
	} else {
		uint8_t * d = ( uint8_t * ) destination;
		const uint8_t * s = ( const uint8_t * ) source;

		for ( i = 0; i < length; i++ )
			d[i] = s[i];
	}

	return destination;
}


/**
 * @brief Calculates the length of a string.
 *
 * This function calculates the length of the null terminated string pointed to by `str`, excluding the terminating null byte ('\0').
 *
 * @param str The string whose length is to be calculated.
 * @return size_t Returns the number of characters in the string pointed to by `str`.
 */


uint64_t shared_libc_strlen ( const char * str )
{
	const char * s = str;
	while ( *s )
		++s;
	return s - str;
}



char * shared_libc_new_str_copy(char * string, void *(*malloc_fun)(uint64_t)){
	if(string == NULL || malloc_fun == NULL ){
		return NULL;
	}
	uint64_t len = shared_libc_strlen(string) + 1;
	char * copy = malloc_fun(len);
	if(copy == NULL){
		return NULL;
	}
	shared_libc_memcpy(copy, string, len);
	return copy;
}

char ** shared_libc_copy_argv(pid_t pid, char ** argv, uint64_t argc, void *(*malloc_fun)(uint64_t), void (*free_fun)(void *))
{
	if (argc == 0 || argv == NULL || malloc_fun == NULL || free_fun == NULL) {
		return NULL;
	}

	char ** ans = malloc_fun(sizeof(char *) * (argc + 1));

	if (ans == NULL) {
		return NULL;
	}

	for (uint64_t i = 0; i < argc; i++) {
		ans[i] = shared_libc_new_str_copy(argv[i], malloc_fun);
		if (ans[i] == NULL) {
			for (uint64_t j = 0; j < i; j++) {
				free_fun((void *)ans[j]);
			}
			free_fun((void *)ans);
			return NULL;
		}
	}
	return ans;
}

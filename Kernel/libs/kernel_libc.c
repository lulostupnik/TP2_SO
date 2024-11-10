// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kernel_libc.h>


char * new_str_copy(char * string){
	if(string == NULL){
		return NULL;
	}
	uint64_t len = shared_libc_strlen(string) + 1;
	char * copy = my_malloc(len);
	if(copy == NULL){
		return NULL;
	}
	shared_libc_memcpy(copy, string, len);
	return copy;
}

char ** copy_argv(pid_t pid, char ** argv, uint64_t argc)
{
	if (argc == 0 || argv == NULL) {
		return NULL;
	}

	char ** ans = my_malloc(sizeof(char *) * (argc + 1));

	if (ans == NULL) {
		return NULL;
	}

	for (uint64_t i = 0; i < argc; i++) {
		ans[i] = new_str_copy(argv[i]);
		if (ans[i] == NULL) {
			for (uint64_t j = 0; j < i; j++) {
				my_free((void *)ans[j]);
			}
			my_free((void *)ans);
			return NULL;
		}
	}
	return ans;
}


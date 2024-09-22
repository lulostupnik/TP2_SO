#include <test_mm_module.h>


void start_test_mm(){
    char * argv[] = {MAX_MEMORY};
    if(test_mm(1, argv) == -1){
        printf("Could not start test\n");
    }
}
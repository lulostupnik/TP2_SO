#include <test_mm_module.h>


void start_test_mm(){
    char * argv[] = {"32"};
    if(test_mm(1, argv) == -1){
        printf("Could not start test\n");
    }
}
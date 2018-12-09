//////////////////////////////////////////////
//    Simple demo of _kbhit()

#include "kbhit.hpp"
#include <unistd.h>

int main(int argc, char* argv[]) {
    printf("Press any key");
    while(true) {
        while (!_kbhit()) {
            printf(".");
            fflush(stdout);
            fflush(stdin);
            usleep(100000);
        }
        int c = fgetc(stdin);
    }

    return 0;
} 
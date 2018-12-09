#include "xgetchar.hpp"

int main(void)
{

    while (true) {
        char c;
        if(c = xgetchar()){
            if(c >= 0) printf("[%d]", c);
        //    char c = getchar();
        //    printf("'%c'を押しました。\n", getchar());
        }
        printf(".");
        fflush(stdout);
        usleep(16667);
    }

    return 0;
}
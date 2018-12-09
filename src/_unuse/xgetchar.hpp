#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stropts.h>

int xgetchar(void)
{
//    struct termios oldt;
    struct termios newt;
    int ch = -2;
//    int oldf;

//    tcgetattr(STDIN_FILENO, &oldt);
//    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

//    ch = getchar();
    int n;
    if (ioctl(0, I_NREAD, &n) == 0 && n > 0) {
        ch = fgetc(stdin);
    }
    fflush(stdin);

//    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // if (ch != EOF) {
    //     ungetc(ch, stdin);
    //     return 1;
    // }

    // return 0;

    return ch;
}

#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stropts.h>
#include <unistd.h>

int main(void)
{

    int ch;

    struct termios terminal_settings;

    // Get the current terminal settings
    if (tcgetattr(0, &terminal_settings) < 0)
    perror("tcgetattr()");


    // disable canonical mode processing in the line discipline driver
    terminal_settings.c_lflag &= ~ICANON;

    // apply our new settings
    if (tcsetattr(0, TCSANOW, &terminal_settings) < 0)
    perror("tcsetattr ICANON");

    printf("waiting...");
    fflush(stdout);
    sleep(5);

    int n;
    // Now, getchar won't wait until enter is pressed.
    if (ioctl(0, I_NREAD, &n) == 0 && n > 0) {
        printf("(%d bytes to read)\n", n);
        ch = getchar();
    }else{
        printf("no bytes to read\n");
    }

    printf("%d\n", ch);


    return 0;
}
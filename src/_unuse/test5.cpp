#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    fd_set readfds;
    FD_ZERO(&readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    char message[50];

    while(1)
    {
        FD_SET(STDIN_FILENO, &readfds);

        if (select(1, &readfds, NULL, NULL, &timeout))
        {
            scanf("%s", message); 
            printf("Message: %s\n", message);
        }

        printf("...\n");
        sleep(1);
    }

    return(0);
}
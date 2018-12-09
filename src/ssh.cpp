#include "libssh2/ShellSSH.hpp"
using namespace libssh2;

#include <iostream>
#include <vector>
#include <sys/ioctl.h>
#include <sys/stropts.h>


int main(void){

    /*
     * Connect to SSH by password authentication
     */
    auto ssh = std::make_shared<ShellSSH>(
        "127.0.0.1", 22,
        "test",
        "hogehoge"
    );

    if(!ssh){
        std::cerr << "Failed to connect SSH.\n";
        return -1;
    }

    /*
     * Setting STDIN buffering options
     */
    ssh->clear_blocking();

    fd_set readfds;
    FD_ZERO(&readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    

    system("stty -echo");
    system("stty -icanon");
    


    std::cout << "Ready...\n";

    /*
     * Accept inputs until disconnected
     */
    while(!ssh->is_eof()) {

        /*
         * Read from remote SSH and output it to stdout
         */

        std::vector<char> obuf(65536, '\0');
        
        ssize_t n_obuf_read = ssh->read(obuf);
        if(n_obuf_read > 0) {
            printf("%s", obuf.data());
            fflush(stdout);
        }

        /*
         * check read error.
         * LIBSSH2_ERROR_EAGAIN is not error.
         */
        if(n_obuf_read < 0 && n_obuf_read != LIBSSH2_ERROR_EAGAIN){
            printf("RE(%ld)", n_obuf_read);
        }


        /*
         * Input from stdin and send it to remote SSH
         */

        FD_SET(STDIN_FILENO, &readfds);
        if(select(1, &readfds, NULL, NULL, &timeout)){
            int c = getchar();
            if(c > 0){ 
                auto st = ssh->write(char(c));
                if(st < 0) printf("WE(%ld)", st);
            }
        }


        usleep(16667);
    }

    printf("Disconnected.\n");


    /*
     * Restore STDIN buffering options
     */
    system("stty echo");
    system("stty icanon");


    return 0;
}

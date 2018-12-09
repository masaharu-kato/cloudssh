#include "libssh2/SSH.hpp"
using namespace libssh2;

#include <iostream>
#include <vector>
#include <sys/ioctl.h>
#include <sys/stropts.h>


int main(){

    const char* command = "ls -la /";

    /*
     * Connect to SSH by password authentication
     */
    auto ssh = std::make_shared<SSH>(
        "127.0.0.1", 22,
        "test",
        "hogehoge"
    );

    if(!ssh){
        std::cerr << "Failed to connect SSH.\n";
        return -1;
    }

    ssh->exec(command);


    std::cout << "Running '" << command << "' on SSH...\n";

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
            fprintf(stderr, "RE(%ld)", n_obuf_read);
        }

        usleep(16667);
    }

    printf("(END)\n");
    return 0;
}

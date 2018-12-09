#include "libssh2/Socket.hpp"
#include "libssh2/Session.hpp"
#include "libssh2/Channel.hpp"
using namespace libssh2;

#include <iostream>
#include <vector>
#include <sys/ioctl.h>
#include <sys/stropts.h>

std::shared_ptr<Channel> connect_to_ssh(
    Socket::HostAddr hostname,
    Socket::Port port,
    std::string username,
    std::string password
) {
    auto libssh2 = Libssh2::create();

    auto socket = Socket::create(hostname, port);

    auto session = libssh2->new_session();

    if(!session->handshake(socket)) {
        std::cerr << "Failed to establish SSH connection.\n";
        return nullptr;
    }

    if(!session->is_password_auth_supported(username)) {
        std::cerr << "Password Authentication not supported.\n";
        return nullptr;
    }

    if(!session->password_auth(username, password)) {
        std::cerr << "Password Authentication failed.\n";
        return nullptr;
    }

    auto channel = session->new_channel();
    channel->shell();

    return channel;
}



int main(void){

    auto channel = connect_to_ssh(
        "127.0.0.1", 22,
        "test",
        "hogehoge"
    );
    if(!channel) return -1; 

    channel->clear_blocking();

    std::cout << "Ready...\n";

    fd_set readfds;
    FD_ZERO(&readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    

    system("stty -echo");
    system("stty -icanon");
    
    while(!channel->is_eof()) {

        std::vector<char> obuf(65536, '\0');
        
        size_t n_obuf_read = channel->read(obuf);
        if(n_obuf_read > 0) {
            printf("%s", obuf.data());
            fflush(stdout);
        }
        if(n_obuf_read < 0) printf("read error");

        int n;
        FD_SET(STDIN_FILENO, &readfds);
        if(select(1, &readfds, NULL, NULL, &timeout)){
            int c = getchar();
            if(c > 0){ 
                auto st = channel->write(char(c));
                if(st < 0) printf("write error");
            }
        }

        usleep(16667);
    }

    system("stty echo");
    system("stty icanon");

    printf("Disconnected.\n");
    return 0;
}
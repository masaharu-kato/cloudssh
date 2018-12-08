#include "libssh2/Socket.hpp"
#include "libssh2/Session.hpp"
#include "libssh2/Channel.hpp"
using namespace libssh2;

#include <iostream>

void connect_to_ssh(
    const char* hostname,
    const char* username,
    const char* password
) 
{
    auto libssh2 = Libssh2::create();

    auto socket = Socket::create(hostname, 22);

    auto session = Session::create(libssh2);

    if(!session->handshake(*socket)) {
        std::cerr << "Failed to establish SSH connection.\n";
        return;
    }

    if(!session->is_password_auth_supported(username)) {
        std::cerr << "Password Authentication not supported.\n";
        return;
    }

    if(!session->password_auth(username, password)) {
        std::cerr << "Password Authentication failed.\n";
        return;
    }

    auto channel = Channel::create(session);
    channel->shell();

    std::cout << "OK!\n";

}



int main(void){
    connect_to_ssh(
        "127.0.0.1",
        "test",
        "hogehoge"
    );
}
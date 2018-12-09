#pragma once
#include "Channel.hpp"
#include <exception>

namespace libssh2 {

    class SSH : public Channel
    {
    public:

        SSH(
            Socket::HostAddr hostname,
            Socket::Port port,
            std::string username,
            std::string password
        ) {
            auto libssh2 = Libssh2::create();

            auto socket = Socket::create(hostname, port);

            session = libssh2->new_session();

            if(!session->handshake(socket)) {
                throw new std::runtime_error("Failed to establish SSH connection.");
            }

            if(!session->is_password_auth_supported(username)) {
                throw new std::runtime_error("Password Authentication not supported.");
            }

            if(!session->password_auth(username, password)) {
                throw new std::runtime_error("Password Authentication failed.\n");
            }

            open_channel();
        }

    };


}
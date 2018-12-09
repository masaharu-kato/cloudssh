#pragma once
#include "Channel.hpp"
#include <vector>
#include <stdexcept>
#include <exception>

namespace libssh2 {

    class SSH : public std::enable_shared_from_this<SSH> 
    {
    public:
        std::shared_ptr<Channel> channel;

        SSH(
            Socket::HostAddr hostname,
            Socket::Port port,
            std::string username,
            std::string password
        ) {
            auto libssh2 = Libssh2::create();

            auto socket = Socket::create(hostname, port);

            auto session = libssh2->new_session();

            if(!session->handshake(socket)) {
                throw new std::runtime_error("Failed to establish SSH connection.");
            }

            if(!session->is_password_auth_supported(username)) {
                throw new std::runtime_error("Password Authentication not supported.");
            }

            if(!session->password_auth(username, password)) {
                throw new std::runtime_error("Password Authentication failed.\n");
            }

            channel = session->new_channel();
            channel->shell();
        }

        /* 
         * get shared pointer of this
         */
        auto operator &() {
            return shared_from_this();
        }

        /*
         * access to channel object
         */
        auto operator ->() {
            return channel;
        }

        /*
         * access to channel object (const)
         */
        auto operator ->() const {
            return channel;
        }

        /*
         * get shared object of raw channel object
         */
        operator std::shared_ptr<Channel>() {
            return channel;
        }

    };


}
#pragma once
#include "Libssh2.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

namespace libssh2 {

    using SocketAddress = struct sockaddr_in;

    class Socket {
        libssh2_socket_t sock;

        Socket(const char* hostaddr, uint16_t port, int domain, int type, int protocol) : 
            sock(::socket(domain, type, protocol))
        {
            if(hostaddr) {
                connect(hostaddr, port);
            }
        }

    public:
        ~Socket() {
            close(sock);
        }

        /* 
         * Create new Socket instance
         * Ultra basic "connect to port 22 on localhost".  Your code is
         * responsible for creating the socket establishing the connection
         */
        static auto create(
            const char* hostaddr = NULL,
            uint16_t port = 0,
            int domain = AF_INET,
            int type = SOCK_STREAM,
            int protocol = 0
        ) {
            return std::shared_ptr<Socket>(new Socket(hostaddr, port, domain, type, protocol));
        }

        void connect(const char* hostaddr, uint16_t port) {

            struct sockaddr_in sin;
            sin.sin_family = AF_INET;
            sin.sin_port = htons(port);
            sin.sin_addr.s_addr = inet_addr(hostaddr);

            if(::connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in))) {
                throw new std::runtime_error("Failed to connect.\n");
            }

        }

        operator libssh2_socket_t() const {
            return sock;
        }

    };

}
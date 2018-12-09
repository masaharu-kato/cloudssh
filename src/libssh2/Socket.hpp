#pragma once
#include "Libssh2.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

namespace libssh2 {

    using SocketAddress = struct sockaddr_in;

    class Socket : public std::enable_shared_from_this<Socket> {
    public:
        using HostAddr = const char*;
        using Port = int16_t;

    private:
        libssh2_socket_t sock;
        struct sockaddr_in sin;

        Socket(
            HostAddr hostaddr, 
            Port port,
            int domain   = AF_INET,
            int type     = SOCK_STREAM,
            int protocol = 0
        ) {
            sock = ::socket(domain, type, protocol);
            if(sock == -1) {
                throw new std::runtime_error("Socket error.\n");
            }
            if(hostaddr) {
                connect(hostaddr, port);
            }
        }

    public:
        ~Socket() {
            close(sock);
        }

        auto operator &() {
            return shared_from_this();
        }

        template <class... Args>
        static auto create(Args&&... args) {
            return std::shared_ptr<Socket>(new Socket(args...));
        }

        void connect(const char* hostaddr, uint16_t port) {

            sin.sin_family = AF_INET;
            sin.sin_port = htons(port);
            sin.sin_addr.s_addr = inet_addr(hostaddr);

            printf("setting connection...\n");

            if(::connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in))) {
                throw new std::runtime_error("Failed to connect.\n");
            }

        }

        operator libssh2_socket_t() const {
            return sock;
        }

    };

}
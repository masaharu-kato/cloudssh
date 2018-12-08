#pragma once
#include "Libssh2.hpp"
#include "Session.hpp"

namespace libssh2 {

    class Channel {
        std::shared_ptr<Session> session;
        LIBSSH2_CHANNEL *channel;

        Channel(std::shared_ptr<Session> session) : 
            session(session),
            channel(libssh2_channel_open_session(*session)) {
             if(!channel) {
                throw new std::runtime_error("Unable to open a session\n");
             }
        }

    public:
        ~Channel() {
            libssh2_channel_free(channel);
        }

        /* Create new instance */
        static auto create(std::shared_ptr<Session> session) {
            return std::shared_ptr<Channel>(new Channel(session));
        }

        /* 
         * Some environment variables may be set,
         * It's up to the server which ones it'll allow though
         */
        void setenv(const char* name, const char* value) {
            libssh2_channel_setenv(channel, name, value);
        }

        void setenv(std::string name, std::string value) {
            setenv(name.c_str(), value.c_str());
        }

        /*
         * Open a SHELL
         * Request a terminal with 'vanilla' terminal emulation
         * See /etc/termcap for more options
         */
        void shell(const char* type = "vanilla") {
            if (libssh2_channel_request_pty(channel, type)) {
                throw new std::runtime_error("Failed requesting pty\n");
            }
            if (libssh2_channel_shell(channel)) {
                throw new std::runtime_error("Unable to request shell on allocated pty\n");
            }
        }

        operator LIBSSH2_CHANNEL*() const {
            return channel;
        }

    };

}
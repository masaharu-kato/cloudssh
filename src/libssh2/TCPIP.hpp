#pragma once
#include "Channel.hpp"

namespace libssh2 {

    class TCPIP : public Channel {
    private:
        friend Session;
        /*
         * construct direct TCP/IP channel
         */
        TCPIP(std::shared_ptr<Session> session, const char* host, int port, const char* shost, int sport) : Channel(session) {
             if(!(channel = libssh2_channel_direct_tcpip_ex(*session, host, port, shost, sport))) {
                throw new std::runtime_error("Unable to open TCP/IP session from session\n");
             }
        }

    public:
        ~TCPIP() = default;
        
    };

    /*
    * create new channel from session using direct TCP/IP
    */
    auto Session::new_channel_tcpip(const char* host, int port, const char* shost, int sport) {
        return std::shared_ptr<TCPIP>(new TCPIP(shared_from_this(), host, port, shost, sport));
    }

}
#pragma once
#include "Libssh2.hpp"
#include "Session.hpp"

namespace libssh2 {

    class Channel;

    class Listener : public std::enable_shared_from_this<Listener> {
    public:
        using HostAddr = char *;
        using Port = int;

    private:
        friend Session;

        LIBSSH2_LISTENER *listener;
        std::shared_ptr<Session> session;

        Listener(std::shared_ptr<Session> session, Port port) : session(session) {
             if(!(listener = libssh2_channel_forward_listen(*session, port))) {
                throw new std::runtime_error("Unable to forward listen\n");
             }
        }

        Listener(std::shared_ptr<Session> session, HostAddr host, Port port, Port* bound_port, int queue_maxsize) : session(session) {
             if(!(listener = libssh2_channel_forward_listen_ex(*session, host, port, bound_port, queue_maxsize))) {
                throw new std::runtime_error("Unable to forward listen\n");
             }
        }


    public:
        ~Listener() {
            libssh2_channel_forward_cancel(listener);
        }

        auto operator &() {
            return shared_from_this();
        }

        auto new_channel();

        operator LIBSSH2_LISTENER*() const {
            return listener;
        }

    };

    /*
     * create new listener from session
     */
    template <class... Args>
    auto Session::new_listener(Args&&... args) {
        return std::shared_ptr<Listener>(new Listener(shared_from_this(), args...));
    }



}
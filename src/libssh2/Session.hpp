#pragma once
#include "Libssh2.hpp"
#include "Socket.hpp"

namespace libssh2 {

    class Channel;

    class Session : public std::enable_shared_from_this<Session> {

        friend Libssh2;

        LIBSSH2_SESSION *session;
        std::shared_ptr<Libssh2> _libssh2;
        std::shared_ptr<Socket> sock;

        Session(std::shared_ptr<Libssh2> _libssh2)
         : _libssh2(_libssh2), session(libssh2_session_init())
        {}

    public:
        ~Session() {
            libssh2_session_free(session);
        }

        /* 
         * get shared pointer of this
         */
        auto operator &() {
            return shared_from_this();
        }

        auto new_channel();
        
        auto new_channel_tcpip(const char* host, int port, const char* shost, int sport);

        template <class... Args>
        auto new_listener(Args&&... args);


        /* Create a session instance and start it up. This will trade welcome
        * banners, exchange keys, and setup crypto, compression, and MAC layers.
        */
        bool handshake(std::shared_ptr<Socket> _sock) {
            sock = _sock;
            return !libssh2_session_handshake(session, *sock);
        }

        /* 
         * return a hash of the remote host's key 
         *   At this point we havn't authenticated. The first thing to do is check
         *   the hostkey's fingerprint against our known hosts Your app may have it
         *   hard coded, may go to a file, may present it to the user, that's your call.
         */
        const char* fingerprint(int hash_type = LIBSSH2_HOSTKEY_HASH_SHA1) {
            return libssh2_hostkey_hash(session, hash_type);
        }

        const char* get_supported_auth_list(const char* username) {
            return libssh2_userauth_list(session, username, strlen(username));
        }

        const char* get_supported_auth_list(std::string username) {
            return libssh2_userauth_list(session, username.c_str(), username.length());
        }

        template <class T>
        bool is_password_auth_supported(T username) {
            return strstr(get_supported_auth_list(username), "password");
        }

        template <class T>
        bool is_pubkey_auth_supported(T username) {
            return strstr(get_supported_auth_list(username), "pubkey");
        }


        /*
         * authenticate a session with username and password 
         *   username: Name of user to attempt plain password authentication for. 
         *   password: Password to use for authenticating username. 
         * 
         *   Return true on success or false on failure.
         */
        bool password_auth(const char* username, const char* password) {
            return !libssh2_userauth_password(session, username, password);
        }

        /*
         * authenticate a session with username and password 
         *   username: Name of user to attempt plain password authentication for. 
         *   password: Password to use for authenticating username. 
         *   passwd_change_cb:
         *       If the host accepts authentication but requests that the password be changed,this callback will be issued.
         *       If no callback is defined, but server required password change, authentication will fail. 
         * 
         *   Return true on success or false on failure.
         */
        bool password_auth(std::string username, std::string password, LIBSSH2_PASSWD_CHANGEREQ_FUNC((*passwd_change_cb)) = NULL) {
            return !libssh2_userauth_password_ex(session, username.c_str(), username.length(), password.c_str(), password.length(), passwd_change_cb);
        }

        /*
         * return authentication status 
         *   Returns true if authenticated and false if not. 
         */
        bool authenticated() {
            return  libssh2_userauth_authenticated(session);
        }


        /*
         * set blocking mode on session 
         */
        void set_blocking() {
            libssh2_session_set_blocking(session, 1);
        }

        /*
         * clear blocking mode on session 
         */
        void clear_blocking() {
            libssh2_session_set_blocking(session, 0);
        }

        /*
         * set a callback function 
         *   cbtype: specify one kind of callback from LIBSSH2_CALLBACK_*
         *   callback: pointer to callback function
         */
        void set_callback(int cbtype, void *callback) {
            libssh2_session_callback_set(session, cbtype, callback);
        }

        /*
         * get raw pointer
         */
        operator LIBSSH2_SESSION*() const {
            return session;
        }

    };

    auto Libssh2::new_session() {
        return std::shared_ptr<Session>(new Session(shared_from_this()));
    }

}
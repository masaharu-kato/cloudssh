#pragma once
#include "Libssh2.hpp"
#include "Socket.hpp"

namespace libssh2 {

    class Session {
        std::shared_ptr<Libssh2> _libssh2;
        LIBSSH2_SESSION *session;

        Session(std::shared_ptr<Libssh2> _libssh2)
         : _libssh2(_libssh2), session(libssh2_session_init())
        {}

    public:
        ~Session() {
            libssh2_session_free(session);
        }

        /* Create new instance */
        static auto create(std::shared_ptr<Libssh2> _libssh2) {
            return std::shared_ptr<Session>(new Session(_libssh2));
        }

        /* Create a session instance and start it up. This will trade welcome
        * banners, exchange keys, and setup crypto, compression, and MAC layers.
        */
        bool handshake(const Socket& sock) {
            return !libssh2_session_handshake(session, sock);
        }

        /* 
         * At this point we havn't authenticated. The first thing to do is check
         * the hostkey's fingerprint against our known hosts Your app may have it
         * hard coded, may go to a file, may present it to the user, that's your call.
         */
        const char* fingerprint(int hash_type = LIBSSH2_HOSTKEY_HASH_SHA1) {
            return libssh2_hostkey_hash(session, hash_type);
        }

        const char* get_supported_auth_list(const char* username) {
            return libssh2_userauth_list(session, username, strlen(username));
        }

        bool is_password_auth_supported(const char* username) {
            return strstr(get_supported_auth_list(username), "password");
        }

        bool is_pubkey_auth_supported(const char* username) {
            return strstr(get_supported_auth_list(username), "pubkey");
        }

        bool password_auth(const char* username, const char* password) {
            return !libssh2_userauth_password(session, username, password);
        }

        operator LIBSSH2_SESSION*() const {
            return session;
        }

    };

}
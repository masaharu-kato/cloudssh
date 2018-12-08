#include "libssh2/Socket.hpp"
#include "libssh2/Session.hpp"
#include "libssh2/Channel.hpp"

#include <iostream>

int connect_to_ssh(
    std::string hostname,
    std::string username,
    std::string password
) 
{

    unsigned long hostaddr = inet_addr(hostname.c_str());
    if(libssh2_init(0)){
        throw new std::runtime_error("libssh2 initialization failed.\n");
    }

    /* Ultra basic "connect to port 22 on localhost".  Your code is
responsible for creating the socket establishing the connection
     */
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if(connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in))){
        throw new std::runtime_error("Failed to connect.\n");
    }

    /* Create a session instance and start it up. This will trade welcome
     * banners, exchange keys, and setup crypto, compression, and MAC layers
     */
    LIBSSH2_SESSION *session = libssh2_session_init();
    if(libssh2_session_handshake(session, sock)){
        throw new std::runtime_error("Failed to establish SSH session.\n");
    }

    /* At this point we havn't authenticated. The first thing to do is check
     * the hostkey's fingerprint against our known hosts Your app may have it
     * hard coded, may go to a file, may present it to the user, that's your
     * call
     */
    const char *fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);
    fprintf(stderr, "Fingerprint: ");
    for(int i = 0; i < 20; i++) {
        fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
    }
    fprintf(stderr, "\n");

    if(!strstr(libssh2_userauth_list(session, username.c_str(), username.length()), "password")){
        throw new std::runtime_error("Password Authentication is not supported on this user.\n");
    }

    /* We could authenticate via password */
    if(libssh2_userauth_password(session, username.c_str(), password.c_str())){
        throw new std::runtime_error("Password Authentication failed!\n");
    }

    /* Request a shell */
    LIBSSH2_CHANNEL *channel;
    if(!(channel = libssh2_channel_open_session(session))) {
        throw new std::runtime_error("Unable to open a session\n");
    }

    /* Some environment variables may be set,
     * It's up to the server which ones it'll allow though
     */
    libssh2_channel_setenv(channel, "FOO", "bar");

    /* Request a terminal with 'vanilla' terminal emulation
     * See /etc/termcap for more options
     */
    if (libssh2_channel_request_pty(channel, "vanilla")) {
        throw new std::runtime_error("Failed requesting pty\n");
        /* goto skip_shell; */
    }

    /* Open a SHELL on that pty */
    if (libssh2_channel_shell(channel)) {
        throw new std::runtime_error("Unable to request shell on allocated pty\n");
    }

    /* At this point the shell can be interacted with using
     * libssh2_channel_read()
     * libssh2_channel_read_stderr()
     * libssh2_channel_write()
     * libssh2_channel_write_stderr()
     *
     * Blocking mode may be (en|dis)abled with: libssh2_channel_set_blocking()
     * If the server send EOF, libssh2_channel_eof() will return non-0
     * To send EOF to the server use: libssh2_channel_send_eof()
     * A channel can be closed with: libssh2_channel_close()
     * A channel can be freed with: libssh2_channel_free()
     */

  skip_shell:
    if(channel){
        libssh2_channel_free(channel);
        channel = NULL;
    }

    /* Other channel types are supported via:
     * libssh2_scp_send()
     * libssh2_scp_recv2()
     * libssh2_channel_direct_tcpip()
     */

  shutdown:

    libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
    libssh2_session_free(session);


    close(sock);

    std::cout <<  "all done!\n";

    libssh2_exit();

    return 0;
}



int main(void){
    connect_to_ssh(
        "127.0.0.1",
        "test",
        "hogehoge"
    );
}
#include "libssh2/Socket.hpp"
#include "libssh2/Session.hpp"
#include "libssh2/Channel.hpp"
using namespace libssh2;

#include <iostream>
#include <vector>

std::shared_ptr<Channel> connect_to_ssh(
//  LIBSSH2_CHANNEL* connect_to_ssh(
    const char* hostname,
    const char* username,
    const char* password
) 
{
    auto libssh2 = Libssh2::create();

    auto socket = Socket::create(hostname, 22);

    auto session = Session::create(libssh2);

    if(!session->handshake(socket)) {
        std::cerr << "Failed to establish SSH connection.\n";
        return nullptr;
    }

    if(!session->is_password_auth_supported(username)) {
        std::cerr << "Password Authentication not supported.\n";
        return nullptr;
    }

    if(!session->password_auth(username, password)) {
        std::cerr << "Password Authentication failed.\n";
        return nullptr;
    }

    auto channel = Channel::create(session);
    channel->shell();

    return channel;

	// /* Variables to don't touch */
	// int rc;
	// struct sockaddr_in sin;
	// LIBSSH2_SESSION *session;

	// /* Libss2 init block */
	// rc = libssh2_init(0);
	// if (rc) {
	// 	fprintf(stderr, "Error: libssh_init()\n");
    //     return nullptr;
	// }


    // auto socket = Socket::create(hostname, 22);

	// /* Create a session instance and start it up. This will trade welcome
	//  * banners, exchange keys, and setup crypto, compression, and MAC layers */
	// session = libssh2_session_init();
	// if (!session) {
	// 	fprintf(stderr, "SSH init failed\n");
    //     return nullptr;
	// }

	// /* Handshake for session */
	// rc = libssh2_session_handshake(session, *socket);
	// if (rc) {
	// 	fprintf(stderr, "SSH handshake failed\n");
    //     return nullptr;
	// }

	// /* Lets authenticate */
	// rc = libssh2_userauth_password(session, username, password);
	// if (rc) {
	// 	printf("Authentication by password failed\n");
    //     return nullptr;
	// } else {
	// 	printf("Authentication by password succeeded\n");
	// }



    // auto channel = Channel::create(session);
    // channel->shell();



    // return channel;
}



int main(void){

    auto channel = connect_to_ssh(
        "127.0.0.1",
        "test",
        "hogehoge"
    );
    if(!channel) return -1;


	// char command[BUFSIZ];
	// char inputbuf[BUFSIZ];

    // /* Main loop starts here.
	//  * In it you will be requested to input a command
	//  * command will be executed at remote side
	//  * an you will get output from it */
	// do {
	// 	/* Request for command input */
	// 	printf("$ ");
	// 	fgets(command, BUFSIZ, stdin);
	// //	printf("Command is %s", command);
	// 	if (strcmp(command, "\n") == 0) {
	// 	//	printf("Empty command\n");
	// 		continue;
	// 	}

	// 	/* Write command to stdin of remote shell */
	// 	ssize_t wc = libssh2_channel_write(*channel, command, strlen(command));
	// //	printf("Channel write return value is %d\n", rc);

	// 	/* Read output from remote side */
	// 	ssize_t rc = libssh2_channel_read(*channel, inputbuf, BUFSIZ);
	// //	printf("Channel write return value is %d\n", rc);
	// 	if(rc < 0) printf("Channel write error.\n");
	// 	inputbuf[rc] = '\0';
	// 	printf("%s", inputbuf);

	// } while(strcmp(command, "exit\n") != 0);


	/* Main loop ends here */

    system("stty -echo");
    system("stty -icanon");

    //  std::vector<char> ibuf;
    //  ibuf.resize(65536);
    std::vector<char> obuf;
    obuf.resize(65536);

    std::cout << "Ready...\n";

    while(true) {

        size_t n_obuf_read = channel->read(obuf);
        fwrite(obuf.data(), 1, n_obuf_read, stdout);

        // size_t n_ibuf_read = fread(ibuf.data(), 1, ibuf.capacity(), stdin);
        // if(n_ibuf_read > 0){
        //     ibuf.resize(n_ibuf_read);
        //     channel->write(ibuf);
        // }
        int ic = fgetc(stdin);
        if(ic >= 0){
            channel->write(char(ic));
            //  printf("You entered %c (%d)\n", c, c);
        }
    //    sleep(1);
        usleep(10 * 1000);
    //    fputc('.', stdout);
    }

    system("stty echo");
    system("stty icanon");

}
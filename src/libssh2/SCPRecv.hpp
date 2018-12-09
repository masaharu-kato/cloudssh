#pragma once
#include "Channel.hpp"

namespace libssh2 {

    class SCPRecv : public Channel {

        /*
         * construct SCP reception channel
         * 
         *   session: shared pointer of session object
         *   path    : Full path and filename of file to transfer. That is the remote file name.
         *   fileinfo: Populated with remote file's size, mode, mtime, and atime
         * 
         * Request a file from the remote host via SCP. 
         */
        SCPRecv(std::shared_ptr<Session> session, const char* path, libssh2_struct_stat* fileinfo) : Channel(session) {
             if(!(channel = libssh2_scp_recv2(*session, path, fileinfo))) {
                throw new std::runtime_error("Unable to open SCP reception session from session\n");
             }
        }
        
    };

}
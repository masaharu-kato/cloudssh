#pragma once
#include "Channel.hpp"

namespace libssh2 {

    class SCPSend : public Channel {

        /*
         * construct SCP transmission channel
         * 
         *   session: shared pointer of session object
         *   path   : Full path and filename of file to transfer to. That is the remote file name.
         *   mode   : File access mode to create file with
         *   size   : Size of file being transmitted (Must be known ahead of time precisely)
         *   mtime  : mtime to assign to file being created
         *   atime  : atime to assign to file being created (Set this and mtime to zero to instruct remote host to use current time).
         * 
         * Send a file to the remote host via SCP. 
         */
        SCPSend(std::shared_ptr<Session> session, const char* path, int mode, size_t size, long mtime, long atime)
         : Channel(session)
        {
             if(!(channel = libssh2_scp_send_ex(*session, path, mode, size, mtime, atime))) {
                throw new std::runtime_error("Unable to open SCP transmission session from session\n");
             }
        }
        
    };

}
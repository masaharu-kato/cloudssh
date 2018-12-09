#pragma once
#include "Libssh2.hpp"
#include "Session.hpp"
#include "Listener.hpp"
#include <vector>

namespace libssh2 {

    class Channel : public std::enable_shared_from_this<Channel> 
    {
        friend Session;
        friend Listener;

    protected:
        LIBSSH2_CHANNEL *channel = NULL;
        std::shared_ptr<Session> session;
    //    std::shared_ptr<Listener> listener;

        auto open_channel() {
            return channel = libssh2_channel_open_session(*session);
        }

        /*
         * default constructor (for inheritanced use)
         */ 
        Channel() = default;

        /*
         * construct channel for shell from session
         */
        Channel(std::shared_ptr<Session> session) : session(session) {
             if(!open_channel()) {
                throw new std::runtime_error("Unable to open a session from session\n");
             }
        }
        
        /*
         * construct channel for shell from listener
         */
        // Channel(std::shared_ptr<Listener> listener) : listener(listener) {
        //      if(!(channel = libssh2_channel_forward_accept(*listener))) {
        //         throw new std::runtime_error("Unable to open a session from listener\n");
        //      }
        // }

    public:
        using StreamID = int;

        ~Channel() {
            libssh2_channel_free(channel);
        }

        /* 
         * get shared pointer of this
         */
        auto operator &() {
            return shared_from_this();
        }


        /* 
         * set environment variable
         *   name : name of environment variable
         *   value: value of environment variable
         */
        void setenv(const char* name, const char* value) {
            libssh2_channel_setenv(channel, name, value);
        }

        /* 
         * set environment variable
         *   name : name of environment variable
         *   value: value of environment variable
         */
        void setenv(std::string name, std::string value) {
            libssh2_channel_setenv_ex(channel, name.c_str(), name.length(), value.c_str(), value.length());
        }


        /*
         * open a sHELL
         *   term: name of terminal emulator
         *   To check list of available terminal emulators, run 
         *     tree /etc/terminfo
         *     tree /lib/terminfo
         *     tree /usr/share/terminfo
         */
        void shell(const char* term = "xterm") {
            if(libssh2_channel_request_pty(channel, term)) {
                throw new std::runtime_error("Failed requesting pty\n");
            }
            if(libssh2_channel_shell(channel)) {
                throw new std::runtime_error("Unable to request shell on allocated pty\n");
            }
        }

        /*
         * open a sHELL
         *   term       : name of terminal emulator
         *   modes      : Terminal mode modifier values 
         *   width_char : Width of pty in characters
         *   height_char: Height of pty in characters
         *   width_px   : Width of pty in pixels
         *   height_px  : Height of pty in pixels 
         */
        void shell(std::string term, std::string modes, int width_char, int height_char, int width_px, int height_px) {
            if(libssh2_channel_request_pty_ex(channel, term.c_str(), term.length(), modes.c_str(), modes.length(), width_char, height_char, width_px, height_px)) {
                throw new std::runtime_error("Failed requesting pty\n");
            }
            if(libssh2_channel_shell(channel)) {
                throw new std::runtime_error("Unable to request shell on allocated pty\n");
            }
        }

        /*
         * set blocking mode on channel
         *   alias for Session::set_blocking(), and therefore will affect the session and all channels
         */
        void set_blocking() {
            libssh2_channel_set_blocking(channel, 1);
        }

        /*
         * clear blocking mode on channel 
         *   alias for Session::clear_blocking(), and therefore will affect the session and all channels
         */
        void clear_blocking() {
            libssh2_channel_set_blocking(channel, 0);
        }


        auto write(const std::vector<char>& in) {
            return libssh2_channel_write(channel, in.data(), in.size());
        }

        auto write(const std::vector<char>& in, StreamID sid) {
            return libssh2_channel_write_ex(channel, sid, in.data(), in.size());
        }

        auto write(char c) {
            return libssh2_channel_write(channel, &c, 1);
        }

        auto write(char c, StreamID sid) {
            return libssh2_channel_write_ex(channel, sid, &c, 1);
        }


        /*
         * read data from a channel stream 
         *   out: buffer to output read data
         */
        auto read(std::vector<char>& out){
            return libssh2_channel_read(channel, out.data(), out.size());
        }

        /*
         * read data from a specified stream on channel
         *   out: buffer to output read data
         *   sid: stream id to read
         */
        auto read(std::vector<char>& out, StreamID sid){
            return libssh2_channel_read_ex(channel, sid, out.data(), out.size());
        }


        /*
         *  flush a channel 
         */
        auto flush() {
            return libssh2_channel_flush(channel);
        }
        
        /*
         *  flush a specified stream on channel 
         *    sid: stream id to read
         */
        auto flush(StreamID sid) {
            return libssh2_channel_flush_ex(channel, sid);
        }


        auto exec(const char* command) {
            return libssh2_channel_exec(channel, command);
        }

        auto subsystem(const char* subsystem) {
            return libssh2_channel_subsystem(channel, subsystem);
        }

        /*
         * request a shell on a channel 
         *   request: Type of process to startup. The SSH2 protocol currently defines shell, exec, and subsystem as standard process services.
         *   message: Request specific message data to include.
         */
        auto process_startup(std::string request, std::string message) {
            return libssh2_channel_process_startup(channel, request.c_str(), request.length(), message.c_str(), message.length());
        }

        auto receive_window_adjust(unsigned long adjustment, unsigned char force, unsigned int* window) {
            return libssh2_channel_receive_window_adjust2(channel, adjustment, force, window);
        }

        /*
         * check a channel's EOF status 
         */
        auto is_eof() {
            return libssh2_channel_eof(channel);
        }

        /*
         * send EOF to remote server 
         */
        auto send_eof() {
            return libssh2_channel_send_eof(channel);
        }

        /*
         * close a channel
         */
        auto close() {
            return libssh2_channel_close(channel);
        }

        /*
         * wait for the remote to close the channel 
         */
        auto wait_closed() {
            return libssh2_channel_wait_closed(channel);
        }

        /*
         * wait for the remote to reply to an EOF request 
         */
        auto wait_eof() {
            return libssh2_channel_wait_eof(channel);
        }

        /*
         * get raw channel pointer
         */
        operator LIBSSH2_CHANNEL*() const {
            return channel;
        }

    };

    /*
    * create new channel from session
    */
    auto Session::new_channel() {
        return std::shared_ptr<Channel>(new Channel(shared_from_this()));
    }

    // /*
    // * create new channel from listener
    // */
    // auto Listener::new_channel() {
    //     return std::shared_ptr<Channel>(new Channel(shared_from_this()));
    // }


}
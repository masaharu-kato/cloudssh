#pragma once
#include <libssh2.h>
#include <stdexcept>
#include <exception>
#include <memory>

namespace libssh2 {

    class Session;

    class Libssh2 : public std::enable_shared_from_this<Libssh2>  {
    private:
        Libssh2() {
            if(libssh2_init(0)){
                throw new std::runtime_error("libssh2 initialization failed.\n");
            } 
        }

    public:
        ~Libssh2() {
            libssh2_exit();
        }

        auto new_session();


        static std::shared_ptr<Libssh2> ptr;

        static std::shared_ptr<Libssh2> create() {
            if(!ptr) ptr.reset(new Libssh2());
            return ptr;
        }
    };

    std::shared_ptr<Libssh2> Libssh2::ptr;

}
#pragma once
#include "SSH.hpp"
#include <exception>

namespace libssh2 {

    class ShellSSH : public SSH
    {
    public:

        template <class... Args>
        ShellSSH(Args&&... args) : SSH(args...) {
            shell();
        }

    };


}
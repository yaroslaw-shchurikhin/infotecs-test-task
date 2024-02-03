#include "handler.hpp"

#include <iostream>
#include <string>

#include <unistd.h>

#include <sys/socket.h>

namespace detail
{

Handler::Handler(int32_t client_sock, uint32_t buf_size) :
    client_sock_(client_sock),
    buf_size_(buf_size)
{
    buf_ = new char[buf_size_];
}

Handler::~Handler()
{
    delete[] buf_;
    buf_ = nullptr;
    close(client_sock_);
}

void Handler::start()
{
    constexpr size_t MIN_MSG_LENGTH = 3;
    constexpr size_t DIVIDER_OF_MSG = 32;

    uint32_t r_count = 1;
    while(r_count != 0)
    {
        // Send to client that server ready to receive data
        buf_[0] = 'R';
        send(client_sock_, buf_, 1, 0);

        r_count = recv(client_sock_, buf_, buf_size_, 0);
        if (r_count != 0)
        {
            int32_t sum = std::stoi(buf_);
            if (static_cast<std::string>(buf_).length() >= MIN_MSG_LENGTH &&
                std::stoi(buf_) % DIVIDER_OF_MSG == 0)
            {
                std::cout << "RECEIVED DATA: " << buf_ << '\n';
            }
            else
            {
                std::cout << "ERROR\n";
            }
        }
    }
}
} // namespace detail

#include "handler.hpp"

#include <iostream>
#include <thread>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <stdexcept>

#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#include "buffer.hpp"
#include "../../../config.hpp"

namespace detail
{

constexpr uint16_t BUFFER_SIZE = 256;
constexpr char SOCK_PATH[] = EXCHANGE_FILE;

Handler::Handler() :
    client_sock_(-1),
    connected_(false)
{
    socket_buf_ = new char[BUFFER_SIZE];

    server_sockaddr_ = {0};
    server_sockaddr_.sun_family = AF_UNIX;
    strcpy(server_sockaddr_.sun_path, SOCK_PATH);
}

Handler::~Handler()
{
    delete[] socket_buf_;
    socket_buf_ = nullptr;
    close(client_sock_);
}

void Handler::start(Buffer<std::string>& buffer)
{
    std::string next_str;
    for(;;)
    {
        if(!connected_)
        {
            connectToServer();
        }
    
        // sorry about essay like "How my summer" ;)
        // About 'R' message from server:
        // -------------------------------
        // Initially, I didn't do this functionality 
        // - client sent data immediately when connect, 
        // but then errors occurred - for example, 
        // if server wasn't running, 
        // and client was ready to send several 
        // sums at once, then server eventually 
        // read them from buffer as one 
        // amount (two sum '4' read as '44'). 
        // A similar situation occurred when I quickly 
        // entering a many different strings 
        // in program 1 when program 2 is running

        // Checks if server ready to receive data
        recv(client_sock_, socket_buf_, BUFFER_SIZE, 0);
        if(socket_buf_[0] == 'R')
        {
            // extract data from buffer
            next_str = buffer.pop();
            std::cout << next_str << '\n';

            int32_t sum = 0;
            std::for_each(next_str.cbegin(), next_str.cend(), [&sum](char x) 
            { 
                sum += isdigit(x) ? static_cast<int32_t>(x - '0') : 0;
            });
            
            // Send data
            send(client_sock_,
                strcpy(socket_buf_, std::to_string(sum).c_str()),
                BUFFER_SIZE, MSG_NOSIGNAL);
        }
        else
        {
            connected_ = false;
            buffer.push_front(next_str);
            close(client_sock_);
        }
    }
}


void Handler::connectToServer()
{
    constexpr uint16_t WAIT_FOR_RECONNECT = 1000u;

    // Create a UNIX domain stream socket
    client_sock_ = socket(AF_UNIX, SOCK_STREAM, 0);
    if(client_sock_ < 0)
    {
        throw std::runtime_error("Socket error.");
    }

    // Connect to server
    while (!connected_)
    {
        connected_ = connect(client_sock_,
            reinterpret_cast<sockaddr*>(&server_sockaddr_), sizeof(server_sockaddr_)) == 0;
        // Wait before try to reconnect again
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_FOR_RECONNECT));
    }
}
} // namespace detail

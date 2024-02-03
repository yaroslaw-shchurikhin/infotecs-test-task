#include "server.hpp"

#include <thread>
#include <stdexcept>
#include <iostream>
#include <mutex>

#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#include "handler.hpp"

namespace detail
{

Server::Server(const std::string& server_path, const uint32_t buf_size, const uint32_t max_queued_connections) :
    server_path_(server_path),
    buf_size_(buf_size),
    max_queued_connections_(max_queued_connections)
{
    buf_ = new char[buf_size_];
}

Server::~Server()
{
    delete[] buf_; 
    buf_ = nullptr;
    close(server_sock_);
}

void Server::start()
{
    if(!openToConnect())
    {
        throw std::runtime_error("SERVER SOCKET ERROR");
    }

    std::exception_ptr exc_ptr = nullptr;
    std::mutex exc_mutex;
    std::mutex exc_occurred;
    exc_occurred.lock();
    
    // Create new thread to handle exceptions from handler threads
    std::thread exc_handler([&exc_ptr, &exc_occurred] () {
      try
      {
        exc_occurred.lock();
        std::rethrow_exception(exc_ptr);
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << '\n';
        exit(1);
      }
    });
    exc_handler.detach();

    for(;;)
    {
        // Listen for any client sockets
        if (listen(server_sock_, max_queued_connections_) == -1)
        {
           throw std::runtime_error("SERVER LISTEN ERROR");
        }

        // Accept an incoming connection
        int32_t client_sock = accept(server_sock_, nullptr, nullptr); 
        if (client_sock == -1)
        {
            close(client_sock);
            throw std::runtime_error("SERVER ACCEPTION ERROR");
        }

        // Create separate thread to work with client
        uint32_t buf_size = buf_size_;
        std::thread thread_handler ([&client_sock, &buf_size, &exc_ptr, &exc_mutex, &exc_occurred]()
        {
            try
            {
                Handler handler(client_sock, buf_size);
                handler.start();
            }
            catch(...)
            {
                exc_mutex.lock();
                exc_ptr = std::current_exception();
                exc_occurred.unlock();
                exc_mutex.unlock();
            }
        });
        thread_handler.detach();
    }
}

bool Server::openToConnect()
{
    struct sockaddr_un server_sockaddr_ {0};

    // Create a UNIX domain stream socket 
    server_sock_ = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock_ < 0)
    {
       return false;
    }

    // Set up the UNIX sockaddr structure
    server_sockaddr_.sun_family = AF_UNIX;   
    strcpy(server_sockaddr_.sun_path, server_path_.c_str()); 
    
    // Unlink the file so the bind will succeed, then bind to that file
    unlink(server_path_.c_str());
    if (bind(server_sock_, (struct sockaddr*) &server_sockaddr_, sizeof(server_sockaddr_)) == 1)
    {
        return false;
    }

    return true;
}

} // namespace detail
